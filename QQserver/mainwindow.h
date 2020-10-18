#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QTableWidgetItem>
#include <QTimer>
#include <QMapIterator>
//用户信息
class RegInfo
{
    //友元类
    friend class MainWindow;
public:
    RegInfo(){
    }
    RegInfo(QString newname,QString u,QString ps,QString pe,QString path)
    {
        name=newname;
        username=u;
        pass=ps;
        phone=pe;
        headpath = path;
    }
    ~RegInfo()
    {
    }
    void setMsg(QString newname,QString u,QString ps,QString pe,QString path);
    void setMsg(int newremember,int newautomatic);
    void clear();
    void setName(QString newname);
    void setPass(QString newpass);
    void setPhone(QString newphone);
    void setPath(QString newpath);
    bool operator==(const RegInfo &Reg);
    RegInfo operator=(const RegInfo &Reg);
protected:
    void setMsg(QString ip,int port);
    QString username;  //登录账号
    QString socketip; //用户ip
    int socketport;       //端口号
private:

    QString name;   //用户昵称
    QString pass;      //密码
    QString phone;     //手机号码
    QString headpath;

};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void getreg();
    int find(QString str, double);
    int find(QString name);
    void reg(QString regcont);
    int login(QTcpSocket *p,QString logcont);
    void chat();
    void getlist();

    void findpass(QString num, QString newpass, QString phone);
    void setname(QString num, QString newname);
    void setphone(QString num, QString oldphone, QString newphone);
    void setpath(QString num, QString newpath);
    void findfriend(QTcpSocket *p,QString findname);
    void chat(QTcpSocket *usersocked, QString senmum, QString text, QString text1);
    QTcpSocket *find(QString ip,int port);
    RegInfo *find(QTcpSocket *p,QString num);

    void logoff();
    QString readimage(QString path);
    void sendhead(QTcpSocket *p,QString num);
    void savefriend(RegInfo *user, RegInfo *otheruser);
    void sendaddfriendflag(QTcpSocket *p, QString myname, QString findname);

    void getfriend(QString usernum);

public slots:
    void readyConnection();
    void quitConnection();
    void toread();
    void setreg();
    void sendaddfriend();
    void sendfriends();
private:
    Ui::MainWindow *ui;
    QTcpServer *myserver;    //服务器
    QList<RegInfo> clientsocket;   //连接到服务器的客户端,在线用户
    QList<QTcpSocket *> clientsockets; //连接服务的套接字
    RegInfo clientuser;   //最新上线的客户端信息即当前客户端
    int row;   //表格列表的行
    QList<RegInfo> reginfo;   //已注册的用户信息
    QMap<RegInfo*,QString> friendinfo;   //添加好友是未在线好友-发起添加好友和对象
    QMap<RegInfo*,QString>::const_iterator itf;   //只读迭代器
    QList<RegInfo *> addinfo;   //发起申请添加好友的用户信息
    QMap<QString,QString> friendsinfo;   //好友列表
    QMap<QString,QString>::const_iterator itfs;  //只读迭代器
    QTimer mytimer;
    QTimer mytimerf;
    QTimer mytimerfs;
    QTcpSocket *recv;
    QTcpSocket *recvfriend;
    QString offnum;
    int count;
};

#endif // MAINWINDOW_H
