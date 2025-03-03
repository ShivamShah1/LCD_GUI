#include "settingsscreen.h"
#include "ui_settingsscreen.h"
#include "wifiscreen.h"
#include "ethernetscreen.h"

SettingsScreen::SettingsScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsScreen)
{
    ui->setupUi(this);
}

SettingsScreen::~SettingsScreen()
{
    delete ui;
}

void SettingsScreen::slot1() {
    emit showWifiScreen();
}

void SettingsScreen::slot2() {
    emit showEthernetScreen();
}
