#include "chat.h"
#include "ui_chat.h"
#include "mainwindow.h"
#include <QDebug>
#include "qq.h"
#include <QFileDialog>
chat::chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chat)
{
    ui->setupUi(this);
    qobject_cast<MainWindow *>(parentWidget()->parentWidget())->key=0;
    emojiflag=0;
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(QStringLiteral("scrollArea"));
    scrollArea->setGeometry(QRect(0, 30, 430, 200));
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 428, 198));
    y=0;
    scrollArea->setStyleSheet("background-color:rgb(255,255,255,10);");

}

chat::~chat()
{
    delete ui;
    delete scrollArea;
    delete scrollAreaWidgetContents;
}
//获取聊天对象账号
void chat::get(QString chatname, QString chatnum, QString headpath)
{
    this->chatname = chatname;
    this->chatnum = chatnum;
    this->headpath = headpath;
    ui->label->setText(chatname);
    ui->label->setAlignment(Qt::AlignCenter);
}
//接收信息
void chat::gettext(QString text,QString text1)
{
    //先删除组件后设置组件
    scrollArea->takeWidget();
    receivewidget = new receive(scrollAreaWidgetContents);
    receivewidget->setObjectName(QStringLiteral("widget"));
    receivewidget->setGeometry(QRect(5, y, 420, receivewidget->hight));
    receivewidget->getStr(text);
    receivewidget->getHtml(text1);
    receivewidget->set(chatname,headpath);

    y+=receivewidget->hight;
    if(y>198)
    {
        scrollAreaWidgetContents->setMinimumSize(428,y);
    }
    scrollArea->setWidget(scrollAreaWidgetContents);
    //将滚动条自动下拉到底
    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());

}
//输入表情包
void chat::set(QString text)
{
    ui->textEdit->insertHtml(text);
}

//发送信息
void chat::on_pushButton_clicked()
{
    //发送信息
    QString sendstr = QString("@#$%onebyone::%1::%2::%3::%4").arg(chatnum).arg(qobject_cast<QQ *>(parentWidget())->userinfo.username).arg(ui->textEdit->toPlainText()).arg(ui->textEdit->toHtml());
    qobject_cast<MainWindow *>(parentWidget()->parentWidget())->mysorcket->write(sendstr.toUtf8());

    //先删除组件后设置组件
    scrollArea->takeWidget();

    sendwidget = new send(scrollAreaWidgetContents);
    sendwidget->setObjectName(QStringLiteral("widget"));
    sendwidget->setGeometry(QRect(5, y, 420, sendwidget->hight));

    sendwidget->getStr(ui->textEdit->toPlainText());
    sendwidget->getHtml(ui->textEdit->toHtml());

    sendwidget->set(qobject_cast<QQ *>(parentWidget())->userinfo.name,qobject_cast<QQ *>(parentWidget())->userinfo.headpath);

    ui->textEdit->clear();
    y+=sendwidget->hight;

    if(y>198)
    {
        scrollAreaWidgetContents->setMinimumSize(428,y);
    }
    scrollArea->setWidget(scrollAreaWidgetContents);
    //将滚动条自动下拉到底
    scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->maximum());

}
//表情包
void chat::on_pushButton_3_clicked()
{ 
    //打开表情包
    if(emojiflag==0)
    {
        emojiwidget = new emoji(this);
        emojiwidget->setObjectName(QStringLiteral("widget"));
        emojiwidget->setGeometry(QRect(0, 80, 340, 150));
        emojiwidget->setMinimumSize(QSize(340, 150));
        emojiwidget->setMaximumSize(QSize(340, 150));
        emojiwidget->raise();  //置顶
        emojiwidget->show();
        emojiflag=1;
    }
    else //关闭表情包
    {
        delete emojiwidget;
        emojiflag=0;
    }
}
//个性装扮
void chat::on_pushButton_2_clicked()
{
    //打开文件对话框
    QString filename = QFileDialog::getOpenFileName(this,"自定义装扮");
    if(!filename.isEmpty())
    {
        //设置背景
        QString background = QString("border-image:url(%1);").arg(filename);
        ui->textBrowser->setStyleSheet(background);
    }
}
//关闭
void chat::on_pushButton_4_clicked()
{
    this->hide();
}
//设置背景透明度
void chat::on_horizontalSlider_valueChanged(int value)
{
    QString temp = QString("background-color:rgb(255,255,255,%1);").arg(value);
    scrollArea->setStyleSheet(temp);
}
//软件盘
void chat::on_pushButton_5_clicked()
{
    if(qobject_cast<MainWindow *>(parentWidget()->parentWidget())->key==0)
    {
        mykeyboard = new keyboard(qobject_cast<QQ *>(parentWidget()));
        mykeyboard->setGeometry(QRect(130, 130, 550, 140));
        mykeyboard->raise();
        mykeyboard->show();
        qobject_cast<MainWindow *>(parentWidget()->parentWidget())->key=1;
    }
    else
    {
        mykeyboard->close();
        qobject_cast<MainWindow *>(parentWidget()->parentWidget())->key=0;
    }
}
