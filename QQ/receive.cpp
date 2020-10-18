#include "receive.h"
#include "ui_receive.h"
#include <QDebug>
#include <QFont>
#include <QTextCursor>
#include <QDir>
receive::receive(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::receive)
{
    ui->setupUi(this);
    hight=50;
    browserhight=20;
    browserwight=20;
    count=0;
    linecount=0;
    flag=0;
    //获取全部表情包
    QDir emojidir("F:/QT/protect/day08/QQ/emoji/");
    QStringList mylist = emojidir.entryList();
    for(auto& i:mylist)
    {
        if(i.indexOf(".jpg")!=-1)
        {
            emojiname.append(i);
        }
    }
}

receive::~receive()
{
    delete ui;
}
//获取信息显示到textbrowser
void receive::getHtml(QString text)
{
    checkHtml(text);
    ui->textBrowser->setAlignment(Qt::AlignLeft);
    ui->textBrowser->setHtml(text);
}
void receive::getStr(QString text)
{
    linecount = text.count("\n");
    endlist = text.split("\n");
    if(endlist.isEmpty())
        endlist.append(text);
}
//检查html有多少张表情
void receive::checkHtml(QString text)
{
    flag=0;
    QStringList htmlendlist = text.split("</p>\n");
    if(htmlendlist.isEmpty())
        htmlendlist.append(text);

    for(int i=0;i<htmlendlist.size();i++)
    {
        int indeximg1=0,indeximg2=0;
        int width=0;
        while(1)
        {
            indeximg1 = htmlendlist.at(i).indexOf(QRegExp("<img src=\".\{1,}\" />"),indeximg2);
            if(indeximg1!=-1)
            {
                flag=1;
                width+=56;
                indeximg2 = htmlendlist.at(i).indexOf('>',indeximg1);
                if(indeximg2!=-1)
                {
                    indeximg2++;
                }
            }
            else
            {
                endwidth.append(width+QFontMetrics(ui->myfont).width(endlist.at(i)));
                break;
            }
        }
    }
}

//设置昵称和头像
void receive::set(QString name, QString headpath)
{
    ui->label->setText(name);
    ui->label->setAlignment(Qt::AlignLeft);//左对齐
    QString headimage;
    if(QFile::exists(QString(":/new/prefix1/%1").arg(headpath)))
        headimage = QString("border-image: url(:/new/prefix1/%1);border-radius:20px;border:1px solid#000000;").arg(headpath);

    else if(QFile::exists(QString("F:/QT/protect/day08/pic/%1").arg(headpath)))
        headimage = QString("border-image: url(F:/QT/protect/day08/pic/%1);border-radius:20px;border:1px solid#000000;").arg(headpath);

    ui->pushButton->setStyleSheet(headimage);
}

//文本内容改变改变发生的信号
void receive::on_textBrowser_textChanged()
{
    if(!ui->textBrowser->toPlainText().isEmpty())
    {
        if(!endwidth.isEmpty())
        {
            count=linecount;
            //获取最长的宽度
            int maxwidth=endwidth.at(0);
            for(auto &i:endwidth)
            {
                if(maxwidth<i)
                    maxwidth=i;
            }
            if(maxwidth>370&&linecount==0)
            {
                maxwidth=370;
                count++;
            }
            if(flag==1)
            {
                browserhight=56;
                browserhight+=56*count;
                hight=90;
                hight+=60*count;
            }
            else
            {
                browserhight+=20*count;
                hight+=20*count;
            }
            resize(420,hight);
            ui->textBrowser->setGeometry(40,20,maxwidth+15,browserhight);
            linecount=0;
        }
    }
}

