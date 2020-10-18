#include "addfriend.h"
#include "ui_addfriend.h"
#include <QTest>
#include <QListWidgetItem>
#include "qq.h"
#include <QDebug>

addfriend::addfriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addfriend)
{
    ui->setupUi(this);
    myparent = qobject_cast<MainWindow *>(parentWidget()->parentWidget());
    listwidgetlist = new QList<QListWidget *>;
    //窗口图标和名称
    setWindowIcon(QIcon(":/new/prefix1/qq.jpg"));
    setWindowTitle("添加好友");
    connect(&mytimer,SIGNAL(timeout()),this,SLOT(display()));
    connect(ui->pushButton_5,SIGNAL(clicked(bool)),this,SLOT(openkeyborad()));
}

addfriend::~addfriend()
{
    delete ui;
    delete listwidgetlist;
}
//设置开关
void addfriend::set(int key)
{
    myparent->key=key;
}

//搜索好友
void addfriend::on_pushButton_clicked()
{
    QString send = QString("@#$%friend::%1").arg(ui->lineEdit->text());
    myparent->mysorcket->write(send.toUtf8());
    mytimer.start(500); //定时一秒
}

//单击到了某一项
void addfriend::on_listWidget_itemClicked(QListWidgetItem *item)
{
    friendname = item->text();
}
//关闭
void addfriend::on_pushButton_3_clicked()
{
    myparent->friendinfo.clear();
    this->close();
}
//添加好友
void addfriend::on_pushButton_2_clicked()
{
    myparent->mysorcket->write(QString("@#$%friendadd::%1::%2").arg(qobject_cast<QQ *>(parentWidget())->userinfo.username).\
                               arg(ui->lineEdit->text()).toUtf8());
    this->close();
}
//显示搜索到的好友
void addfriend::display()
{
    mytimer.stop();
    if(myparent->friendaddflag==1)//找到好友了
    {
        ui->label->setText("搜索到的好友信息为：");
        QListWidget *newlistwidget = new QListWidget(this);
        newlistwidget->setGeometry(20,70,360,160);
        QString path;
        if(QFile::exists(QString(":/new/prefix1/%1").arg(myparent->friendinfo.headpath)))
            path = QString(":/new/prefix1/%1").arg(myparent->friendinfo.headpath);
        else
            path = QString("F:/QT/protect/day08/pic/%1").arg(myparent->friendinfo.headpath);
        QIcon icon(path);
        QString friendinfo = QString("%1(%2)").arg(myparent->friendinfo.name).arg(myparent->friendinfo.username);
        QListWidgetItem *newitem = new QListWidgetItem(friendinfo);
        newitem->setIcon(icon);
        newlistwidget->addItem(newitem);
        newlistwidget->raise();//置顶
        newlistwidget->show();
        listwidgetlist->append(newlistwidget);
        connect(newlistwidget,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(on_listWidget_itemClicked(QListWidgetItem *)));
    }
    else if(myparent->friendaddflag==0)  //没有找到好友
    {
        ui->label->setText("没有搜索到该好友");
        QListWidget *newlistwidget = new QListWidget(this);
        newlistwidget->setGeometry(20,70,360,160);
        newlistwidget->raise();//置顶
        newlistwidget->show();
        listwidgetlist->append(newlistwidget);
    }
}
//软键盘
void addfriend::openkeyborad()
{
    if(myparent->key==0)
    {
        mykey = new keyboradl(this);
        mykey->setGeometry(QRect(0, 70, 400, 140));
        mykey->raise();
        mykey->show();
        myparent->key=1;
    }
    else
    {
        mykey->close();
        myparent->key=0;
    }
}
