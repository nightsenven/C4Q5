#include "mypushbutton.h"
#include <QDebug>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QApplication>
#include "keyboard.h"
#include "keyboradl.h"
#include "mainwindow.h"
#include "qq.h"
#include "chat.h"
#include "addfriend.h"
mypushbutton::mypushbutton(QWidget *parent) : QPushButton(parent)
{
    flag=0;
    //关联信号与槽函数
    connect(this,SIGNAL(clicked()),this,SLOT(pressdown()));
}

//键盘按下
void mypushbutton::pressdown()
{
    //创建键盘事件
    QKeyEvent *keyevent;
    //获取按键按下的按键按钮
    mypushbutton *bt = qobject_cast<mypushbutton *>(sender());

    if(bt->text()=="Ent")    //回车键
    {
        keyevent=new QKeyEvent(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier,bt->text());
    }
    else if(bt->text()=="Cap")  //大小写
    {
       if(flag==0) //大写
       {
           //把按钮变个颜色
           bt->setStyleSheet("background-color:rgb(255,255,255)");
           flag=1;
       }
       else     //小写
       {
           //把按钮变个颜色
           bt->setStyleSheet("background-color:rgb(200,200,200)");
           flag=0;
       }
        keyevent=new QKeyEvent(QEvent::KeyPress,Qt::Key_CapsLock,Qt::NoModifier,bt->text());
    }
    else if(bt->text()=="<-")  //返回（关闭）
    {
        keyevent=new QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier,"");

        if(qobject_cast<MainWindow *>(parentWidget()->parentWidget()->parentWidget())!=Q_NULLPTR)
        {
            qobject_cast<MainWindow *>(parentWidget()->parentWidget()->parentWidget())->key=0;
            qobject_cast<keyboard *>(parentWidget())->close();
        }
        else if(qobject_cast<addfriend *>(parentWidget()->parentWidget())->myparent!=Q_NULLPTR)
        {
            qobject_cast<addfriend *>(parentWidget()->parentWidget())->set(0);
            qobject_cast<keyboradl *>(parentWidget())->close();
        }
        else if(qobject_cast<MainWindow *>(parentWidget()->parentWidget())!=Q_NULLPTR)
        {
            qobject_cast<MainWindow *>(parentWidget()->parentWidget())->key=0;
            qobject_cast<keyboard *>(parentWidget())->close();
        }


    }
    else if(bt->text()=="Backspace")  //删除 //退格
    {
        keyevent=new QKeyEvent(QEvent::KeyPress,Qt::Key_Backspace,Qt::NoModifier,bt->text());
    }
    else if(bt->text()=="Space")  //空格
    {
        keyevent=new QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier," ");
    }
    else
    {
        int key=bt->text().at(0).toLatin1();
        if(flag==0&&key>=65&&key<=90)  //小写
            keyevent = new QKeyEvent(QEvent::KeyPress,key,Qt::NoModifier, \
                                       QString(bt->text().at(0).toLatin1()+32));
        else
            keyevent = new QKeyEvent(QEvent::KeyPress,key,Qt::NoModifier,bt->text());

    }
    //发送该事件给输入框(本质上把事件发送给鼠标光标进入的那个组件)
    QCoreApplication::postEvent(QApplication::focusWidget(),keyevent);
}
