#ifndef RIGHTMENU_H
#define RIGHTMENU_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class rightmenu;
}

class rightmenu : public QWidget
{
    Q_OBJECT

public:
    explicit rightmenu(QWidget *parent = 0);
    ~rightmenu();
    void get(QString usernumber);
private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

private:
    Ui::rightmenu *ui;
    QString usernum;
};

#endif // RIGHTMENU_H
