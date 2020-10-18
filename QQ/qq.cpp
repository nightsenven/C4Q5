#include "qq.h"
#include "ui_qq.h"

#include "addfriend.h"
#include <QDebug>
#include <QIcon>
#include <QTest>
#include <QFileDialog>
#include <QProcess>
#include <QListWidget>
#include <QPushButton>
#include "isadd.h"
QQ::QQ(QString usernum, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QQ)
{
    ui->setupUi(this);
    lrcwidget = new lrc(this);
    lrcwidget->setObjectName(QStringLiteral("widget"));
    lrcwidget->setGeometry(QRect(169, 9, 470, 80));
    play=0;
    ms=0;
    lrcflag=0;
    addflag=0;
    headimgmap.clear();
    qDebug()<<usernum;
    userinfo.username = usernum;
    qobject_cast<MainWindow *>(parentWidget())->mysorcket->write(QString("@#$%person::%1").arg(usernum).toUtf8());
    //窗口图标和名称
    setWindowIcon(QIcon(":/new/prefix1/qq.jpg"));
    setWindowTitle("QQ");
    chatobject = new QMap<QString,chat *>;
    ui->radioButton->setChecked(true);
    //设置树状列表框的水平头
    ui->treeWidget->setHeaderLabels(QStringList()<<"联系人");
    //记载顶层节点
    item1=new QTreeWidgetItem(QStringList()<<"我的好友");
    item2=new QTreeWidgetItem(QStringList()<<"家人");
    item3=new QTreeWidgetItem(QStringList()<<"同学");
    item4=new QTreeWidgetItem(QStringList()<<"同事");
    ui->treeWidget->addTopLevelItem(item1);
    ui->treeWidget->addTopLevelItem(item2);
    ui->treeWidget->addTopLevelItem(item3);
    ui->treeWidget->addTopLevelItem(item4);

    //获取歌曲和歌词
    QString dirname = "F:/嵌入式/share/music/";
    loadsong(dirname);
    //加载歌曲到listwidget
    for(auto &i:song_sheet)
    {
        QString tempname = i.mid(dirname.size()+1);
        QListWidgetItem *song=new QListWidgetItem(tempname);
        ui->listWidget->addItem(song);
    }

    friendinfolist = new QList<RegInfo>;
    friendinfolist->clear();
    //关联信号与槽函数
    connect(ui->radioButton_2,SIGNAL(clicked()),this,SLOT(on_radioButton_clicked()));
    connect(ui->radioButton_3,SIGNAL(clicked()),this,SLOT(on_radioButton_clicked()));

    connect(&mytimer,SIGNAL(timeout()),this,SLOT(dislrc()));
    connect(&mytimerwhile,SIGNAL(timeout()),this,SLOT(scanning()));
    mytimerwhile.start(200);
}

QQ::~QQ()
{
    delete ui;
}

//获取登录的个人信息
void QQ::get(RegInfo loginuesr,QString imagebuf)
{
    userinfo = loginuesr;
    setpath(userinfo.headpath,imagebuf);
    ui->label->setText(userinfo.name);
    getfriend();     //加载好友
}

