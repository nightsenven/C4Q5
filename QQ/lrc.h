#ifndef LRC_H
#define LRC_H

#include <QWidget>

namespace Ui {
class lrc;
}

class lrc : public QWidget
{
    Q_OBJECT

public:
    explicit lrc(QWidget *parent = 0);
    ~lrc();
    //设置QTextBrowser文本信息
    void set(QString lrc1,QString lrc2);
    void clear();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::lrc *ui;
};

#endif // LRC_H
