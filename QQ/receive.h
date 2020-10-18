#ifndef RECEIVE_H
#define RECEIVE_H

#include <QWidget>

namespace Ui {
class receive;
}

class receive : public QWidget
{
    Q_OBJECT
    friend class chat;
public:
    explicit receive(QWidget *parent = 0);
    ~receive();
    void getHtml(QString text);
    void getStr(QString text);
    void checkHtml(QString text);
    void set(QString name, QString headpath);
private slots:
    void on_textBrowser_textChanged();

private:
    Ui::receive *ui;
    int hight;    //窗口的高
    int browserhight; //文本的高
    int browserwight; //宽
    int count;   //计数
    int linecount;
    QStringList emojiname;  //表情包
    QStringList emoji;   //发送的表情包
    QStringList endlist;
    QList<int> endwidth;
    int flag;
};

#endif // RECEIVE_H
