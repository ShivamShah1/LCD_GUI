#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include <QDialog>
#include <QDebug>
#include "wifiscreen.h"
#include "ethernetscreen.h"

namespace Ui {
class SettingsScreen;
}

class SettingsScreen : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsScreen(QWidget *parent = 0);
    ~SettingsScreen();

private:
    Ui::SettingsScreen *ui;

public slots:
    void slot1();
    void slot2();

protected:

signals:
    void showWifiScreen();
    void showEthernetScreen();
};

#endif // SETTINGSSCREEN_H
