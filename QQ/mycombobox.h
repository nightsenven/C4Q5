#ifndef MYCOMBOCOX_H
#define MYCOMBOCOX_H

#include <QComboBox>
#include <QMouseEvent>
class mycombobox : public QComboBox
{
    Q_OBJECT
public:
    explicit mycombobox(QWidget *parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
signals:
    void clicked(); //单击信号
public slots:
};

#endif // MYCOMBOCOX_H
