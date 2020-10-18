#ifndef QQ_H
#define QQ_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include "mainwindow.h"
#include "chat.h"
#include <QMap>
#include "lrc.h"
#include <QTimer>
#include <QProcess>
#include "rightmenu.h"
#include "isadd.h"
namespace Ui {
class QQ;
}

class QQ : public QMainWindow
{
    Q_OBJECT
    friend class MainWindow;
    friend class addfriend;
    friend class chat;
    friend class rightmenu;
    friend class personally;
    friend class mypushbutton;
    friend class isadd;
public:
    explicit QQ(QString usernum,QWidget *parent = 0);
    ~QQ();
    void get(RegInfo loginuesr, QString imagebuf);
    void setname(QString name);
    void setpath(QString path, QString imagebuf);
    //自动加载歌曲曲目
    int loadsong(QString dirname);
    //获取当前播放歌曲所在listwidget的行号
    int getrow(QString nowname);
    void getlrc(QString nowname);
    long toMs(QString time);
    void nextsong();

    void getfriend();

    void addfriends(QString name, QString headpath);
public slots:
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_radioButton_clicked();

    void dislrc();

    void scanning();
private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_7_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();


    void on_pushButton_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::QQ *ui;
    QTreeWidgetItem *item1;
    QTreeWidgetItem *item2;
    QTreeWidgetItem *item3;
    QTreeWidgetItem *item4;
    QMap<QString,chat *> *chatobject;

    QStringList friendname;
    QList<RegInfo> *friendinfolist;

    QString currentname;   //当前的聊天对象
    RegInfo userinfo;   //当前登录的用户信息

    QList<QString> song_sheet;  //歌单
    QList<QString> song_lyric;     //歌词名
    QMap<long,QString> nowsong_lyric;     //当前歌曲歌词内容
    QString lrcname;    //当前播放歌曲的无后缀名
    int play;    //当前是否有歌曲在播放  0无播放

    lrc *lrcwidget;  //歌词窗口

    QTimer mytimer; //定时器
    QTimer mytimerwhile; //定时器(循环或取)
    long ms;   //已播放的时间 ms
    int lrcflag;
    long songms;    //歌曲总时长
    QMap<QString,QString> headimgmap;
    rightmenu *rm;
    isadd *newadd;
    int addflag;
};

#endif // QQ_H
