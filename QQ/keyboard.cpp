#include "keyboard.h"
#include "ui_keyboard.h"
#include <QDebug>
#include "mainwindow.h"
keyboard::keyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::keyboard)
{
    ui->setupUi(this);
}

keyboard::~keyboard()
{
    delete ui;
}
