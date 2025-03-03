#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QDialog>
#include <QDebug>
#include "infoscreen.h"
#include "packetsscreen.h"
#include "settingsscreen.h"
#include "selectapscreen.h"
#include "keyboard/keyboard.h"

namespace Ui {
class HomeScreen;
}

class HomeScreen : public QDialog
{
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = 0);
    ~HomeScreen();
    void setUpWifi();

private:
    Ui::HomeScreen *ui;
    int timerId;

protected:
    void timerEvent(QTimerEvent *);
    InfoScreen *i;
    PacketsScreen *p;
    SettingsScreen *s;
    WifiScreen *w;
    EthernetScreen *e;
    SelectAPScreen *sap;
    Keyboard *pk;
    QString apname;
    QString password;

public slots:
    void slot1();
    void slot2();
    void slot3();
    void onScanUpdate(QStringList const &ssidList);
    void onStatusUpdate(QString const &wpastatus, QString const &ssid, QString const &ip);
    void onShowWifiScreen();
    void onShowEthernetScreen();
    void onReturnToHome();
    void onSelectAP();
    void onSelectedAPName(QString apname);
    void onEnteredPassword(QString password);
    void onGpsUpdate(QString const &gpslocation);

signals:
    void requestStatus();
    void requestScan();
    void requestConnect(QString apname, QString password);
};

#endif // HOMESCREEN_H
