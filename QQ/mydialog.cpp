#include "mydialog.h"
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <QTextBrowser>
#include <QTime>
#include "mainwindow.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
int mydialog::childflag=0;   //默认不需要窗子窗口
mydialog::mydialog(QWidget *parent) : QDialog(parent)
{
    flag=1;    //默认注册账号用户不存在
    if(childflag==0)
    {
        parentwindow=qobject_cast<MainWindow *>(parent);
        //发送获取已注册账号的信息标志
        parentwindow->mysorcket->write("@#$%getreg");
    }
}

mydialog::~mydialog()
{
    childflag=0;  //窗口退出时置0
}
//获取随机的字符串
QString setRandString()
{
    int max = 10;
    QString tmp("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    QString randString;
    QTime t;
    t = QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    for(int i=0;i<max;i++)
    {
        int ir = qrand()%tmp.length();
        randString[i] = tmp.at(ir);
    }
    return randString;
}
//新注册并保存注册信息
void mydialog::regfinished()
{
    if(flag==1)
    {
        if(!usered.text().isEmpty()&&!passed.text().isEmpty()&&!phoneed.text().isEmpty())
        {
            if(usered.text().toLongLong()!=0&&phoneed.text().toLongLong()!=0)
            {

                //获取ip
                QString writeinfo = QString("@#$%reg::%1::%2::%3::%4::10.jpg").\
                                    arg(setRandString()).arg(usered.text()).  \
                                    arg(passed.text()).arg(phoneed.text());

                parentwindow->mysorcket->write(writeinfo.toUtf8());
                childflag=0;  //窗口退出时置0
                this->close();
            }
            else
                QMessageBox::warning(this,"注册失败","用户名或者手机号码只能是数字！");
        }
        else
            QMessageBox::warning(this,"注册失败","信息填写不全！");

    }
    else if(flag==0)
    {
        QMessageBox::warning(this,"注册失败","用户已存在！");
        childflag=0;  //窗口退出时置0
        this->close();
    }
}
//关闭
void mydialog::cancelreg()
{
    childflag=0;  //窗口退出时置0
    this->close();
}
//找回密码
void mydialog::findpass()
{
    childflag=1;
    if(parentwindow->reginfo->isEmpty())
    {
        qDebug()<<"暂无注册信息";
        return ;
    }
    int mark=0;
    //查找要找回密码的用户
    for(int i=0;i<parentwindow->reginfo->size();i++)
    {
        if(parentwindow->reginfo->at(i).username == usered.text() && parentwindow->reginfo->at(i).phone == phoneed.text())
        {
            mark=1;
            //打开找回密码对话框
            succdialog = new mydialog(this);
            succdialog->setStyleSheet("QDialog{border-image: url(:/new/prefix1/经典.jpg);}");
            succdialog->resize(300,200);
            //窗口图标和名称
            succdialog->setWindowIcon(QIcon(":/new/prefix1/qq.jpg"));
            succdialog->setWindowTitle("填写新密码");

            QLabel userlb("  新密码:",succdialog);
            QLabel passlb("确认密码:",succdialog);
            succdialog->usered.setParent(succdialog);
            succdialog->passed.setParent(succdialog);
            succdialog->usered.setMaxLength(16);
            succdialog->passed.setMaxLength(16);
            QHBoxLayout *hlay=new QHBoxLayout;
            QHBoxLayout *hlay2=new QHBoxLayout;
            QGridLayout *glay=new QGridLayout(succdialog);
            hlay->addWidget(&userlb);hlay->addWidget(&succdialog->usered);
            hlay2->addWidget(&passlb);hlay2->addWidget(&succdialog->passed);
            glay->addLayout(hlay,0,0,Qt::AlignHCenter);
            glay->addLayout(hlay2,1,0,Qt::AlignHCenter);
            QPushButton bt("OK",succdialog);
            QPushButton newcancel("取消",succdialog);
            glay->addWidget(&bt,2,0,Qt::AlignLeft);
            glay->addWidget(&newcancel,2,0,Qt::AlignRight);

            connect(&bt,SIGNAL(clicked()),this,SLOT(findsucc()));
            connect(&newcancel,SIGNAL(clicked()),this,SLOT(closesucc()));
            succdialog->exec();
        }
    }
    if(mark==0)
    {
        //打开找回密码对话框
        errdialog = new mydialog(this);
        errdialog->setStyleSheet("QDialog{border-image: url(:/new/prefix1/经典.jpg);}");
        errdialog->resize(150,100);
        //窗口图标和名称
        errdialog->setWindowIcon(QIcon(":/new/prefix1/qq.jpg"));
        errdialog->setWindowTitle("找回密码");
        QTextBrowser tb(errdialog);
        tb.setGeometry(10,10,130,50);
        tb.setText("用户名或者手机号码输入错误！");
        tb.setAlignment(Qt::AlignHCenter);

        QPushButton bt("OK",errdialog);
        bt.setGeometry(90,70,50,20);
        connect(&bt,SIGNAL(clicked()),this,SLOT(closeerr()));
        errdialog->exec();
    }
}
//关闭窗口
void mydialog::closeerr()
{
    childflag=0;  //窗口退出时置0
    errdialog->close();
}
//关闭窗口
void mydialog::closesucc()
{
    childflag=0;  //窗口退出时置0
    succdialog->close();
    this->close();
}
//找回密码成功
void mydialog::findsucc()
{
    if(succdialog->usered.text()!=succdialog->passed.text())
        succdialog->passflag=0;
    else
        succdialog->passflag=1;

    if(succdialog->passflag==1)
    {
        QString sendbuf = QString("@#$%findsucceed::%1::%2::%3"). \
                        arg(usered.text()).arg(succdialog->passed.text()).\
                        arg(phoneed.text());
        parentwindow->mysorcket->write(sendbuf.toUtf8());
        childflag=0;  //窗口退出时置0
        succdialog->close();
        this->close();
    }
    else
        QMessageBox::warning(this,"输入错误","重新输入密码与新密码不一致！");
}
//判断注册的账号是否存在
void mydialog::isUseable(const QString &arg1)
{
    for(int i=0;i<parentwindow->reginfo->size();i++)
    {
        if(parentwindow->reginfo->at(i).username == arg1)
        {
            usered.setStyleSheet("color:rgb(255,0,0);");
            flag=0;   //用户已存在
            break;
        }
        else
        {
            flag=1;   //用户不存在
            usered.setStyleSheet("color:rgb(0,0,0);");
        }
    }

}
