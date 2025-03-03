#include "packetsscreen.h"
#include "ui_packetsscreen.h"
#include <QListWidget>
#include <QScrollBar>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

PacketsScreen::PacketsScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PacketsScreen)
{
    ui->setupUi(this);
    //ui->listWidget->setStyleSheet(QString("QScrollBar:horizontal { border: 2px solid grey; background: #32CC99; height: 15px; margin: 0px 20px 0 20px; }"));
    ui->listWidget->verticalScrollBar()->setStyleSheet("width: 25px; background: #323232;");
    ui->listWidget->horizontalScrollBar()->setStyleSheet("height: 25px; background: #323232;");
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::Any,1681);
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

PacketsScreen::~PacketsScreen()
{
    delete ui;
}

void PacketsScreen::showEvent(QShowEvent * event) {
    QWidget::showEvent(event);
    ui->listWidget->scrollToBottom();
}

void PacketsScreen::readyRead()
{
    QByteArray Buffer, Payload;
    QString msg;
    Buffer.resize(socket->pendingDatagramSize());
    QJsonObject::iterator it;
    QHostAddress sender;
    quint16 senderPort;

    socket->readDatagram(Buffer.data(),Buffer.size(),&sender,&senderPort);

    //qDebug() << "Message from: " << sender.toString();
    //qDebug() << "Message port: " << senderPort;
    //qDebug() << "Message: " << Buffer.simplified();

    Payload = Buffer.simplified().mid(12);
    msg = QString::fromStdString(Payload.toStdString());
    //qDebug() << "Msg: " << msg;

    if (msg.simplified().isEmpty() == false) {
        QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
        QJsonObject obj = doc.object();
        it = obj.begin();
        //qDebug() << it.key();

        // Only show packet if it doesn't start with "stat" key
        if (it.key() != "stat")
        {
            ui->listWidget->addItem(msg.simplified());

            if (ui->listWidget->count() > 25) {
                ui->listWidget->takeItem(0);
            }

            ui->listWidget->scrollToBottom();
        }
    }
}
