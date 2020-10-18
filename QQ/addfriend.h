#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QDialog>
#include "mainwindow.h"
#include <QListWidget>
#include <QAbstractButton>
#include <QList>
#include <QTimer>
#include "keyboradl.h"
namespace Ui {
class addfriend;
}

class addfriend : public QDialog
{
    Q_OBJECT
    friend class mypushbutton;
public:
    QString newfriend;

    explicit addfriend(QWidget *parent = 0);
    ~addfriend();
    void set(int key);
private slots:

    void on_pushButton_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void display();

    void openkeyborad();
private:
    Ui::addfriend *ui;
    MainWindow * myparent;
    QList<QListWidget *> *listwidgetlist;
    QString friendname;
    QTimer mytimer;
    keyboradl *mykey;
};

#endif // ADDFRIEND_H
