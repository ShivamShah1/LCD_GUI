#ifndef PACKETSSCREEN_H
#define PACKETSSCREEN_H

#include <QDialog>
#include <QUdpSocket>

namespace Ui {
class PacketsScreen;
}

class PacketsScreen : public QDialog
{
    Q_OBJECT

public:
    explicit PacketsScreen(QWidget *parent = 0);
    ~PacketsScreen();
    void showEvent(QShowEvent * event);

public slots:
    void readyRead();

private:
    Ui::PacketsScreen *ui;
    QUdpSocket *socket;
};

#endif // PACKETSSCREEN_H
