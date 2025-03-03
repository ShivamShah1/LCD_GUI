#ifndef WIFISCREEN_H
#define WIFISCREEN_H

#include <QDialog>

namespace Ui {
class WifiScreen;
}

class WifiScreen : public QDialog
{
    Q_OBJECT

public:
    explicit WifiScreen(QWidget *parent = 0);
    ~WifiScreen();
    void updateStatusDisplay(QString, QString, QString);

private:
    Ui::WifiScreen *ui;

public slots:
    void slot1();
    void slot2();

signals:
    void returnToHome();
    void selectAP();
};

#endif // WIFISCREEN_H
