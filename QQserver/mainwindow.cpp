#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QFileDevice>
#include <QTest>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    count=0;
    myserver=new QTcpServer;
    clientsocket.clear();
    //获取注册信息
    reginfo.clear();
    getreg();
    //窗口图标和名称
    setWindowIcon(QIcon(":/new/prefix1/qq.jpg"));
    setWindowTitle("QQ服务器");
    myserver->listen(QHostAddress::Any,65530);  //监听绑定服务器ip端口号 QHostAddress::Any
    qDebug()<<myserver->serverAddress().toString();
    //关联信号与槽函数
    connect(myserver,SIGNAL(newConnection()),this,SLOT(readyConnection()));
    //设置水平头
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"昵称"<<"账号"<<"密码"<<"手机号码"<<"用户ip"<<"用户端口号");
    //设置表格大小（行和列）
    row=0;   //第一行
    //设置列数
    ui->tableWidget->setColumnCount(6);
    //设置行数
    ui->tableWidget->setRowCount(row+1);
    connect(&mytimer,SIGNAL(timeout()),this,SLOT(setreg()));
    connect(&mytimerf,SIGNAL(timeout()),this,SLOT(sendaddfriend()));
    connect(&mytimerfs,SIGNAL(timeout()),this,SLOT(sendfriends()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//获取注册信息
void MainWindow::getreg()
{
    QFile regtext("F:/QT/protect/day08/TXT/reg.txt");
    char buf[100]={0};
    if(regtext.open(QIODevice::ReadOnly))
    {
        while(!regtext.atEnd())
        {
            memset(buf,0,100);
            regtext.readLine(buf,100);
            QString temp(buf);
            temp=temp.remove("\n");  //删除"\n"
            QStringList templist = temp.split("::");
            if(templist.size()>=5)
            {
                RegInfo reg(templist.at(0),templist.at(1),templist.at(2),templist.at(3),templist.at(4));
                reginfo.append(reg);
            }
        }
    }
    regtext.close();
}

//qint16 转换成int整型
int Qint16_toint(int port)
{
    return port;
}
//查找用户信息
int MainWindow::find(QString name)
{
    for(int i=0;i<clientsocket.size();i++)
    {
        if(name==clientsocket.at(i).username)
        {
            return i;
        }
    }
    return -1;
}
//遍历查找列表项  --  成功返回行号，失败返回-1
int MainWindow::find(QString str, double)
{
    int rowtemp=0;
    for(;rowtemp < ui->tableWidget->rowCount();rowtemp++)
    {
        if(str == ui->tableWidget->item(rowtemp,1)->text())
            return rowtemp;
    }
    return -1;
}

//保存注册信息
void MainWindow::reg(QString regcont)
{
    qDebug()<<regcont;
    QStringList templist=regcont.split("::");
    RegInfo newuser(templist.at(0),templist.at(1),templist.at(2),templist.at(3),templist.at(4));
    reginfo.append(newuser);
    QFile regtext("F:/QT/protect/day08/TXT/reg.txt");
    if(regtext.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        regcont=QString("%1\n").arg(regcont);
        regtext.write(regcont.toUtf8());
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    regtext.close();
}
//登录验证
int MainWindow::login(QTcpSocket *p,QString logcont)
{
    //查看是否已在线
    QStringList logcontlist = logcont.split("::");
    for(int i=0;i<clientsocket.size();i++)
    {
        if(logcontlist.at(0)==clientsocket.at(i).username)
        {
            p->write("@#$%logining");
            return -1;
        }
    }
    for(auto&i:reginfo)
    {
        if(logcontlist.at(0)==i.username&&logcontlist.at(1)==i.pass)
        {
            p->write("@#$%argee");  //登录成功
            clientuser=i;
            clientuser.setMsg(p->peerAddress().toString().mid(7),p->peerPort());
            clientsocket.append(clientuser);  //添加连接用户，即在线用户

            //设置行数
            ui->tableWidget->setRowCount(row+1);
            //添加到表格列表表框中
            QTableWidgetItem *newitem1=new QTableWidgetItem(i.name);
            QTableWidgetItem *newitem2=new QTableWidgetItem(i.username);
            QTableWidgetItem *newitem3=new QTableWidgetItem(i.phone);
            QTableWidgetItem *newitem4=new QTableWidgetItem(i.headpath);
            QTableWidgetItem *itemip=new QTableWidgetItem(p->peerAddress().toString().mid(7));
            QString portstr = QString::number(p->peerPort());
            QTableWidgetItem *itemport=new QTableWidgetItem(portstr);

            ui->tableWidget->setItem(row,0,newitem1);    //添加列表项
            ui->tableWidget->setItem(row,1,newitem2);    //添加列表项
            ui->tableWidget->setItem(row,2,newitem3);    //添加列表项
            ui->tableWidget->setItem(row,3,newitem4);    //添加列表项

            ui->tableWidget->setItem(row,4,itemip);    //添加列表项
            ui->tableWidget->setItem(row,5,itemport);    //添加列表项
            row++;
            //判断登录的用户是否是添加好友之前未在线的用户

            for(auto j=friendinfo.cbegin();j!=friendinfo.cend();j++)
            {

                if(clientuser.username == j.value())
                {
                    qDebug()<<clientuser.username;
                    qDebug()<<j.value();
                    recvfriend = p;
                    itf=friendinfo.cbegin();
                    mytimerf.start(1000); //定时1秒
                    break;
                }
            }
            return 0;
        }
    }
    p->write("@#$%unargee");  //登录失败

    return -1;
}
//发送已注册的用户信息
void MainWindow::setreg()
{
    if(reginfo.isEmpty())
    {
        recv->write("@#$%null");  //空文件
        return ;
    }
    QString temp = QString("@#$%getreg::%1::%2::%3::%4::%5").arg(reginfo.at(count).name).\
                    arg(reginfo.at(count).username).arg(reginfo.at(count).pass).\
                    arg(reginfo.at(count).phone).arg(reginfo.at(count).headpath);
    recv->write(temp.toUtf8());
    count++;
    if(count==reginfo.size())
    {
        count=0;
        mytimer.stop();
        return ;
    }
}
//发送添加好友确认---好友不在线等到好友上线时
void MainWindow::sendaddfriend()
{
    qDebug()<<clientuser.username;
    if(itf==friendinfo.cend())
    {
        mytimerf.stop();
        friendinfo.clear();
        itf=friendinfo.cbegin();
        return;
    }

    if(itf.value()==clientuser.username)
    {
        RegInfo *sendinfo = itf.key();
        if(sendinfo!=Q_NULLPTR)
            recvfriend->write(QString("@#$%friendon::%1::%2::%3::%4::%5").\
                        arg(sendinfo->name).arg(sendinfo->username).\
                        arg(sendinfo->pass).arg(sendinfo->phone).arg(sendinfo->headpath).toUtf8());
    }
    itf++;
}

//发送好友信息
void MainWindow::sendfriends()
{
    if(itfs==friendsinfo.cend())
    {
        mytimerfs.stop();
        friendsinfo.clear();
        itfs=friendsinfo.cbegin();
        return;
    }
    recv->write(QString("@#$%friends::%1::%2").\
                arg(itfs.key()).arg(itfs.value()).toUtf8());
    itfs++;
}
//找回密码成功
void MainWindow::findpass(QString name,QString newpass,QString phone)
{
    for(auto& i:reginfo)
    {
        if(name==i.username&&phone==i.phone)
        {
            i.setPass(newpass);
            break;
        }
    }
    QFile regtext("F:/QT/protect/day08/TXT/reg.txt");
    if(regtext.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        for(auto& i:reginfo)
        {
            QString readstr = QString("%1::%2::%3::%4::%5\n").arg(i.name).arg(i.username).arg(i.pass).arg(i.phone).arg(i.headpath);
            regtext.write(readstr.toUtf8());
        }
    }
    regtext.close();
}
//修改昵称
void MainWindow::setname(QString num, QString newname)
{
    for(auto& i:reginfo)
    {
        if(num==i.username)
        {
            i.setName(newname);
            break;
        }
    }
    int tablerow = find(num,1.2);
    if(tablerow!=-1)
        ui->tableWidget->item(tablerow,0)->setText(newname);
    QFile regtext("F:/QT/protect/day08/TXT/reg.txt");
    if(regtext.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        for(auto& i:reginfo)
        {
            QString readstr = QString("%1::%2::%3::%4::%5\n").arg(i.name).arg(i.username).arg(i.pass).arg(i.phone).arg(i.headpath);
            regtext.write(readstr.toUtf8());
        }
    }
    regtext.close();
}
//修改手机号码
void MainWindow::setphone(QString num,QString oldphone, QString newphone)
{
    for(auto& i:reginfo)
    {
        if(num==i.username&&oldphone==i.phone)
        {
            i.setPhone(newphone);
            break;
        }
    }
    int tablerow = find(num,1.2);
    if(tablerow!=-1)
        ui->tableWidget->item(tablerow,2)->setText(newphone);
    QFile regtext("F:/QT/protect/day08/TXT/reg.txt");
    if(regtext.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        for(auto& i:reginfo)
        {
            QString readstr = QString("%1::%2::%3::%4::%5\n").arg(i.name).arg(i.username).arg(i.pass).arg(i.phone).arg(i.headpath);
            regtext.write(readstr.toUtf8());
        }
    }
    regtext.close();
}
//修改头像
void MainWindow::setpath(QString num, QString newpath)
{
    for(auto& i:reginfo)
    {
        if(num==i.username)
        {
            i.setPath(newpath);
            break;
        }
    }
    int tablerow = find(num,1.2);
    if(tablerow!=-1)
        ui->tableWidget->item(tablerow,3)->setText(newpath);
    QFile regtext("F:/QT/protect/day08/TXT/reg.txt");
    if(regtext.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        for(auto& i:reginfo)
        {
            QString readstr = QString("%1::%2::%3::%4::%5\n").arg(i.name).arg(i.username).arg(i.pass).arg(i.phone).arg(i.headpath);
            regtext.write(readstr.toUtf8());
        }
    }
    regtext.close();
}

//查找好友
void MainWindow::findfriend(QTcpSocket *p,QString findname)
{
    qDebug()<<findname;
    //遍历注册内容账号
    for(auto& i:reginfo)
    {
        if(findname == i.username)
        {
            QString temp=QString("@#$%friend::%1::%2::%3::%4::%5").\
                        arg(i.name).arg(i.username).arg(i.pass).arg(i.phone).arg(i.headpath);

            p->write(temp.toUtf8());
            return ;
        }
    }
    p->write("@#$%nofriend");
}

//发送好友申请通知
void MainWindow::sendaddfriendflag(QTcpSocket *p,QString myname,QString findname)
{
    qDebug()<<findname;
    RegInfo *sendinfo = find(p,myname);
    if(sendinfo!=Q_NULLPTR)
    {
        addinfo.append(sendinfo);   //添加发起好友申请的用户
        //先遍历当前在线的用户
        for(auto& j:clientsocket)
        {
            if(findname == j.username)
            {
                qDebug()<<j.username;
                //发送请求添加好友标志
                QTcpSocket *friendsocket = find(j.socketip,j.socketport);
                if(friendsocket!=Q_NULLPTR)
                {
                    //并且发送申请添加好友的客户信息
                    QString temp=QString("@#$%friendon::%1::%2::%3::%4::%5").\
                                          arg(sendinfo->name).arg(sendinfo->username).\
                                        arg(sendinfo->pass).arg(sendinfo->phone).arg(sendinfo->headpath);
                    friendsocket->write(temp.toUtf8());
                    return ;
                }
            }
        }
        //遍历注册内容账号
        for(auto& i:reginfo)
        {
            //如果有则保存起来，待该好友上线时发送添加指令
            if(findname == i.username)
            {
                friendinfo.insert(sendinfo,findname);    //添加到QMap容器
                return ;
            }
        }
    }
}
//获取好友信息
void MainWindow::getfriend(QString usernum)
{
    friendsinfo.clear();
    QFile myfriend(QString("F:/QT/protect/day08/TXT/%1friend.txt").arg(usernum));
    if(myfriend.open(QIODevice::ReadWrite))
    {
        char buf[256]={0};
        QStringList templist;
        while(!myfriend.atEnd())
        {
            memset(buf,0,100);
            myfriend.readLine(buf,256);
            QString temp(buf);
            temp=temp.remove("\n");  //删除换行符"\r\n"
            templist = temp.split("::");
            if(templist.size()>=2)
            {
                friendsinfo.insert(templist.at(0),templist.at(1));
            }
        }
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    myfriend.close();
}

//聊天
void MainWindow::chat(QTcpSocket *usersocked,QString senmum,QString text,QString text1)
{
    int sendnum = find(senmum);
    QString temp = QString("%1(%2)").arg(clientsocket.at(sendnum).name).arg(clientsocket.at(sendnum).username);
    QString send = QString("@#$%onebyone::%1::%2::%3").arg(temp).arg(text).arg(text1);
    usersocked->write(send.toUtf8());
}
//查找聊天对象（套接字）
QTcpSocket *MainWindow::find(QString ip, int port)
{
    for(int i=0;i<clientsockets.size();i++)
    {
        if(clientsockets.at(i)->peerAddress().toString().mid(7)==ip&&clientsockets.at(i)->peerPort()==port)
            return clientsockets.at(i);
    }
    return Q_NULLPTR;
}
//查找个人信息
RegInfo *MainWindow::find(QTcpSocket *p,QString num)
{
    for(auto &i:clientsocket)
    {
        if(num==i.username&&p->peerAddress().toString().mid(7)==i.socketip&&p->peerPort()==i.socketport)
            return &i;
    }
    return Q_NULLPTR;
}
//注销
void MainWindow::logoff()
{
    //遍历查找列表项
    int finerow = find(offnum,1.1);
    if(finerow!=-1)
    {
        //先删除用户信息里的用户
        int ret = find(ui->tableWidget->item(finerow,1)->text());
        if(ret!=-1)
        {
            clientsocket.removeAt(ret);
        }
        //从tableWidget中删除这个客户端---删除行
        ui->tableWidget->removeRow(finerow);
        row--;    //第n行减一
        offnum.clear();
    }
}
//读取图片数据并发送
QString MainWindow::readimage(QString path)
{
    QFile image(QString("F:/QT/protect/day08/QQ/headpic/%1").arg(path));
    if(image.open(QIODevice::ReadOnly))
    {
        QString readbuf = image.readAll();
        return readbuf;
    }
    else
        qDebug()<<"读取失败";
    image.close();
    return QString();
}
//查询头像并发送
void MainWindow::sendhead(QTcpSocket *p, QString num)
{
    for(auto& i:reginfo)
    {
        if(num==i.username)
        {
            p->write(QString("@#$%head::%1").arg(i.headpath).toUtf8());
            return ;
        }
    }
    p->write(QString("@#$%nohead").toUtf8());
}

//保存好友信息
void MainWindow::savefriend(RegInfo *user,RegInfo *otheruser)
{
    QFile myfriend(QString("F:/QT/protect/day08/TXT/%1friend.txt").arg(user->username));
    if(myfriend.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        QString temp=QString("%1(%2)::%3\n").arg(otheruser->name).arg(otheruser->username).arg(otheruser->headpath);
        myfriend.write(temp.toUtf8());
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    myfriend.close();
}

//连接准备
void MainWindow::readyConnection()
{
    clientuser.clear();
    qDebug()<<"有客户端连接";
    //接收连接客户端
    QTcpSocket *socket = myserver->nextPendingConnection();
    clientsockets.append(socket);

    //关联信号与槽函数
    connect(socket,SIGNAL(readyRead()),this,SLOT(toread()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(quitConnection()));

}
//读取准备
void MainWindow::toread()
{
    qDebug()<<"有数据可读";
    //定义接收信息规则
    /*
        reg:xxxx -----》注册账号
        login:xxxxx  ---》登录账号
        昵称:xxxxxx ----》私聊
        getlist:xxxxx  ----》获取已连接的用户
        getreg:xxxxx -----》获取已注册的信息
        findsucceed:xxxx  ---》找回密码成功
        friend:xxxx ---》好友搜索
        friendon:xxxx --》确认添加好友
        friendoff:xxxx --》拒绝添加好友
        onebyone:xxxx --》私聊
        person:xxxx ---》获取个人信息
        logoff:xxx---》注销登录
        name:xxx---》修改昵称
        pass:xxx --》修改密码
        phone:xxx ---》修改手机号码
        path:xxxx ---》修改头像
        getfriend:xxx --》获取好友信息
    */
    //获取信号的发送者--》能够知道究竟是谁发送信息过来了
    recv=qobject_cast<QTcpSocket *>(sender());
    if(recv==Q_NULLPTR)
        return ;
    QString senerstr = recv->readAll();
    qDebug()<<senerstr;
    QStringList newlist = senerstr.split("::");
    QString rule(newlist.at(0));
    if(rule=="@#$%reg")
    {
        QString regcont = senerstr.mid(rule.size()+2);
        reg(regcont);
    }
    else if(rule=="@#$%login")
    {
        qDebug()<<"登录";
        QString logcont = senerstr.mid(rule.size()+2);
        login(recv,logcont);
    }
    else if(rule=="@#$%getreg")
    {
        mytimer.start(100);
    }
    else if(rule=="@#$%gethead")
    {
        sendhead(recv, clientuser.username);
    }
    else if(rule=="@#$%findsucceed")
    {
        findpass(newlist.at(1),newlist.at(2),newlist.at(3));
    }
    else if(rule=="@#$%getfriend")
    {
        getfriend(newlist.at(1));
        itfs = friendsinfo.cbegin();
        mytimerfs.start(100);
    }
    else if(rule=="@#$%friend")
    {
        findfriend(recv,newlist.at(1));
    }
    else if(rule=="@#$%friendadd")
    {
        //发送添加好友信号
        sendaddfriendflag(recv,newlist.at(1),newlist.at(2));
    }
    else if(rule=="@#$%friendon")  //确认添加
    {
        //相互保存好友信息
        RegInfo *curuserinfo = find(recv,newlist.at(1));  //点击同意的客户信息
        QStringList templist = newlist.at(2).split("(");
        templist = templist.at(1).split(")");
        if(templist.size()>=1)
        {
            for(int i=0;i<addinfo.size();i++)    //发起好友申请的用户
            {
                if(addinfo.at(i)->username==templist.at(0))
                {
                    savefriend(curuserinfo,addinfo.at(i));
                    savefriend(addinfo.at(i),curuserinfo);
                    QTcpSocket *sendsoket = find(addinfo.at(i)->socketip,addinfo.at(i)->socketport);
                    sendsoket->write(QString("@#$%friendyes::%1(%2)::%3").\
                                     arg(curuserinfo->name).arg(curuserinfo->username).\
                                     arg(curuserinfo->headpath).toUtf8());
                    addinfo.removeAt(i);
                    break;
                }
            }
        }
        else
            qDebug()<<"发起申请好友信息错误";
    }
    else if(rule=="@#$%friendoff")  //拒绝添加
    {
        //给添加好友发送者发送拒绝信号
        RegInfo *curuserinfo = find(recv,newlist.at(1));  //点击同意的客户信息
        QStringList templist = newlist.at(2).split("(");
        templist = templist.at(1).split(")");
        if(templist.size()>=1)
        {
            for(int i=0;i<addinfo.size();i++)    //发起好友申请的用户
            {
                if(addinfo.at(i)->username==templist.at(0))
                {
                    QTcpSocket *sendsoket = find(addinfo.at(i)->socketip,addinfo.at(i)->socketport);
                    sendsoket->write(QString("@#$%friendoff::%1").arg(curuserinfo->username).toUtf8());
                    addinfo.removeAt(i);
                    break;
                }
            }
        }
    }
    else if(rule=="@#$%onebyone")
    {
        int chatnum = find(newlist.at(1));
        if(chatnum!=-1)
        {
            QTcpSocket *onesock=find(clientsocket.at(chatnum).socketip,clientsocket.at(chatnum).socketport);
            if(onesock!=Q_NULLPTR)
            {
                qDebug()<<"聊天";
                chat(onesock,newlist.at(2),newlist.at(3),newlist.at(4));
            }
        }
    }
    else if(rule=="@#$%person")
    {
        RegInfo *person = find(recv,newlist.at(1));
        if(person!=Q_NULLPTR)
        {
            QString imagebuf = readimage(person->headpath);
            QString personinfo = QString("@#$%person::%1::%2::%3::%4::%5::%6").arg(person->name).\
                                arg(person->username).arg(person->pass).\
                                arg(person->phone).arg(person->headpath).arg(imagebuf);
            recv->write(personinfo.toUtf8());
        }
    }
    else if(rule=="@#$%logoff")
    {
        offnum=newlist.at(1);
        logoff();
    }
    else if(rule=="@#$%name")
    {
        setname(newlist.at(1),newlist.at(2));
    }
    else if(rule=="@#$%pass")
    {
        findpass(newlist.at(1),newlist.at(2),newlist.at(3));
    }
    else if(rule=="@#$%phone")
    {
        setphone(newlist.at(1),newlist.at(2),newlist.at(3));
    }
    else if(rule=="@#$%path")
    {
        setpath(newlist.at(1),newlist.at(2));
    }
}
//客户端下线信号
void MainWindow::quitConnection()
{
    qDebug()<<"有客户端下线";
    //获取信号的发送者--》能够知道究竟是谁断开了
    QTcpSocket *p=qobject_cast<QTcpSocket *>(sender());
    qDebug()<<"ip地址为"<<p->peerAddress()<<"的客户端下线了!";
    //delete p;
}
//设置信息
void RegInfo::setMsg(QString ip,int port)
{
    socketip=ip;
    socketport=port;
}

void RegInfo::setMsg(QString newname, QString u, QString ps, QString pe, QString path)
{
    name=newname;
    username=u;
    pass=ps;
    phone=pe;
    headpath = path;
}

//清空
void RegInfo::clear()
{
    name.clear();
    username.clear();
    pass.clear();
    phone.clear();
    socketip.clear();
    socketport=0;
}
//修改昵称
void RegInfo::setName(QString newname)
{
    name = newname;
}
//修改密码
void RegInfo::setPass(QString newpass)
{
    pass = newpass;
}
//修改手机号码
void RegInfo::setPhone(QString newphone)
{
    phone = newphone;
}
//修改图片
void RegInfo::setPath(QString newpath)
{
    headpath = newpath;
}
//重载双目运算符==
bool RegInfo::operator==(const RegInfo &Reg)
{
    if(Reg.name==name&&Reg.username==username&&   \
            Reg.pass==pass&&Reg.phone==phone&&      \
            Reg.socketip==socketip&&Reg.socketport==socketport)
        return  true;
    else
        return  false;
}

//重载赋值运算符
RegInfo RegInfo::operator=(const RegInfo &Reg)
{
    name=Reg.name;
    username=Reg.username;
    pass=Reg.pass;
    phone=Reg.phone;
    headpath=Reg.headpath;
    return *this;
}
