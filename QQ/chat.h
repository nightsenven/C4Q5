#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QtWidgets/QScrollArea>
#include <QScrollBar>
#include "receive.h"
#include "send.h"
#include "emoji.h"
#include <QIcon>
#include "keyboard.h"
namespace Ui {
class chat;
}

class chat : public QWidget
{
    Q_OBJECT
    friend class emoji;
    friend class keyboard;
    friend class mypushbutton;
public:
    chat(){}
    explicit chat(QWidget *parent = 0);
    ~chat();
    void get(QString chatname,QString chatnum,QString headpath);
    void gettext(QString text, QString text1);
    void set(QString text);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_5_clicked();

private:
    Ui::chat *ui;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    //widget的高
    int y;
    send *sendwidget;
    receive *receivewidget;
    //聊天对象
    QString chatname;
    QString chatnum;
    QString headpath;
    int emojiflag;
    emoji *emojiwidget;
    keyboard *mykeyboard;
};

#endif // CHAT_H