void QQ::setname(QString name)
{
    ui->label->setText(name);
}
//设置头像
void QQ::setpath(QString path,QString imagebuf)
{
    QString headimage;
    if(QFile::exists(QString(":/new/prefix1/%1").arg(path)))
        headimage = QString("border-image: url(:/new/prefix1/%1);border-radius:15px;border:1px solid#000000;").arg(path);

    else if(QFile::exists(QString("F:/QT/protect/day08/pic/%1").arg(path)))
        headimage = QString("border-image: url(F:/QT/protect/day08/pic/%1);border-radius:15px;border:1px solid#000000;").arg(path);

    else
    {
        qDebug()<<"新建";
        QFile headimagefile(QString("F:/QT/protect/day08/pic/%1").arg(path));
        if(headimagefile.open(QIODevice::ReadWrite|QIODevice::Text))
        {
            headimagefile.write(imagebuf.toUtf8());
        }
        else
            qDebug()<<"新建失败";
        headimagefile.close();
        headimage = QString("border-image: url(F:/QT/protect/day08/pic/%1);border-radius:15px;border:1px solid#000000;").arg(path);
    }
    ui->pushButton->setStyleSheet(headimage);
    userinfo.setPath(path);
}
//加载歌曲曲目
int QQ::loadsong(QString dirname)
{
    //打开目录
    QDir dir(dirname);
    //读取目录
    QStringList mylist = dir.entryList();
    for(auto& i:mylist)
    {
        QString tempname = QString("%1/%2").arg(dirname).arg(i);
        QFileInfo finfo(tempname);
        if(finfo.isDir())   //目录
        {
            if(i=="."||i=="..")
                continue;
            loadsong(tempname);
        }
        else if(finfo.isFile()) //文件
        {
            if(tempname.indexOf(".mp3")!=-1)
            {
                //添加歌曲
                song_sheet.push_back(tempname);
            }
            else if(tempname.indexOf(".lrc")!=-1)
            {
                //添加歌曲歌词
                song_lyric.push_back(tempname);
            }
        }

    }
    //如果目录为空则直接返回
    return 0;
}
//获取当前播放歌曲所在listwidget的行号
int QQ::getrow(QString nowname)
{
    QString nowsongname = QString("%1.mp3").arg(nowname);
    for(int i=0;i<ui->listWidget->count();i++)
    {
        if(nowsongname==ui->listWidget->item(i)->text())
            return i;
    }
    return -1;
}
//获取当前歌曲的歌词
void QQ::getlrc(QString nowname)
{
    nowsong_lyric.clear();
    QString lrcname;

    for(auto &i:song_lyric)
    {
        if(i.indexOf(nowname)!=-1)
        {
            lrcname=i;
            break;
        }
    }
    if(!lrcname.isEmpty())
    {
        //打开读取歌词信息
        QFile lyric(lrcname);
        if(lyric.open(QIODevice::ReadOnly))
        {
            char linetext[100]={0};
            int i=0;
            while(!lyric.atEnd())  //文件读到末尾
            {
                memset(linetext,0,100);   //清空数组
                lyric.readLine(linetext,100);
                QString temp(linetext);
                temp.remove(0,1); //删除第一个字节
                if(!temp.isEmpty())
                {
                    QStringList templist = temp.split("]");
                    long ms = toMs(templist.at(0)); //时间字符串转换成毫秒
                    if(i==0)
                    {
                        songms=ms;
                        i++;
                    }

                    //插入Qmap
                    // 等价于 lrcinfo.insert(templist.at(0),templist.at(1));
                    //lrcinfo[templist.at(0)]=templist.at(1);
                    nowsong_lyric.insert(ms,templist.at(1));
                }
            }
        }
    }
}
//时间字符串转换成毫秒
long QQ::toMs(QString time)
{
    long sec=0;
    QStringList timelist = time.split(":");
    sec+=timelist.at(0).toInt()*60*1000;
    timelist = timelist.at(1).split(".");
    sec+=timelist.at(0).toInt()*1000;
    sec+=timelist.at(0).toInt()*10;
    return sec;
}

//自动下一首
void QQ::nextsong()
{
    int newrow = getrow(lrcname);
    if(newrow!=-1)
    {
        newrow+=1;
        if(newrow==ui->listWidget->count())
            newrow=0;
        QProcess *pro=new QProcess;
        QProcess *newpro=new QProcess;
        pro->start("killall -KILL madplay");
        //判断madplay这个进程是不是彻底结束了
        if(pro->waitForFinished()) //说明madplay结束
        {
            qDebug()<<"进程完成";
            //拼接得到完整播放命令
            QString cmd=QString("madplay ../music/%1 &").arg(ui->listWidget->item(newrow)->text());
            //播放音乐
            newpro->start(cmd);
            mytimer.stop();
        }
        else
        {
            qDebug()<<"进程未完成";
            ms=0;
            mytimer.stop();
            return ;
        }
        ms=0;
        mytimer.start(10);
        lrcname = ui->listWidget->item(newrow)->text().remove(".mp3"); //当前播放歌曲的名字
        getlrc(lrcname);  //获取歌词
        qDebug()<<lrcname;
        ui->pushButton_5->setStyleSheet("border-radius:15px;"
                                        "border-image: url(:/new/prefix1/暂停.jpg);"
                                        "border:1px solid#000000;");
        play=1;
    }
    else
    {
        ms=0;
        mytimer.stop();
    }

}

