#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QList>
#include <QTcpSocket>
#include <QTimer>
#include "keyboard.h"

class RegInfo
{
    //友元类
    friend class mydialog;
    friend class QQ;
    friend class MainWindow;
    friend class addfriend;
    friend class chat;
    friend class rightmenu;
    friend class personally;
    friend class isadd;
public:
    RegInfo(){}
    RegInfo(QString newname,QString u,QString ps,QString pe,QString path)
    {
        name = newname;
        username=u;
        pass=ps;
        phone=pe;
        headpath=path;
    }
    ~RegInfo()
    {
    }
    void setMsg(QString newname,QString u,QString ps,QString pe,QString path);

    void clear();
    bool isEmpty();
    void setName(QString newname);
    void setPass(QString newpass);
    void setPhone(QString newphone);
    void setPath(QString newpath);
    RegInfo operator=(const RegInfo &Reg);
protected:
    void setMsg(QString ip,int port);
private:
    QString name;   //用户昵称
    QString username;  //登录账号
    QString pass;      //密码
    QString phone;     //手机号码
    QString headpath;
    QString ip;        //登录ip地址
    int port;       //登录端口号
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    //友元类
    friend class mydialog;
    friend class chat;
    friend class QQ;
    friend class addfriend;
    friend class rightmenu;
    friend class personally;
    friend class keyboard;
    friend class mypushbutton;
    friend class isadd;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void login(bool is);
    void rememberpass(QString username,QString pass,QString headpath);
    void automaticlogin(QString username, QString pass, QString headpath);
    void saveremembername(QString namenum,QString pass,QString headpath);
    void saveautomaticname(QString namenum, QString pass, QString headpath);
    void save(QString name,QString pass,QString headpath);
    QString get();
    void chatonebyone(QStringList readlist);
    QMap<QString,QString> *getlogin();
    void savelogin(QString num,QString path);

private slots:
    void getrand();
    void hasconnected();
    void toreadcliet();
    void quitConnection();
    void login();
    void set();

    void on_hide_clicked();

    void on_reg_clicked();

    void on_automatic_login_clicked();

    void on_rememberpass_clicked();

    void on_findpass_clicked();

    void on_login_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);
    void showpopup();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    int remember;    //记住密码
    int automatic;    //自动登录
    int automaticflag;
    RegInfo user;    //当前登录的用户名信息
    QString imagebuf;  //图片数据
    QString path;    //头像路径
    QString headimage;
    QList<RegInfo> rememberuser;  //保存记住密码的用户
    QList<RegInfo> automaticuser;  //保存自动登录的用户
    QList<RegInfo> rauser;   //保存自动登录和记住密码的用户
    int flag;            //隐藏/显示密码
    int connectflag;    //0连接失败，1连接成功

    QTcpSocket *mysorcket;    //客户端
    QList<RegInfo> *reginfo;  //获取已注册的信息
    QMap<QString,QString> *loginlist;   //获取在本地登录过的账号
    int loginflag;  //登录过的标识
    int friendflag; //好友同意通知标志
    int friendaddflag; //好友添加标志
    RegInfo friendinfo;
    QList<RegInfo> friendadd;
    QTimer mytimer;
    QTimer mytimer1;
    QTimer randtimer;
    int rand;
    int randcount;
    QList<QStringList> loginlistly;
    int key;
    keyboard *mykeyboard;
};

#endif // MAINWINDOW_H
