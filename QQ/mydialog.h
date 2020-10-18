#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "mainwindow.h"
class mydialog : public QDialog
{
    Q_OBJECT
    friend class MainWindow;
public:
    explicit mydialog(QWidget *parent = 0);
    ~mydialog();
signals:

public slots:
    void regfinished();
    void cancelreg();
    void findpass();
    void closeerr();
    void closesucc();
    void findsucc();
    void isUseable(const QString &arg1);
private:
    QLineEdit usered;
    QLineEdit passed;
    QLineEdit phoneed;
    QPushButton reg;
    QPushButton cancel;
    mydialog *succdialog;
    mydialog *errdialog;
    int flag;           //用户名是否被注册了
    static int childflag;  //是否需要建立子窗口
    MainWindow *parentwindow;    //父窗口
    QString username;
    QString phone;
    QString pass;
    int passflag;
};

#endif // MYDIALOG_H