//发送好友信息信号
void QQ::getfriend()
{
    headimgmap.clear();
    qobject_cast<MainWindow *>(parentWidget())->mysorcket->write(QString("@#$%getfriend::%1").arg(userinfo.username).toUtf8());
}
//添加好友到好友项
void QQ::addfriends(QString name,QString headpath)
{
    QString path;
    if(QFile::exists(QString(":/new/prefix1/%1").arg(headpath)))
        path = QString(":/new/prefix1/%1").arg(headpath);
    else
        path = QString("F:/QT/protect/day08/pic/%1").arg(headpath);
    QIcon icon(path);
    QTreeWidgetItem *temp = new QTreeWidgetItem(QStringList()<<name);
    temp->setIcon(0,icon);
    item1->addChild(temp);
    headimgmap.insert(name,headpath);  //插入
}
//双击联系人
void QQ::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    //过滤顶层节点
    if(item->parent()!=Q_NULLPTR)
    {
        qDebug()<<item->text(column);
        currentname = item->text(column);
        QString chatpath=headimgmap.value(currentname);

        QStringList templist = item->text(column).split("(");
        templist = templist.at(1).split(")");

        chat *widget = chatobject->value(item->text(column));
        if(widget==Q_NULLPTR)
        {
            qDebug()<<"找不到键值";
            widget=new chat(this);
            chatobject->insert(item->text(column),widget);
        }
        else
        {
            qDebug()<<"找到键值";
            widget->setParent(this);
        }
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(180, 40, 430, 350));
        widget->raise();
        widget->get(currentname,templist.at(0),chatpath);
        widget->show();
    }
}

//添加好友
void QQ::on_pushButton_2_clicked()
{
    qobject_cast<MainWindow *>(parentWidget())->friendinfo.clear();
    addfriend *add = new addfriend(this);
    add->exec();
}
//个性装扮
void QQ::on_pushButton_3_clicked()
{
    //打开文件对话框
    QString filename = QFileDialog::getOpenFileName(this,"自定义装扮");
    if(!filename.isEmpty())
    {
        //设置背景
        QString background = QString("QMainWindow{border-image:url(%1);}").arg(filename);
        this->setStyleSheet(background);
    }
}
//装扮选择
void QQ::on_radioButton_clicked()
{
    //判断radioButton是否可以选中，默认不可选中（false）
    if(ui->radioButton->isChecked()) //真
        this->setStyleSheet("QMainWindow{border-image:url(:/new/prefix1/经典.jpg);}");
    else  if(ui->radioButton_2->isChecked()) //真
        this->setStyleSheet("QMainWindow{border-image:url(:/new/prefix1/简约.jpg);}");
    else  if(ui->radioButton_3->isChecked()) //真
        this->setStyleSheet("QMainWindow{border-image:url(:/new/prefix1/柔和.jpg);}");
}
//定时10ms
void QQ::dislrc()
{
    ms+=10;
    if(ms==songms)
    {
        QTest::qSleep(1000);
        nextsong();
        return ;
    }
    QMap<long, QString>::const_iterator it = nowsong_lyric.find(ms);
    if(it!=nowsong_lyric.end()&&lrcflag==0)  //第一次找到
    {
        QString lrc1,lrc2;
        lrc1= it.value();
        it++;
        if(it==nowsong_lyric.end())
            lrc2.clear();
        else
            lrc2 = it.value();
        lrcwidget->set(lrc1,lrc2);
        lrcflag=1;
    }
    else if(it!=nowsong_lyric.end()&&lrcflag==1) //第二次找到
        lrcflag=0;
}
//扫描获取好友申请
void QQ::scanning()
{
    //有好友申请，弹出好友申请框
    if(qobject_cast<MainWindow *>(parentWidget())->friendflag==1)
    {
        qobject_cast<MainWindow *>(parentWidget())->friendflag=0;
        //打开好友通知
        on_pushButton_10_clicked();
        //添加好友申请
        for(int i=0;i<qobject_cast<MainWindow *>(parentWidget())->friendadd.size();i++)
            newadd->disfriend(QString("%1(%2)").arg(qobject_cast<MainWindow *>(parentWidget())->friendadd.at(i).name).\
                            arg(qobject_cast<MainWindow *>(parentWidget())->friendadd.at(i).username),\
                                qobject_cast<MainWindow *>(parentWidget())->friendadd.at(i).headpath);
    }
}

