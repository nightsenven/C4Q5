#include "mainwindow.h"
#include "mydialog.h"
#include "chat.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QMessageBox>
#include <QIcon>
#include <QHostAddress>
#include "qq.h"
#include <QTest>
#include <QTime>
QQ *loginuser;   //全局变量，，聊天界面


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    friendaddflag=0;
    key=0;
    mysorcket=new QTcpSocket;
    randcount=0;
    connect(&randtimer,SIGNAL(timeout()),this,SLOT(getrand()));
    randtimer.start(1);  //定时1ms
    reginfo = new QList<RegInfo>;
    loginlist = getlogin();   //获取本地登录过的账号
    loginflag=0;    //没有在本地登录过
    connectflag=0;   //默认没有连接
    automaticflag=0;
    friendflag = 0;

    //设置Combobox下拉框的最大输入长度
    ui->comboBox->lineEdit()->setMaxLength(11);

    //关联信号与槽函数
    connect(mysorcket,SIGNAL(readyRead()),this,SLOT(toreadcliet()));//判断是否可读
    connect(mysorcket,SIGNAL(connected()),this,SLOT(hasconnected()));//判断是否连接成功
    connect(mysorcket,SIGNAL(disconnected()),this,SLOT(quitConnection()));//判断服务器是否断开
    //窗口图标和名称
    setWindowIcon(QIcon(":/new/prefix1/qq.jpg"));
    setWindowTitle("QQ登录");
    //隐藏密码图标
    QIcon hideicon(":/new/prefix1/hide.jpg");
    ui->hide->setIcon(hideicon);

    //登录用户的头像
    path="/new/prefix1/10.jpg";
    headimage =QString("border-image: url(:%1);border-radius:45px;border:1px solid#000000;").arg(path);
    ui->pushButton->setStyleSheet(headimage);
    automatic=0;    //非
    remember=0;     //非
    flag=0;         //隐藏密码
    ui->lineEdit_2->setEchoMode(QLineEdit::Password); //隐藏密码
    ui->automatic_login->setChecked(false);
    ui->rememberpass->setChecked(false);
   // ui->radioButton_3->setChecked(true);
    this->setStyleSheet("QMainWindow{border-image: url(:/new/prefix1/经典.jpg);}");
    QString numname = get();    //获取最后一个本机登录的记住密码或者自动登录的用户
    //显示本地登陆过账号
    QStringList templist;

    if(!loginlist->isEmpty())
    {
        auto i=loginlist->cend();
        i--;
        for(;;i--)
        {
            templist.append(i.key());
            if(i==loginlist->cbegin())
                break;
        }
        path.clear();

        //添加combobox的列表项
        ui->comboBox->addItems(templist);
        if(!numname.isEmpty())
        {
            ui->comboBox->setCurrentText(numname);
            //登录用户的头像
            path=loginlist->value(numname);
        }
        else
            path=loginlist->value(templist.at(0));

        path=path.remove("\n");  //删除"\n"

        if(QFile::exists(QString(":/new/prefix1/%1").arg(path)))
        {
            ui->pushButton->setStyleSheet(QString("border-image: url(:/new/prefix1/%1);border-radius:45px;border:1px solid#000000;").arg(path));
        }
        else
            ui->pushButton->setStyleSheet(QString("border-image: url(F:/QT/protect/day08/pic/%1);border-radius:45px;border:1px solid#000000;").arg(path));
    }

    //关联信号与槽函数
    connect(ui->comboBox,SIGNAL(clicked()),this,SLOT(showpopup()));
    connect(&mytimer,SIGNAL(timeout()),this,SLOT(login()));
    connect(&mytimer1,SIGNAL(timeout()),this,SLOT(set()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete reginfo;
    delete loginlist;
}
//登录是否成功
void MainWindow::login(bool is)
{
    if(is)
    {
        if(automaticflag==0)
        {
            save(ui->comboBox->currentText(),ui->lineEdit_2->text(),path);
            if(loginflag==0)
                savelogin(ui->comboBox->currentText(),path);
        }

        loginuser = new QQ(ui->comboBox->currentText(),this);
        //登录成功
        loginuser->show();
        mytimer1.start(200);
        this->hide();
    }
    else //登录失败
    {
        QMessageBox::warning(this,"警告","用户名或者密码错误！");
    }

}
//记住密码
void MainWindow::rememberpass(QString username, QString pass,QString headpath)
{
    path.clear();
    path = headpath;
    if(QFile::exists(QString(":/new/prefix1/%1").arg(path)))
    {
        ui->pushButton->setStyleSheet(QString("border-image: url(:/new/prefix1/%1);border-radius:45px;border:1px solid#000000;").arg(path));
    }
    else
        ui->pushButton->setStyleSheet(QString("border-image: url(F:/QT/protect/day08/pic/%1);border-radius:45px;border:1px solid#000000;").arg(path));

    ui->comboBox->setCurrentText(username);
    ui->lineEdit_2->setText(pass);
    ui->rememberpass->setChecked(true);
}
//自动登录
void MainWindow::automaticlogin(QString username,QString pass,QString headpath)
{
    path.clear();
    path = headpath;

    if(QFile::exists(QString(":/new/prefix1/%1").arg(path)))
    {
        ui->pushButton->setStyleSheet(QString("border-image: url(:/new/prefix1/%1);border-radius:45px;border:1px solid#000000;").arg(path));
    }
    else
        ui->pushButton->setStyleSheet(QString("border-image: url(F:/QT/protect/day08/pic/%1);border-radius:45px;border:1px solid#000000;").arg(path));
    ui->comboBox->setCurrentText(username);
    ui->lineEdit_2->setText(pass);
    ui->automatic_login->setChecked(true);
    ui->rememberpass->setChecked(true);


    mytimer.start(2000);
}
//保存记住密码的用户
void MainWindow::saveremembername(QString namenum, QString pass, QString headpath)
{
    QFile rememberread("F:/QT/protect/day08/TXT/remembername.txt");
    if(rememberread.open(QIODevice::ReadWrite))
    {
        char buf[100]={0};
        while(!rememberread.atEnd())
        {
            memset(buf,0,100);
            rememberread.readLine(buf,100);
            if(QString(buf).indexOf(namenum)!=-1)
            {
                return ;
            }
        }
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    rememberread.close();
    QFile rememberwrite("F:/QT/protect/day08/TXT/remembername.txt");
    if(rememberwrite.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        QString temp=QString("%1::%2::%3\n").arg(namenum).arg(pass).arg(headpath);
        rememberwrite.write(temp.toUtf8());
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    rememberwrite.close();
}
//保存自动登录的用户
void MainWindow::saveautomaticname(QString namenum,QString pass,QString headpath)
{
    QFile automaticread("F:/QT/protect/day08/TXT/automaticname.txt");
    if(automaticread.open(QIODevice::ReadWrite))
    {
        char buf[100]={0};
        while(!automaticread.atEnd())
        {
            memset(buf,0,100);
            automaticread.readLine(buf,100);
            if(QString(buf).indexOf(namenum)!=-1)
            {
                return ;
            }
        }
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    automaticread.close();

    QFile automaticwrite("F:/QT/protect/day08/TXT/automaticname.txt");
    if(automaticwrite.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        QString temp=QString("%1::%2::%3\n").arg(namenum).arg(pass).arg(headpath);
        automaticwrite.write(temp.toUtf8());
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    automaticwrite.close();
}
//获取自动登录或者记住密码的用户
QString MainWindow::get()
{
    char buf[100]={0};
    loginlistly.clear();
    QFile automaticname("F:/QT/protect/day08/TXT/automaticname.txt");
    if(automaticname.open(QIODevice::ReadWrite))
    {
        while(!automaticname.atEnd())
        {
            memset(buf,0,100);
            automaticname.readLine(buf,100);
            QString temp(buf);
            temp=temp.remove("\n");  //删除换行符"\r\n"
            QStringList templist = temp.split("::");
            if(!templist.isEmpty())
                loginlistly.append(templist);
        }
        if(!loginlistly.isEmpty())
        {
            //自动登录
            auto it = loginlistly.cend();
            it--;
            while(it->size()<3)
                it--;
            automaticlogin(it->at(0),it->at(1),it->at(2));
            automaticname.close();
            return it->at(0);
        }
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    automaticname.close();
    loginlistly.clear();
    QFile remembername("F:/QT/protect/day08/TXT/remembername.txt");
    if(remembername.open(QIODevice::ReadWrite))
    {
        while(!remembername.atEnd())
        {
            memset(buf,0,100);
            remembername.readLine(buf,100);
            QString temp(buf);
            temp=temp.remove("\n");  //删除换行符"\r\n"
            QStringList templist = temp.split("::");
            if(!templist.isEmpty())
                loginlistly.append(templist);

        }
        if(!loginlistly.isEmpty())
        {
            auto it = loginlistly.cend();
            it--;
            while(it->size()<3)
                it--;
            rememberpass(it->at(0),it->at(1),it->at(2));
            remembername.close();
            return it->at(0);
        }
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    remembername.close();
    return QString();
}
//聊天
void MainWindow::chatonebyone(QStringList readlist)
{
    chat *currentwidget = loginuser->chatobject->value(readlist.at(1));
    QString chatpath=loginuser->headimgmap.value(readlist.at(1));
    QStringList templist = readlist.at(1).split("(");
    templist = templist.at(1).split(")");
    if(currentwidget==Q_NULLPTR)
    {
        qDebug()<<"找不到键值";
        currentwidget=new chat(loginuser);
        loginuser->chatobject->insert(readlist.at(1),currentwidget);
    }
    else
    {
        qDebug()<<"找到键值";
        currentwidget->setParent(loginuser);
    }
    currentwidget->setObjectName(QStringLiteral("widget"));
    currentwidget->setGeometry(QRect(180, 40, 430, 350));
    currentwidget->raise();
    currentwidget->get(readlist.at(1),templist.at(0),chatpath);
    currentwidget->gettext(readlist.at(2),readlist.at(3));
    currentwidget->show();
}
//配置qq登录的信息
void MainWindow::set()
{
    mytimer1.stop();
    loginuser->get(user,imagebuf);
}
//获取本地登录过的账号
QMap<QString,QString> *MainWindow::getlogin()
{
    QMap<QString,QString> *login=new QMap<QString,QString>;
    login->clear();
    QFile loginname("F:/QT/protect/day08/TXT/loginname.txt");
    if(loginname.open(QIODevice::ReadWrite))
    {
        char buf[100];
        while(!loginname.atEnd())
        {
            memset(buf,0,100);
            loginname.readLine(buf,100);
            QString temp(buf);
            temp=temp.remove("\n");  //删除换行符"\r\n"
            QStringList templist = temp.split("::",QString::SkipEmptyParts);
            if(templist.size()>=2)
                login->insert(templist.at(0),templist.at(1));
        }
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    loginname.close();
    return login;
}
//自动登录
void MainWindow::login()
{
    automaticflag=1;
    mytimer.stop();
    if(connectflag==1)
    {
        QString send = QString("%1::%2::%3::%4").arg("@#$%login").  \
                        arg(ui->comboBox->currentText()).   \
                        arg(ui->lineEdit_2->text()).     \
                        arg(path);

        qDebug()<<send;
        mysorcket->write(send.toUtf8());
    }
    else
        QMessageBox::warning(this,"警告","无法登录！\n原因：没有网络！");
}
//保存在本地登录的信息
void MainWindow::savelogin(QString num, QString path)
{
    QFile loginread("F:/QT/protect/day08/TXT/loginname.txt");
    if(loginread.open(QIODevice::ReadWrite))
    {
        char buf[100]={0};
        while(!loginread.atEnd())
        {
            memset(buf,0,100);
            loginread.readLine(buf,100);
            if(QString(buf).indexOf(num)!=-1)
            {
                return ;
            }
        }
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    loginread.close();
    QFile loginwrite("F:/QT/protect/day08/TXT/loginname.txt");
    if(loginwrite.open(QIODevice::ReadWrite|QIODevice::Append))
    {
        QString temp=QString("%1::%2\n").arg(num).arg(path);
        loginwrite.write(temp.toUtf8());
    }
    else
    {
        qDebug()<<"打开文件失败";
    }
    loginwrite.close();
}

//产生随机数
void MainWindow::getrand()
{
    qsrand(QTime::currentTime().msec()+QTime::currentTime().second()*1000);
    rand = qrand()%64476+1024;
    if(randcount==999)
    {
        randtimer.stop();
        //ip
        //设置绑定客户端ip
        qDebug()<<rand;
        mysorcket->bind(QHostAddress::Any,rand);
        //连接服务器ip
        //连接服务器
        mysorcket->connectToHost(QHostAddress("192.168.29.22"),65530);
    }
    randcount++;
}

//保存
void MainWindow::save(QString name,QString pass,QString headpath)
{
    if(remember==1&&automatic==1)
    {
        saveremembername(name,pass,headpath);
        saveautomaticname(name,pass,headpath);
    }
    else if(remember==1)
        saveremembername(name,pass,headpath);
    else if(automatic==1)
        saveautomaticname(name,pass,headpath);
    else
        savelogin(name,headpath);
}

//连接成功
void MainWindow::hasconnected()
{
    qDebug()<<"连接成功！";
    connectflag=1;   //连接成功

}
//读取数据
void MainWindow::toreadcliet()
{
    qDebug()<<"有数据可读！";
    QString readbuf = mysorcket->readAll();

    QStringList readlist = readbuf.split("::");
    if(!readlist.isEmpty())
    {
        if(readlist.at(0)=="@#$%argee")
            login(true);
        else if(readlist.at(0)=="@#$%unargee")
            login(false);
        else if(readlist.at(0)=="@#$%logining")
            QMessageBox::warning(this,"警告","登录失败！\n原因：已登录！");
        else if(readlist.at(0)=="@#$%getreg")
        {
            if(readlist.size()>=6)
            {
                RegInfo newreg;
                newreg.setMsg(readlist.at(1),readlist.at(2),readlist.at(3),readlist.at(4),readlist.at(5));
                reginfo->append(newreg);
                qDebug()<<readlist.at(1);
            }
        }
        else if(readlist.at(0)=="@#$%null")
        {
            qDebug()<<"暂无注册用户信息！";
            QMessageBox::information(this,"提示","提示:暂无注册用户信息！");
        }
        else if(readlist.at(0)=="@#$%friend")   //查找添加好友
        {
            //查找到的好友信息
            friendaddflag=1;
            friendinfo.setMsg(readlist.at(1),readlist.at(2),readlist.at(3),readlist.at(4),readlist.at(5));
        }
        else if(readlist.at(0)=="@#$%friendon")
        {
            qDebug()<<__LINE__;
            //弹出确认添加好友框标志---好友申请信息
            friendflag=1;
            RegInfo addother(readlist.at(1),readlist.at(2),readlist.at(3),readlist.at(4),readlist.at(5));
            friendadd.append(addother);
        }
        else if(readlist.at(0)=="@#$%nofriend"||readlist.at(0)=="@#$%friendoff")
        {
            friendinfo.clear();
            friendadd.clear();
            friendflag=0;
            friendaddflag=0;
            if(readlist.at(0)=="@#$%friendoff")
            {
                QString text = QString("提示:%1拒绝了你的好友申请！").arg(readlist.at(1));
                QMessageBox::information(loginuser,"提示",text);
            }
        }
        else if(readlist.at(0)=="@#$%friendyes"||readlist.at(0)=="@#$%friends")
        {
            //添加到好友列表
            loginuser->addfriends(readlist.at(1),readlist.at(2));
        }
        else if(readlist.at(0)=="@#$%onebyone")  //聊天
        {
            chatonebyone(readlist);
        }
        else if(readlist.at(0)=="@#$%person")   //接收个人信息
        {
            imagebuf.clear();
            user.setMsg(readlist.at(1),readlist.at(2),readlist.at(3),readlist.at(4),readlist.at(5));
            imagebuf = readlist.at(6);
        }
        else if(readlist.at(0)=="@#$%head")
        {
            //获取头像
            if(QFile::exists(QString(":/new/prefix1/%1").arg(readlist.at(1))))
                headimage = QString("border-image: url(:/new/prefix1/%1);border-radius:45px;border:1px solid#000000;").arg(readlist.at(1));
            else
                headimage = QString("border-image: url(F:/QT/protect/day08/pic/%1);border-radius:45px;border:1px solid#000000;").arg(readlist.at(1));

            ui->pushButton->setStyleSheet(headimage);
        }
        else if(readlist.at(0)=="@#$%nohead")
        {
            //初始化头像
            headimage = QString("border-image: url(:/new/prefix1/10.jpg);border-radius:45px;border:1px solid#000000;");
            ui->pushButton->setStyleSheet(headimage);
        }
    }
}
//服务器断开
void MainWindow::quitConnection()
{
    QMessageBox::warning(this,"警告","警告:服务器已断开！");
    this->close();
}

//注册
void MainWindow::on_reg_clicked()
{
    if(connectflag==1)
    {
        //打开注册对话框
        mydialog *regdialog = new mydialog(this);
        regdialog->setStyleSheet("QDialog{border-image: url(:/new/prefix1/经典.jpg);}");
        //窗口图标和名称
        regdialog->setWindowIcon(QIcon(":/new/prefix1/qq.jpg"));
        regdialog->setWindowTitle("QQ注册");
        regdialog->resize(300,200);
        QLabel userlb("  用户名:",regdialog);
        QLabel passlb("    密码:",regdialog);
        QLabel phonelb("手机号码:",regdialog);
        regdialog->usered.setParent(regdialog);
        regdialog->passed.setParent(regdialog);
        regdialog->phoneed.setParent(regdialog);
        regdialog->usered.setMaxLength(11);
        regdialog->passed.setMaxLength(16);
        regdialog->phoneed.setMaxLength(11);
        regdialog->reg.setParent(regdialog);
        regdialog->reg.setText("注册");
        regdialog->cancel.setParent(regdialog);
        regdialog->cancel.setText("取消");
        regdialog->reg.setMinimumSize(80,21);
        regdialog->reg.setMaximumSize(80,21);
        regdialog->cancel.setMinimumSize(80,21);
        regdialog->cancel.setMaximumSize(80,21);
        //创建布局管理器
        QHBoxLayout *hlay = new QHBoxLayout;
        QHBoxLayout *hlay1 = new QHBoxLayout;
        QHBoxLayout *hlay3 = new QHBoxLayout;
        QGridLayout *grid = new QGridLayout(regdialog);
        QVBoxLayout *vlay = new QVBoxLayout;
        //添加组件
        hlay->addWidget(&userlb);
        hlay->addWidget(&regdialog->usered);
        hlay1->addWidget(&passlb);
        hlay1->addWidget(&regdialog->passed);
        hlay3->addWidget(&phonelb);
        hlay3->addWidget(&regdialog->phoneed);
        vlay->addLayout(hlay);
        vlay->addLayout(hlay1);
        vlay->addLayout(hlay3);
        grid->addLayout(vlay,1,0,Qt::AlignHCenter);

        grid->addWidget(&regdialog->reg,2,0,Qt::AlignLeft);
        grid->addWidget(&regdialog->cancel,2,0,Qt::AlignRight);

        //关联信号与槽函数
        connect(&regdialog->reg,SIGNAL(clicked()),regdialog,SLOT(regfinished()));
        connect(&regdialog->cancel,SIGNAL(clicked()),regdialog,SLOT(cancelreg()));
        connect(&regdialog->usered,SIGNAL(textEdited(QString)),regdialog,SLOT(isUseable(QString)));

        regdialog->exec();  //模态显示
    }
    else
        QMessageBox::warning(this,"警告","无法注册！\n原因：没有网络！");
}


//登录到QQ窗口
void MainWindow::on_login_clicked()
{
    if(ui->automatic_login->isChecked()) //真
        automatic=1;
    else
        automatic=0;
    if(ui->rememberpass->isChecked()) //真
        remember=1;
    else
        remember=0;
    automaticflag=0;
    if(connectflag)
    {
        QString send = QString("%1::%2::%3::%4").arg("@#$%login").  \
                        arg(ui->comboBox->currentText()).   \
                        arg(ui->lineEdit_2->text()).     \
                        arg(path);
        mysorcket->write(send.toUtf8());
    }
    else
    {
        QMessageBox::warning(this,"警告","无法登录！\n原因：没有网络！");
    }
}

//隐藏/显示密码
void MainWindow::on_hide_clicked()
{
    //隐藏
    if(flag==1)
    {
        ui->lineEdit_2->setEchoMode(QLineEdit::Password);
        flag=0;
    }
    else   //显示
    {
        ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
        flag=1;
    }
}


//自动登录
void MainWindow::on_automatic_login_clicked()
{
    if(ui->automatic_login->isChecked()) //真
        automatic=1;
    else
        automatic=0;
}
//记住密码
void MainWindow::on_rememberpass_clicked()
{
    if(ui->rememberpass->isChecked()) //真
        remember=1;
    else
        remember=0;
}
//找回密码
void MainWindow::on_findpass_clicked()
{
    if(connectflag==1)
    {
        //打开找回密码对话框
        mydialog *finddialog = new mydialog(this);
        finddialog->setStyleSheet("QDialog{border-image: url(:/new/prefix1/经典.jpg);}");
        //窗口图标和名称
        finddialog->setWindowIcon(QIcon(":/new/prefix1/qq.jpg"));
        finddialog->setWindowTitle("找回密码");
        finddialog->resize(300,200);
        QLabel userlb("  用户名:",finddialog);
        QLabel passlb("手机号码:",finddialog);
        finddialog->usered.setParent(finddialog);
        finddialog->phoneed.setParent(finddialog);
        finddialog->usered.setMaxLength(11);
        finddialog->phoneed.setMaxLength(11);
        finddialog->reg.setParent(finddialog);
        finddialog->reg.setText("找回密码");
        finddialog->cancel.setParent(finddialog);
        finddialog->cancel.setText("取消");
        finddialog->reg.setMinimumSize(80,21);
        finddialog->reg.setMaximumSize(80,21);
        finddialog->cancel.setMinimumSize(80,21);
        finddialog->cancel.setMaximumSize(80,21);
        //创建布局管理器
        QHBoxLayout *hlay = new QHBoxLayout;
        QHBoxLayout *hlay1 = new QHBoxLayout;
        QGridLayout *grid = new QGridLayout(finddialog);
        QVBoxLayout *vlay = new QVBoxLayout;
        //添加组件
        hlay->addWidget(&userlb);
        hlay->addWidget(&finddialog->usered);
        hlay1->addWidget(&passlb);
        hlay1->addWidget(&finddialog->phoneed);

        vlay->addLayout(hlay);
        vlay->addLayout(hlay1);
        grid->addLayout(vlay,1,0,Qt::AlignHCenter);

        grid->addWidget(&finddialog->reg,2,0,Qt::AlignLeft);
        grid->addWidget(&finddialog->cancel,2,0,Qt::AlignRight);
        //关联信号与槽函数
        connect(&finddialog->reg,SIGNAL(clicked()),finddialog,SLOT(findpass()));
        connect(&finddialog->cancel,SIGNAL(clicked()),finddialog,SLOT(cancelreg()));


        finddialog->exec();  //模态显示
    }
    else
        QMessageBox::warning(this,"警告","无法找回密码！\n原因：没有网络！");
}

//comboBox文本内容发送改变时发送的信号
void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->lineEdit_2->clear();
    if(arg1.isEmpty())
    {
        ui->lineEdit_3->setPlaceholderText("请输入用户名");
        //透明
        ui->comboBox->setStyleSheet("background-color: rgb(255, 255, 255,0);");
    }
    else
    {
        for(auto& i:loginlistly)
        {
            if(i.size()>=3)
                if(arg1==i.at(0))
                    ui->lineEdit_2->setText(i.at(1));
        }
        //不透明
        ui->comboBox->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid#000011;");
        ui->lineEdit_3->setPlaceholderText("");
        path.clear();
        headimage.clear();
        path = loginlist->value(arg1);
        if(!path.isEmpty())
        {
            if(QFile::exists(QString(":/new/prefix1/%1").arg(path)))
            {
                ui->pushButton->setStyleSheet(QString("border-radius:45px;border:1px solid#000000;border-image: url(:/new/prefix1/%1);").arg(path));
            }
            else
                ui->pushButton->setStyleSheet(QString("border-image: url(F:/QT/protect/day08/pic/%1);border-radius:45px;border:1px solid#000000;").arg(path));
            loginflag=1;
        }
        else
        {
            mysorcket->write("@#$%gethead");
            loginflag=0;
        }
    }
}
//点击下拉框了
void MainWindow::showpopup()
{
    //不透明
    ui->comboBox->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid#000011;");
}


//设置用户信息
void RegInfo::setMsg(QString newname,QString u, QString ps, QString pe,QString path)
{
    name=newname;
    username=u;
    pass=ps;
    phone=pe;
    headpath = path;
}
//保存ip端口号
void RegInfo::setMsg(QString ip, int port)
{
    this->ip=ip;
    this->port=port;
}
//清空
void RegInfo::clear()
{
    name.clear();
    username.clear();
    pass.clear();
    phone.clear();
    headpath.clear();
    ip.clear();
    port=0;
}

bool RegInfo::isEmpty()
{
    if(name.isEmpty()&&username.isEmpty()&&pass.isEmpty()&&phone.isEmpty()&&headpath.isEmpty()&&ip.isEmpty()&&port==0)
        return true;
    else
        return false;
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
//重载赋值运算符
RegInfo RegInfo::operator=(const RegInfo &Reg)
{
    name=Reg.name;
    username=Reg.username;
    pass=Reg.pass;
    phone=Reg.phone;
    headpath=Reg.headpath;
    this->ip=Reg.ip;
    this->port=Reg.port;
    return *this;
}
//退出整个程序
void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}
//打开或关闭软键盘
void MainWindow::on_pushButton_3_clicked()
{
    if(key==0)
    {
        mykeyboard = new keyboard(this);
        mykeyboard->setGeometry(QRect(130, 280, 550, 140));
        mykeyboard->raise();
        mykeyboard->show();
        key=1;
    }
    else
    {
        mykeyboard->close();
        key=0;
    }
}
