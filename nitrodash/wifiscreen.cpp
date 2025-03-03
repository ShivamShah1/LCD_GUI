#include "wifiscreen.h"
#include "ui_wifiscreen.h"
#include "wpactrlifc.h"
#include <QDebug>

WifiScreen::WifiScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WifiScreen)
{
    ui->setupUi(this);
    //ui->label_status->setText("hello");
}

WifiScreen::~WifiScreen()
{
    delete ui;
}

void WifiScreen::updateStatusDisplay(QString wpastatus, QString ssid, QString ip) {
    ui->label_status->setText("Connection Status: " + wpastatus);
    ui->label_ap->setText("Current AP: " + ssid);
    ui->label_ipaddr->setText("IP Address: " + ip);
}

void WifiScreen::slot1()
{
    //qDebug() << "slot1()";
    emit returnToHome();
}

void WifiScreen::slot2()
{
    //qDebug() << "slot2()";
    emit selectAP();
}
