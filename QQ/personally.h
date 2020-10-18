#ifndef PERSONALLY_H
#define PERSONALLY_H

#include <QDialog>
#include <QList>
#include <QFile>
namespace Ui {
class personally;
}

class personally : public QDialog
{
    Q_OBJECT

public:
    explicit personally(QWidget *parent = 0);
    ~personally();
    void set();
    void change(int flag, QString usernum, QString changetext, QString path);
    QString readimage(QString path);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::personally *ui;
    QString usernum;
    QString userphone;
    QStringList changebuf;
};

#endif // PERSONALLY_H
