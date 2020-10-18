#include "rightmenu.h"
#include "ui_rightmenu.h"
#include "qq.h"
#include "personally.h"
rightmenu::rightmenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rightmenu)
{
    ui->setupUi(this);
}

rightmenu::~rightmenu()
{
    delete ui;
}

void rightmenu::get(QString usernumber)
{
    usernum = usernumber;
}

//退出程序
void rightmenu::on_pushButton_3_clicked()
{
    MainWindow *w = qobject_cast<MainWindow *>(parentWidget()->parentWidget());
    QString temp = QString("@#$%logoff::%1").arg(usernum);
    w->mysorcket->write(temp.toUtf8());

    qobject_cast<QQ *>(parentWidget())->close();

    w->close();
    this->close();
}
//注销
void rightmenu::on_pushButton_2_clicked()
{

    MainWindow *w = qobject_cast<MainWindow *>(parentWidget()->parentWidget());
    QString temp = QString("@#$%logoff::%1").arg(usernum);
    w->mysorcket->write(temp.toUtf8());

    w->show();
    qobject_cast<QQ *>(parentWidget())->close();
    this->close();
}
//取消
void rightmenu::on_pushButton_4_clicked()
{
    this->close();
}
//个人信息
void rightmenu::on_pushButton_clicked()
{
    personally *p = new personally(qobject_cast<QQ *>(parentWidget()));

    this->close();
    p->exec();
}
