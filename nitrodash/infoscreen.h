#ifndef INFOSCREEN_H
#define INFOSCREEN_H

#include <QDialog>

namespace Ui {
class InfoScreen;
}

class InfoScreen : public QDialog
{
    Q_OBJECT

public:
    explicit InfoScreen(QWidget *parent = 0);
    ~InfoScreen();

private:
    Ui::InfoScreen *ui;
};

#endif // INFOSCREEN_H
