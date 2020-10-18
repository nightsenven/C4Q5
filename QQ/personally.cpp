#include "personally.h"
#include "ui_personally.h"
#include "mainwindow.h"
#include "qq.h"
#include "rightmenu.h"
#include <QFileDialog>
personally::personally(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::personally)
{
    ui->setupUi(this);
    //ui->lineEdit_4->setEchoMode(QLineEdit::Password); //隐藏密码
    ui->lineEdit->setText(qobject_cast<QQ *>(parent)->userinfo.username);
    ui->lineEdit_2->setText(qobject_cast<QQ *>(parent)->userinfo.name);
    ui->lineEdit_3->setText(qobject_cast<QQ *>(parent)->userinfo.phone);
    ui->lineEdit_4->setText(qobject_cast<QQ *>(parent)->userinfo.pass);
    if(QFile::exists(QString(":/new/prefix1/%1").arg(qobject_cast<QQ *>(parent)->userinfo.headpath)))
        ui->pushButton->setStyleSheet(QString("border-radius:20px;border:1px solid#000000;border-image: url(:/new/prefix1/%1);").\
                                      arg(qobject_cast<QQ *>(parent)->userinfo.headpath));
    else
        ui->pushButton->setStyleSheet(QString("border-radius:20px;border:1px solid#000000;border-image:url(F:/QT/protect/day08/pic/%1);").\
                                  arg(qobject_cast<QQ *>(parent)->userinfo.headpath));
    set();
}

personally::~personally()
{
    delete ui;
}

void personally::set()
{
    //个人账户和手机号码
    usernum = ui->lineEdit->text();
    userphone = ui->lineEdit_3->text();
}
//修改账户   flag=1为密码，flag=2为头像,0 修改失败
void personally::change(int flag,QString usernum,QString changetext,QString path)
{
    changebuf.clear();
    if(flag==0)
        return ;
    //先读取
    QFile readfile(path);
    if(readfile.open(QIODevice::ReadWrite))
    {
        char buf[100]={0};
        while(!readfile.atEnd())
        {
            memset(buf,0,100);
            readfile.readLine(buf,100);
            changebuf.append(QString(buf).remove("\n"));
        }
    }
    readfile.close();
    //查找
    for(int i=0;i<changebuf.size();i++)
    {
        QStringList temp = changebuf.at(i).split("::");
        if(temp.size()>=3)
        {
            if(usernum==temp.at(0))
            {
                temp.replace(flag,changetext);
                changebuf.replace(i,QString("%1::%2::%3").arg(temp.at(0)).arg(temp.at(1)).arg(temp.at(2)));
                break;
            }
        }
        else if(temp.size()==2)
        {
            if(usernum==temp.at(0))
            {
                temp.replace(flag-1,changetext);
                changebuf.replace(i,QString("%1::%2").arg(temp.at(0)).arg(temp.at(1)));
                break;
            }
        }

    }
    //重新写入
    QFile writefile(path);
    if(writefile.open(QIODevice::ReadWrite|QIODevice::Truncate))
    {
        for(auto& i:changebuf)
        {
            QString writestr = QString("%1\n").arg(i);
            writefile.write(writestr.toUtf8());
        }
    }
    writefile.close();
}


//修改昵称
void personally::on_pushButton_2_clicked()
{
    QString sendstr = QString("@#$%name::%1::%2").arg(usernum).arg(ui->lineEdit_2->text());
    qobject_cast<MainWindow *>(parentWidget()->parentWidget())->mysorcket->write(sendstr.toUtf8());
    qobject_cast<QQ *>(parentWidget())->setname(ui->lineEdit_2->text());

    qobject_cast<QQ *>(parentWidget())->show();
    this->close();
}
//修改手机号码
void personally::on_pushButton_3_clicked()
{
    QString sendstr = QString("@#$%phone::%1::%2::%3").arg(usernum).arg(userphone).arg(ui->lineEdit_3->text());
    qobject_cast<MainWindow *>(parentWidget()->parentWidget()->parentWidget())->mysorcket->write(sendstr.toUtf8());
    qobject_cast<QQ *>(parentWidget())->userinfo.setPhone(ui->lineEdit_3->text());
    qobject_cast<QQ *>(parentWidget())->show();

    this->close();
}
//修改密码
void personally::on_pushButton_4_clicked()
{
    QString sendstr = QString("@#$%pass::%1::%2::%3").arg(usernum).arg(userphone).arg(ui->lineEdit_4->text());
    qobject_cast<MainWindow *>(parentWidget()->parentWidget()->parentWidget())->mysorcket->write(sendstr.toUtf8());
    qobject_cast<QQ *>(parentWidget())->userinfo.setPass(ui->lineEdit_4->text());
    qobject_cast<QQ *>(parentWidget())->show();
    change(1,usernum,ui->lineEdit_4->text(),"F:/QT/protect/day08/TXT/automaticname.txt");
    change(1,usernum,ui->lineEdit_4->text(),"F:/QT/protect/day08/TXT/remembername.txt");
    this->close();
}
//修改头像
void personally::on_pushButton_clicked()
{
    //打开文件对话框
    QString filename = QFileDialog::getOpenFileName(this,"选择头像");
    QStringList pathlist;
    if(!filename.isEmpty())
    {
        QString readimagebuf = readimage(filename);
        pathlist = filename.split("/");
        QString sendstr = QString("@#$%path::%1::%2").arg(usernum).arg(pathlist.at(pathlist.size()-1));
        qobject_cast<MainWindow *>(parentWidget()->parentWidget())->mysorcket->write(sendstr.toUtf8());
        qobject_cast<QQ *>(parentWidget())->setpath(pathlist.at(pathlist.size()-1),readimagebuf);
    }
    qobject_cast<QQ *>(parentWidget())->show();
    change(2,usernum,pathlist.at(pathlist.size()-1),"F:/QT/protect/day08/TXT/automaticname.txt");
    change(2,usernum,pathlist.at(pathlist.size()-1),"F:/QT/protect/day08/TXT/remembername.txt");
    change(2,usernum,pathlist.at(pathlist.size()-1),"F:/QT/protect/day08/TXT/loginname.txt");
    this->close();
}
//读取图片数据并发送
QString personally::readimage(QString path)
{
    QFile image(path);
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

//关闭
void personally::on_pushButton_5_clicked()
{
    this->close();
}

