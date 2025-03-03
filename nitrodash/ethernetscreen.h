#ifndef ETHERNETSCREEN_H
#define ETHERNETSCREEN_H

#include <QDialog>

namespace Ui {
class EthernetScreen;
}

class EthernetScreen : public QDialog
{
    Q_OBJECT

public:
    explicit EthernetScreen(QWidget *parent = 0);
    ~EthernetScreen();

private:
    Ui::EthernetScreen *ui;
    int timerId;

protected:
    void timerEvent(QTimerEvent *);

public slots:
    void slot1();

signals:
    void returnToHome();
};

#endif // ETHERNETSCREEN_H
