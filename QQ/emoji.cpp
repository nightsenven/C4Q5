#include "emoji.h"
#include "ui_emoji.h"
#include <QDir>
#include "chat.h"
#include <QDebug>
emoji::emoji(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::emoji)
{
    ui->setupUi(this);
    getbt();
    emojinum=0;
    count=0;
    emojiname.clear();
    //获取全部表情包
    QDir emojidir("F:/QT/protect/day08/QQ/emoji");
    QStringList mylist = emojidir.entryList();
    for(auto& i:mylist)
    {
        if(i.indexOf(".jpg")!=-1)
            emojiname.append(i);
        else if(i.indexOf(".png")!=-1)
            emojiname.append(i);
        else if(i.indexOf(".gif")!=-1)
            emojiname.append(i);
    }
    emojicount = disemoji(emojinum);
    //关联信号与槽函数
    for(int i=1;i<allbt.size();i++)
        connect(allbt.at(i),SIGNAL(clicked()),this,SLOT(on_pushButton_1_clicked()));
    setStyleSheet("QMainWindow{border-image: url(:/new/prefix1/headpic/白图.jpg);}");
}

emoji::~emoji()
{
    delete ui;
}
//加载表情包
int emoji::disemoji(int &num)
{
    QString emojipath;
    int i=0;
    for(;i<allbt.size();i++)
    {
        if(num==emojiname.size())
            break;
        emojipath = QString("border-radius:15px;border:1px solid#000000;border-image: url(:/new/prefix1/%1);color: rgb(0, 0, 0,0);").arg(emojiname.at(num++));
        allbt.at(i)->setText(QString::number(i));
        allbt.at(i)->setStyleSheet(emojipath);
    }
    for(int j=i;j<allbt.size();j++)
    {
        allbt.at(j)->setStyleSheet("background-color: rgb(0, 0, 0,0);color: rgb(0, 0, 0,0);");
    }

    return i;
}
//获取保存ui按钮
void emoji::getbt()
{
    allbt.append(ui->pushButton_1);
    allbt.append(ui->pushButton_2);
    allbt.append(ui->pushButton_3);
    allbt.append(ui->pushButton_4);
    allbt.append(ui->pushButton_5);
    allbt.append(ui->pushButton_6);
    allbt.append(ui->pushButton_7);
    allbt.append(ui->pushButton_8);
    allbt.append(ui->pushButton_9);
    allbt.append(ui->pushButton_10);
    allbt.append(ui->pushButton_11);
    allbt.append(ui->pushButton_12);
    allbt.append(ui->pushButton_13);
    allbt.append(ui->pushButton_14);
    allbt.append(ui->pushButton_15);
    allbt.append(ui->pushButton_16);
    allbt.append(ui->pushButton_17);
    allbt.append(ui->pushButton_18);
    allbt.append(ui->pushButton_19);
    allbt.append(ui->pushButton_20);
    allbt.append(ui->pushButton_21);
    allbt.append(ui->pushButton_22);
    allbt.append(ui->pushButton_23);
    allbt.append(ui->pushButton_24);
    allbt.append(ui->pushButton_25);
    allbt.append(ui->pushButton_26);
    allbt.append(ui->pushButton_27);
    allbt.append(ui->pushButton_28);
    allbt.append(ui->pushButton_29);
    allbt.append(ui->pushButton_30);
    allbt.append(ui->pushButton_31);
    allbt.append(ui->pushButton_32);
    allbt.append(ui->pushButton_33);
    allbt.append(ui->pushButton_34);
    allbt.append(ui->pushButton_35);
    allbt.append(ui->pushButton_36);
    allbt.append(ui->pushButton_37);
    allbt.append(ui->pushButton_38);
    allbt.append(ui->pushButton_39);
    allbt.append(ui->pushButton_40);
}
//将表情包路径转换为Html格式
void emoji::imgPathToHtml(QString &path)
{
    path = QString("<img src=\"%1\">").arg(path);
}
//上一页
void emoji::on_pushButton_clicked()
{
    emojinum-=(emojicount+40);
    if(emojinum<0)
        return ;
    emojicount = disemoji(emojinum);
    count--;
    if(count<0)
        count=0;
}
//下一页
void emoji::on_pushButton1_clicked()
{
    emojicount = disemoji(emojinum);
    count++;
}
//点击选择表情包
void emoji::on_pushButton_1_clicked()
{
    QPushButton *bt = qobject_cast<QPushButton *>(sender());
    QString emojipath = QString(":/new/prefix1/%1").arg(emojiname.at(bt->text().toInt()+count*40));
    imgPathToHtml(emojipath);
    qobject_cast<chat *>(parentWidget())->set(emojipath);

    qobject_cast<chat *>(parentWidget())->emojiflag=0;
    this->close();
}
