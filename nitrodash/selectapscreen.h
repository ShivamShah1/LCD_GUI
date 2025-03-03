#ifndef SELECTAPSCREEN_H
#define SELECTAPSCREEN_H

#include <QDialog>

namespace Ui {
class SelectAPScreen;
}

class SelectAPScreen : public QDialog
{
    Q_OBJECT

public:
    explicit SelectAPScreen(QWidget *parent = 0);
    ~SelectAPScreen();
    void updateList(QStringList);
    void clearList();

private:
    Ui::SelectAPScreen *ui;

public slots:
    void slot1();
    void slot2();

signals:
    void returnToHome();
    void selectedAPName(QString apname);
};

#endif // SELECTAPSCREEN_H
