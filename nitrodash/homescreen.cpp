#include "homescreen.h"
#include "ui_homescreen.h"
#include "infoscreen.h"
#include "packetsscreen.h"
#include "settingsscreen.h"
#include "keyboard/keyboard.h"
#include <QDate>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QNetworkInterface>
#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>

HomeScreen::HomeScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeScreen)
{
    ui->setupUi(this);

    timerId = startTimer(1000);
    i = new InfoScreen();
    p = new PacketsScreen();
    //s = new Keyboard();
    s = new SettingsScreen();
    w = new WifiScreen();
    e = new EthernetScreen();
    sap = new SelectAPScreen();
    pk = new Keyboard();

    connect(s, &SettingsScreen::showWifiScreen, this, &HomeScreen::onShowWifiScreen);
    connect(s, &SettingsScreen::showEthernetScreen, this, &HomeScreen::onShowEthernetScreen);
    connect(w, &WifiScreen::returnToHome, this, &HomeScreen::onReturnToHome);
    connect(e, &EthernetScreen::returnToHome, this, &HomeScreen::onReturnToHome);
    connect(w, &WifiScreen::selectAP, this, &HomeScreen::onSelectAP);
    connect(sap, &SelectAPScreen::returnToHome, this, &HomeScreen::onReturnToHome);
    connect(sap, &SelectAPScreen::selectedAPName, this, &HomeScreen::onSelectedAPName);
    connect(pk, &Keyboard::enteredPassword, this, &HomeScreen::onEnteredPassword);
}

HomeScreen::~HomeScreen()
{
    killTimer(timerId);
    delete ui;
}

void HomeScreen::timerEvent(QTimerEvent *) {
    QFile file("/proc/uptime");
    double real_uptime = 0;
    uint mins, hrs, days;

    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
       QTextStream stream(&file);
       QString line;

       do
        {
          line = stream.readLine();
          if (!line.isEmpty())
             real_uptime = line.section(" ", 0, 0).trimmed().toDouble();
        }
       while (!line.isNull());
    }

    int int_real_uptime = (int)real_uptime;

    mins = int_real_uptime / 60 % 60;
    hrs = int_real_uptime / 3600 % 24;
    days = int_real_uptime / 86400;

    QString result = "System Uptime: " + QString::number(days) + " days, " + QString::number(hrs) + " hrs, " + QString::number(mins) + " min";
    ui->uptimeLabel->setText(result);

    ui->dateLabel->setText(QDate::currentDate().toString("MMMM d, yyyy"));
    ui->timeLabel->setText(QTime::currentTime().toString("h:mm:ss a t"));

    QNetworkInterface iface = QNetworkInterface::interfaceFromName("eth0");
    QList<QNetworkAddressEntry> entries = iface.addressEntries();
    if (!entries.isEmpty())
    {
        QNetworkAddressEntry entry = entries.first();
        QHostAddress ip = entry.ip();
        ui->ipLabel->setText("Ethernet IP: " + ip.toString());
    }
    else
    {
        ui->ipLabel->setText("Ethernet IP: None");
    }
}

void HomeScreen::slot1() {
    //qDebug() << "slot1 fired!";
    /*
    InfoScreen infoscreen;
    infoscreen.setModal(true);
    infoscreen.exec();
    */
    i->show();
}

void HomeScreen::slot2() {
    //qDebug() << "slot2 fired!";
    /*
    PacketsScreen pktscreen;
    pktscreen.setModal(true);
    pktscreen.exec();
    */
    p->show();
}

void HomeScreen::slot3() {
    //qDebug() << "slot3 fired!";
    /*
    PacketsScreen pktscreen;
    pktscreen.setModal(true);
    pktscreen.exec();
    */
    s->show();
}

void HomeScreen::onSelectedAPName(QString name) {
    //qDebug() << "HomeScreen::onSelectedAPName()";
    apname = name;
    sap->hide();
    //ep->show();
    pk->show();
}

void HomeScreen::onEnteredPassword(QString password) {
    //qDebug() << "HomeScreen::onEnteredPassword";
    pk->hide();
    //qDebug() << "RequestConnect: " + apname + "," + password;
    emit requestConnect(apname, password);
}

void HomeScreen::onScanUpdate(QStringList const &ssidList) {
    //qDebug() << "HomeScreen::onScanUpdate()";
    sap->updateList(ssidList);
}

void HomeScreen::onStatusUpdate(QString const &wpastatus, QString const &ssid, QString const &ip) {
    w->updateStatusDisplay(wpastatus, ssid, ip);
}

void HomeScreen::onGpsUpdate(QString const &gpslocation) {
    //qDebug() << gpslocation;
    ui->gpsLabel->setText(gpslocation);
}

void HomeScreen::onShowWifiScreen() {
    emit requestStatus();
    this->hide();
    w->show();
    s->hide();
}

void HomeScreen::onShowEthernetScreen() {
    this->hide();
    e->show();
    s->hide();
}

void HomeScreen::onReturnToHome() {
    sap->hide();
    s->hide();
    w->hide();
    e->hide();
    p->hide();
    i->hide();
    this->show();
}

void HomeScreen::onSelectAP() {
    //qDebug() << "HomeScreen::onSelectAP()";
    emit requestScan();
    sap->clearList();
    sap->show();
}
