#ifndef SEND_H
#define SEND_H

#include <QWidget>
#include <QList>
namespace Ui {
class send;
}

class send : public QWidget
{
    Q_OBJECT
    friend class chat;
public:
    explicit send(QWidget *parent = 0);
    ~send();
    void getHtml(QString text);
    void getStr(QString text);
    void checkHtml(QString text);
    void set(QString name,QString path);
private slots:

    void on_textBrowser_textChanged();

private:
    Ui::send *ui;
    int hight;    //窗口的高
    int browserhight; //文本的高
    int browserwight;  //文本的宽
    int count;   //计数
    int linecount;   //换行符的个数
    QStringList emojiname;  //表情包
    QStringList emoji;   //发送的表情包
    QStringList endlist;
    QList<int> endwidth;
    int flag;
};

#endif // SEND_H
