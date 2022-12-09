#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QDir>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_lineEdit_returnPressed()
{
    //press return key ,process into here
    QString path = ui->lineEdit->text();
    QDir dir(path);
    if (!dir.exists())
        return;

    QStringList list =  dir.entryList();
    QFileInfo fileinfo;
     fileinfo.setFile(path);
     QString pathname = fileinfo.absoluteFilePath() ;
     for (int i = 0; i < list.count(); i++) {
         if (list.at(i) == ".")
             continue;
        QString path = pathname + "/" + list.at(i);
        ui->listWidget->addItem((path));
   }
}

void Widget::on_listWidget_clicked(const QModelIndex &index)
{

}



void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QListWidgetItem  *item;
    item = ui->listWidget->item(index.row());
    QString path = item->text();
    QFileInfo fileinfo;
    fileinfo.setFile(path);
   if (fileinfo.isDir()) {
        //update all data
       ui->lineEdit->clear();
       ui->lineEdit->setText(path);
       ui->listWidget->clear();
       on_lineEdit_returnPressed();
   }
}
