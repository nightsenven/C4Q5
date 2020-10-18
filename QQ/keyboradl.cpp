#include "keyboradl.h"
#include "ui_keyboradl.h"

keyboradl::keyboradl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::keyboradl)
{
    ui->setupUi(this);
}

keyboradl::~keyboradl()
{
    delete ui;
}
