#include "wpactrlifc.h"
#include <QDebug>
#include <QProcess>
#include <unistd.h>
#include "config.h"


WpaCtrlIfc::WpaCtrlIfc() :
    ssidlist(0),
    monitor_conn(0),
    msgNotifier(0)
{
    //qDebug() << "WpaCtrlIfc()";
    qRegisterMetaType<QStringList>();
    setUpWifi();
    timerId = startTimer(2000);
}

WpaCtrlIfc::~WpaCtrlIfc() {

    if(msgNotifier) {
        delete msgNotifier;
        msgNotifier = 0;
    }
    killTimer(timerId);
}

void WpaCtrlIfc::timerEvent(QTimerEvent *) {
    //qDebug() << "WpaCtrlIfc::timerEvent";
    onRequestStatus();
}


void WpaCtrlIfc::setUpWifi() {
    //qDebug() << "setUpWifi()";

#ifndef DESKTOP
    // Start wpa-supplicant...
    QProcess process;
    process.setProgram("wpa_supplicant");
    QStringList arguments;
    arguments << "-iwlan0" << "-c/etc/wpa_supplicant.conf";
    process.setArguments(arguments);
    process.setWorkingDirectory("/home/ubuntu/nitrodash");
    qint64 pid;
    process.waitForStarted(5000);
    process.startDetached(&pid);
    sleep(2);
#endif

#ifndef DESKTOP
    monitor_conn = wpa_ctrl_open("/var/run/wpa_supplicant/wlan0");
    ctrl_conn = wpa_ctrl_open("/var/run/wpa_supplicant/wlan0");
#endif

#ifdef DESKTOP
    monitor_conn = wpa_ctrl_open("/var/run/wpa_supplicant/wlxe8de2708c141");
    ctrl_conn = wpa_ctrl_open("/var/run/wpa_supplicant/wlxe8de2708c141");
#endif

    qDebug() << "Opening wpa_ctrl interface.";
    if (!monitor_conn) {
        qDebug() << "Error: Couldn't open wpa_ctrl interface.";
	exit(-1);
    }
    else {
        if(wpa_ctrl_attach(monitor_conn)) {
            qDebug() << "Failed to attach wpa_ctrl interface monitor connection.";
	    exit(-1);
        }
        else {
            msgNotifier = new QSocketNotifier(wpa_ctrl_get_fd(monitor_conn), QSocketNotifier::Read, this);
            connect(msgNotifier, SIGNAL(activated(int)), this, SLOT(onMsgReceived()));
	    getStatus();
        }
    }
}

void WpaCtrlIfc::onMsgReceived() {
    //qDebug() << "onMsgReceived()";

    char buf[256];
    size_t len;

    while(monitor_conn && wpa_ctrl_pending(monitor_conn) > 0) {
        len = sizeof(buf) - 1;
        if(wpa_ctrl_recv(monitor_conn, buf, &len) == 0) {
            buf[len] = '\0';
            QString str(buf);
            //qDebug() << str;

            if(str.contains("CTRL-EVENT-SCAN-RESULTS")) {
                //qDebug() << "Scan results available.";
                updateResults();
            }

	    if(str.contains("CTRL-EVENT-CONNECTED")) {
                //qDebug() << "Scan results available.";
		kickPumpd();
                saveConfig();
            }

        }
        else {
            qDebug() << "huh?";
        }
    }
}


QStringList WpaCtrlIfc::getStatus() {
    char buf[4096];
    size_t len = sizeof(buf);
    char status[256];
    int ret;
    QStringList strList;

    snprintf(status, sizeof(status), "STATUS-VERBOSE");
    ret = wpa_ctrl_request(ctrl_conn, status, strlen(status), buf, &len, NULL);

    if(ret < 0) {
        qDebug() << "Error: WpaCtrlIfc::getStatus() failed.";
    }
    else {
        buf[len] = '\0';
        QString str(buf);
        strList = str.split('\n');
        foreach(QString s, strList) {
            //qDebug() << s;
        }
    }
    return strList;
}

void WpaCtrlIfc::setNetworkValue(int networkId, QString variable, QString value) {
    char buf[256];
    size_t len = sizeof(buf);
    char setNetwork[256];

    snprintf(setNetwork, sizeof(setNetwork), "SET_NETWORK %d %s \"%s\"", networkId, variable.toUtf8().constData(), value.toUtf8().constData());
    wpa_ctrl_request(ctrl_conn, setNetwork, strlen(setNetwork), buf, &len, NULL);
}

void WpaCtrlIfc::enableNetwork(int networkId) {
    char buf[256];
    size_t len = sizeof(buf);
    char enableNetwork[256];

    snprintf(enableNetwork, sizeof(enableNetwork), "ENABLE_NETWORK %d", networkId);
    wpa_ctrl_request(ctrl_conn, enableNetwork, strlen(enableNetwork), buf, &len, NULL);
}

int WpaCtrlIfc::addNetwork() {
    char buf[4096];
    size_t len = sizeof(buf);
    char addNetwork[256];
    int networkId = -1;

    snprintf(addNetwork, sizeof(addNetwork), "ADD_NETWORK");
    wpa_ctrl_request(ctrl_conn, addNetwork, strlen(addNetwork), buf, &len, NULL);
    buf[len] = '\0';
    QString str(buf);
    if(str.compare("FAIL") == 0) {
        qDebug() << "Error: WpaCtrlIfc::addNetwork() failed.";
    }
    else {
        str = str.split("\n").at(0);
        networkId = str.toInt();
    }
    return networkId;
}

