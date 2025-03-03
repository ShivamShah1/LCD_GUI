#ifndef WPACTRLIFC_H
#define WPACTRLIFC_H

#include <QObject>
#include <QSocketNotifier>
#include "wpa_ctrl.h"

class WpaCtrlIfc : public QObject
{
    Q_OBJECT
public:
    WpaCtrlIfc();
    ~WpaCtrlIfc();
    QStringList ssidlist;

protected:
    void timerEvent(QTimerEvent *);

signals:
    void scanUpdate(QStringList const &ssidList);
    void statusUpdate(QString const &wpastatus, QString const &ssid, QString const &ip);

private:
    struct wpa_ctrl* monitor_conn;
    struct wpa_ctrl* ctrl_conn;
    QSocketNotifier* msgNotifier;
    void setUpWifi();
    void updateResults();
    QStringList getStatus();
    int timerId;
    void setNetworkValue(int networkId, QString variable, QString value);
    int addNetwork();
    void selectNetwork(int networkId);
    void enableNetwork(int networkId);
    int networkId;
    void saveConfig();
    void kickPumpd();

public slots:
    void onMsgReceived();
    void onRequestStatus();
    void onRequestScan();
    void onRequestConnect(QString apname, QString password);
};

#endif // WPACTRLIFC_H
