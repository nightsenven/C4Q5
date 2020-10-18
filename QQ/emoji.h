#ifndef EMOJI_H
#define EMOJI_H

#include <QWidget>
#include <QStringList>
#include <QPushButton>
#include <QList>

namespace Ui {
class emoji;
}

class emoji : public QWidget
{
    Q_OBJECT

public:
    explicit emoji(QWidget *parent = 0);
    ~emoji();
    int disemoji(int &num);
    void getbt();
    void imgPathToHtml(QString &path);

public slots:
    void on_pushButton_clicked();

    void on_pushButton1_clicked();

    void on_pushButton_1_clicked();

private:
    Ui::emoji *ui;
    QStringList emojiname;  //表情包
    int emojinum;
    int emojicount;
    int count;
    QList<QPushButton *> allbt;
};

#endif // EMOJI_H
