#ifndef ISADD_H
#define ISADD_H

#include <QDialog>
#include "mainwindow.h"
#include <QMap>
#include <QListWidgetItem>
namespace Ui {
class isadd;
}

class isadd : public QDialog
{
    Q_OBJECT

public:
    explicit isadd(QWidget *parent = 0);
    ~isadd();
    void disfriend(QString itemtext, QString headpath);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::isadd *ui;
    MainWindow *pparent;
    QString selectname;
    QMap<QString,QString> select;
    int row;
};

#endif // ISADD_H
