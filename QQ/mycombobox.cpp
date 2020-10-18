#include "mycombobox.h"

mycombobox::mycombobox(QWidget *parent) : QComboBox(parent)
{

}
//自定义单击信号
void mycombobox::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        emit clicked();
    QComboBox::mousePressEvent(event);
}
