#ifndef KEYBORADL_H
#define KEYBORADL_H

#include <QWidget>

namespace Ui {
class keyboradl;
}

class keyboradl : public QWidget
{
    Q_OBJECT

public:
    explicit keyboradl(QWidget *parent = 0);
    ~keyboradl();

private:
    Ui::keyboradl *ui;
};

#endif // KEYBORADL_H