void WpaCtrlIfc::onRequestConnect(QString ssid, QString passkey)
{
    qDebug() << "WpaCtrlIfc::onRequestConnect: " + ssid + "," + passkey;
    int networkId = addNetwork();
    setNetworkValue(networkId, "ssid", ssid);
    setNetworkValue(networkId, "psk", passkey);
    enableNetwork(networkId);
    selectNetwork(networkId);
}

void WpaCtrlIfc::kickPumpd() {
    qDebug() << "Requesting IP address on wlan0.";

#ifndef DESKTOP
    // Start wpa-supplicant...
    QProcess process;
    process.setProgram("pump");
    QStringList arguments;
    arguments << "-iwlan0" << "-u";
    process.setArguments(arguments);
    process.setWorkingDirectory("/sbin");
    qint64 pid;
    process.waitForStarted(2000);
    process.startDetached(&pid);
#endif
}

void WpaCtrlIfc::saveConfig() {
    char buf[256];
    size_t len = sizeof(buf);
    char saveConfig[256];

    snprintf(saveConfig, sizeof(saveConfig), "SAVE_CONFIG");
    wpa_ctrl_request(ctrl_conn, saveConfig, strlen(saveConfig), buf, &len, NULL);
}

void WpaCtrlIfc::selectNetwork(int networkId) {
    char buf[256];
    size_t len = sizeof(buf);
    char selectNetwork[256];

    snprintf(selectNetwork, sizeof(selectNetwork), "SELECT_NETWORK %d", networkId);
    wpa_ctrl_request(ctrl_conn, selectNetwork, strlen(selectNetwork), buf, &len, NULL);
}

void WpaCtrlIfc::onRequestScan()
{
    char buf[4096];
    size_t len = sizeof(buf);
    char scan[256];

    snprintf(scan, sizeof(scan), "SCAN");
    int ret = wpa_ctrl_request(ctrl_conn, scan, strlen(scan), buf, &len, NULL);

    if(ret < 0) {
        qDebug() << "Error: WpaCtrlIfc::onRequestScan() failed.";
    }
}

void WpaCtrlIfc::onRequestStatus()
{
    //qDebug() << "WpaCtrlIfc::onRequestStatus()";
    QStringList status = getStatus();
    QString wpastatus, ssid, ip;
    QRegExp tagExp("=");
    foreach(QString str, status) {
        if(str.startsWith("wpa_state")) {
            if(str.endsWith("COMPLETED")) {
                wpastatus = "CONNECTED";
            }
            else {
                wpastatus = str.split(tagExp).takeLast();
            }
            //qDebug() << "wpastatus is " + wpastatus;
        }

        if(str.startsWith("ssid")) {
            // ssid
            ssid = str.split(tagExp).takeLast();
            //qDebug() << "ssid is " + ssid;
        }

        if(str.startsWith("ip_address")) {
            // ip
            ip = str.split(tagExp).takeLast();
            //qDebug() << "ip is " + ip;
        }
    }
    emit statusUpdate(wpastatus, ssid, ip);
}

void WpaCtrlIfc::updateResults()
{
    char reply[2048];
    size_t reply_len;
    int index;
    char cmd[20];

    ssidlist.clear();

    index = 0;
    while (1) {
        snprintf(cmd, sizeof(cmd), "BSS %d", index++);
        if (index > 1000)
            break;

        reply_len = sizeof(reply) - 1;
        if (wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), reply, &reply_len, NULL) < 0) {
            qDebug() << "Error: wpa_ctrl_request for bss returned error.";
            break;
        }
        reply[reply_len] = '\0';

        QString bss(reply);
        if (bss.isEmpty() || bss.startsWith("FAIL")) {
            break;
        }

        QString ssid, bssid, freq, signal, flags;

        QStringList lines = bss.split(QRegExp("\\n"));
        for (QStringList::Iterator it = lines.begin();
             it != lines.end(); it++) {
            int pos = (*it).indexOf('=') + 1;
            if (pos < 1)
                continue;

            if ((*it).startsWith("bssid="))
                bssid = (*it).mid(pos);
            else if ((*it).startsWith("freq="))
                freq = (*it).mid(pos);
            else if ((*it).startsWith("level="))
                signal = (*it).mid(pos);
            else if ((*it).startsWith("flags="))
                flags = (*it).mid(pos);
            else if ((*it).startsWith("ssid="))
                ssid = (*it).mid(pos);
        }

        // Ignore ssids with 0x00...  These are supposed to be hidden.
        if (!ssid.isEmpty() & !ssid.startsWith("\\x00")) {
            // Only looks for APs which offer WPA2-PSK
            if (flags.contains("WPA2-PSK")) {
                // Only add the ssid to the list if not already there.
                if (ssidlist.contains(ssid) == false) {
                    ssidlist.append(ssid);
                }
            }
        }

        if (bssid.isEmpty())
            break;
    }
    //qDebug() << ssidlist;
    emit scanUpdate(ssidlist);
}
