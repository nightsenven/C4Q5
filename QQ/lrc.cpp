#include "lrc.h"
#include "ui_lrc.h"
#include <QFontDialog>
#include <QColorDialog>
#include <QFont>
#include <QColor>
#include "qq.h"
lrc::lrc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lrc)
{
    ui->setupUi(this);
    hide();
}

lrc::~lrc()
{
    delete ui;
}
//设置文本信息
void lrc::set(QString lrc1, QString lrc2)
{
    ui->textBrowser->setText(lrc1);
    ui->textBrowser_2->setText(lrc2);
    ui->textBrowser->setAlignment(Qt::AlignLeft|Qt::AlignVCenter); //左对齐
    ui->textBrowser_2->setAlignment(Qt::AlignRight|Qt::AlignVCenter);//右对齐
}
//清空
void lrc::clear()
{
    ui->textBrowser->clear();
    ui->textBrowser_2->clear();
}
//选择字体
void lrc::on_pushButton_clicked()
{
    bool ok;
    QFont selfont = QFontDialog::getFont(&ok,this);
    if(ok)
    {
        ui->textBrowser->setFont(selfont);
        ui->textBrowser_2->setFont(selfont);
    }
}
//选择颜色
void lrc::on_pushButton_2_clicked()
{
    QColor selcolor = QColorDialog::getColor(Qt::white,this);
    if(selcolor.isValid())
    {
        ui->textBrowser->setTextColor(selcolor);
        ui->textBrowser_2->setTextColor(selcolor);
        ui->textBrowser->setText(ui->textBrowser->toPlainText());
        ui->textBrowser_2->setText(ui->textBrowser_2->toPlainText());
        ui->textBrowser->setAlignment(Qt::AlignLeft|Qt::AlignVCenter); //左对齐
        ui->textBrowser_2->setAlignment(Qt::AlignRight|Qt::AlignVCenter);//右对齐
    }
}
//字体放大
void lrc::on_pushButton_3_clicked()
{
    QFont curfont = ui->textBrowser->font(); //当前字体
    int fontsize = curfont.pointSize();
    curfont.setPointSize(fontsize+1);
    ui->textBrowser->setFont(curfont);
    ui->textBrowser_2->setFont(curfont);
}
//字体缩小
void lrc::on_pushButton_4_clicked()
{
    QFont curfont = ui->textBrowser->font(); //当前字体
    int fontsize = curfont.pointSize();
    curfont.setPointSize(fontsize-1);
    ui->textBrowser->setFont(curfont);
    ui->textBrowser_2->setFont(curfont);
}
