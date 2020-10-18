#include "isadd.h"
#include "ui_isadd.h"
#include <QListWidgetItem>
#include "qq.h"
#include <QFile>
isadd::isadd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::isadd)
{
    ui->setupUi(this);
    pparent = qobject_cast<MainWindow *>(parent->parentWidget());
    //窗口图标和名称
    setWindowIcon(QIcon(":/new/prefix1/qq.jpg"));
    setWindowTitle("好友通知");
    selectname.clear();
}

isadd::~isadd()
{
    delete ui;
}
//显示好友申请
void isadd::disfriend(QString itemtext,QString headpath)
{
    select.insert(itemtext,headpath);  //键值对

    QString path;
    if(QFile::exists(QString(":/new/prefix1/%1").arg(headpath)))
        path = QString(":/new/prefix1/%1").arg(headpath);
    else
        path = QString("F:/QT/protect/day08/pic/%1").arg(headpath);
    QIcon icon(path);
    QListWidgetItem *newitem = new QListWidgetItem(itemtext);
    newitem->setIcon(icon);
    ui->listWidget->addItem(newitem);
}
//同意
void isadd::on_pushButton_clicked()
{
    if(pparent!=Q_NULLPTR)
    {
        if(!selectname.isEmpty())
        {
            pparent->mysorcket->write(QString("@#$%friendon::%1::%2").\
                                      arg(qobject_cast<QQ *>(parentWidget())->userinfo.username).\
                                      arg(selectname).toUtf8());
            qobject_cast<QQ *>(parentWidget())->addfriends(selectname,select.value(selectname));
            ui->listWidget->takeItem(row);
            if(ui->listWidget->count()==0)
            {
                qobject_cast<QQ *>(parentWidget())->addflag=0;
                pparent->friendadd.clear();
                this->close();
            }
            selectname.clear();
        }
    }
}
//拒绝
void isadd::on_pushButton_2_clicked()
{
    if(pparent!=Q_NULLPTR)
    {
        if(!selectname.isEmpty())
        {
            pparent->mysorcket->write(QString("@#$%friendoff::%1::%2").\
                                  arg(qobject_cast<QQ *>(parentWidget())->userinfo.username).\
                                  arg(selectname).toUtf8());
            ui->listWidget->takeItem(row);
            if(ui->listWidget->count()==0)
            {
                qobject_cast<QQ *>(parentWidget())->addflag=0;
                pparent->friendadd.clear();
                this->close();
            }
            selectname.clear();
        }
        else
            this->hide();
    }
}
//选择好友申请列表的好友
void isadd::on_listWidget_itemClicked(QListWidgetItem *item)
{
    selectname = item->text();
    if(!selectname.isEmpty())
    {
        ui->pushButton_2->setText("拒绝");
        row=ui->listWidget->row(item);
    }
    else
        ui->pushButton_2->setText("取消");

}
