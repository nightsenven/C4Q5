#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class mypushbutton : public QPushButton
{
    Q_OBJECT
public:
    explicit mypushbutton(QWidget *parent = 0);

signals:

public slots:
    void pressdown();
private:
    int flag;
};

#endif // MYPUSHBUTTON_H