//双击歌曲播放
void QQ::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    lrcname = item->text().remove(".mp3"); //当前播放歌曲的名字

    getlrc(lrcname);  //获取歌词
    //创建线程等待进程
    QString cmd;
    //新建一个进程
    QProcess *pro=new QProcess;
    QProcess *newpro=new QProcess;
    if(play==1)   //有歌曲在播放
    {
        //关闭音乐
        pro->start("killall -KILL madplay");
        //判断madplay这个进程是不是彻底结束了
        if(pro->waitForFinished()) //说明madplay结束
        {
            qDebug()<<"进程完成";
            //拼接得到完整播放命令
            cmd=QString("madplay ../music/%1 &").arg(item->text());

            newpro->start(cmd);
            mytimer.stop();
        }
        else
            qDebug()<<"进程未完成";

    }
    else if(play==0)  //无歌曲在播放
    {
        cmd=QString("madplay ../music/%1 &").arg(item->text());
        pro->start(cmd);
    }
    lrcwidget->clear();
    play=1;
    ms=0;
    mytimer.start(10);  //定时10ms
    ui->pushButton_5->setStyleSheet("border-radius:15px;"
                                    "border-image: url(:/new/prefix1/暂停.jpg);"
                                    "border:1px solid#000000;");
}
//停止播放
void QQ::on_pushButton_7_clicked()
{
    //新建一个进程
    QProcess *pro=new QProcess;
    //关闭音乐
    pro->start("killall -KILL madplay");
    ui->pushButton_5->setStyleSheet("border-radius:15px;"
                                    "border-image: url(:/new/prefix1/播放.jpg);"
                                    "border:1px solid#000000;");
    lrcwidget->set(QString(),QString());   //清空歌词显示
    ms=0;
    play=0;
    mytimer.stop();
}
//暂停播放
void QQ::on_pushButton_5_clicked()
{
    //新建一个进程
    QProcess *pro=new QProcess;
    if(play==0)   //继续
    {
        pro->start("killall -CONT madplay");
        ui->pushButton_5->setStyleSheet("border-radius:15px;"
                                        "border-image: url(:/new/prefix1/暂停.jpg);"
                                        "border:1px solid#000000;");
        play=1;
        mytimer.start(10);
    }
    else if(play==1)   //暂停
    {
        pro->start("killall -STOP madplay");
        ui->pushButton_5->setStyleSheet("border-radius:15px;"
                                        "border-image: url(:/new/prefix1/播放.jpg);"
                                        "border:1px solid#000000;");
        play=0;
        mytimer.stop();
    }
}
//上一首
void QQ::on_pushButton_4_clicked()
{
    //新建一个进程
    QProcess *nowpro=new QProcess;
    //关闭音乐
    nowpro->start("killall -KILL madplay");
    //判断madplay这个进程是不是彻底结束了
    if(nowpro->waitForFinished()) //说明madplay结束
    {
        //拼接得到完整播放命令
        QProcess *prevpro=new QProcess;
        int prevrow = getrow(lrcname)-1;
        if(prevrow!=-2)
        {
            if(prevrow==-1)
            {
                prevrow=ui->listWidget->count()-1;
            }
            QString cmd=QString("madplay ../music/%1 &").arg(ui->listWidget->item(prevrow)->text());

            lrcname = ui->listWidget->item(prevrow)->text().remove(".mp3"); //当前播放歌曲的名字
            getlrc(lrcname);  //获取歌词
            ui->pushButton_5->setStyleSheet("border-radius:15px;"
                                            "border-image: url(:/new/prefix1/暂停.jpg);"
                                            "border:1px solid#000000;");
            ms=0;
            prevpro->start(cmd);
            mytimer.start(10);
        }
    }
}
//下一首
void QQ::on_pushButton_6_clicked()
{
    //新建一个进程
    QProcess *nowpro=new QProcess;
    //关闭音乐
    nowpro->start("killall -KILL madplay");
    //判断madplay这个进程是不是彻底结束了
    if(nowpro->waitForFinished()) //说明madplay结束
    {
        //拼接得到完整播放命令
        QProcess *nextpro=new QProcess;
        int nextrow = getrow(lrcname)+1;
        if(nextrow!=0)
        {
            if(nextrow==ui->listWidget->count())
            {
                nextrow=0;
            }
            QString cmd=QString("madplay ../music/%1 &").arg(ui->listWidget->item(nextrow)->text());

            lrcname = ui->listWidget->item(nextrow)->text().remove(".mp3"); //当前播放歌曲的名字
            getlrc(lrcname);  //获取歌词
            ui->pushButton_5->setStyleSheet("border-radius:15px;"
                                            "border-image: url(:/new/prefix1/暂停.jpg);"
                                            "border:1px solid#000000;");
            ms=0;
            nextpro->start(cmd);
            mytimer.start(10);
        }
    }
}
//打开歌词
void QQ::on_pushButton_8_clicked()
{
    lrcwidget->raise(); //置顶
    lrcwidget->show();
}
//关闭歌词
void QQ::on_pushButton_9_clicked()
{
    lrcwidget->hide();
}

//菜单
void QQ::on_pushButton_clicked()
{
    //创建右键菜单
    rm = new rightmenu(this);
    rm->setGeometry(QRect(30, 30, 130, 120));
    rm->setMinimumSize(QSize(130, 120));
    rm->setMaximumSize(QSize(130, 120));
    rm->raise();
    rm->get(userinfo.username);
    rm->show();
}
//好友通知
void QQ::on_pushButton_10_clicked()
{
    if(addflag==0)
    {
        newadd=new isadd(this);
        newadd->raise();
        newadd->show();
        addflag=1;
    }
    else if(addflag==1)
        newadd->show();

}
