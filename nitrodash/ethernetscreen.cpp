#include "ethernetscreen.h"
#include "ui_ethernetscreen.h"
#include <QDebug>
#include <QNetworkInterface>
#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>

EthernetScreen::EthernetScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EthernetScreen)
{
    ui->setupUi(this);
    timerId = startTimer(1000);
}

EthernetScreen::~EthernetScreen()
{
    killTimer(timerId);
    delete ui;
}

void EthernetScreen::timerEvent(QTimerEvent *) {

    // Interface status
    QNetworkInterface iface = QNetworkInterface::interfaceFromName("eth0");
    if (iface.flags().testFlag(QNetworkInterface::IsUp) == true)
    {
        ui->label_status->setText("Interface Status: Up");
    }
    else
    {
        ui->label_status->setText("Interface Status: Down");
    }

    // Interface IP address
    QList<QNetworkAddressEntry> entries = iface.addressEntries();
    if (!entries.isEmpty())
    {
        QNetworkAddressEntry entry = entries.first();
        QHostAddress ip = entry.ip();
        ui->label_ipaddr->setText("IP Address: " + ip.toString());
    }
    else
    {
        ui->label_ipaddr->setText("IP Address:");
    }
}

void EthernetScreen::slot1()
{
    //qDebug() << "slot1()";
    emit returnToHome();
}
