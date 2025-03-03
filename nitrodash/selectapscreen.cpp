#include "selectapscreen.h"
#include "ui_selectapscreen.h"
#include <QScrollBar>
#include <QDebug>

SelectAPScreen::SelectAPScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectAPScreen)
{
    ui->setupUi(this);
    ui->listWidget->verticalScrollBar()->setStyleSheet("width: 25px; background: #323232;");
    //ui->listWidget->setStyleSheet("QListWidget::item { border-bottom: 1px solid #323232; }");
    ui->listWidget->addItem("Scanning...");
    ui->acceptButton->setEnabled(false);
}

SelectAPScreen::~SelectAPScreen()
{
    delete ui;
}

void SelectAPScreen::updateList(QStringList List) {
    //ui->label_status->setText(List.first());
    ui->listWidget->clear();
    ui->listWidget->addItems(List);
    ui->acceptButton->setEnabled(true);
}

void SelectAPScreen::clearList() {
    ui->listWidget->clear();
    ui->listWidget->addItem("Scanning...");
    ui->acceptButton->setEnabled(false);
}

void SelectAPScreen::slot1() {
    //qDebug() << "Selected:";
    QList<QListWidgetItem*> selected = ui->listWidget->selectedItems();
    if (!selected.isEmpty()) {
        //qDebug() << selected.first()->text();
        QString apname = selected.first()->text();
        emit selectedAPName(apname);
    }
    else {
        emit returnToHome();
    }
}


void SelectAPScreen::slot2() {
    emit returnToHome();
}
