#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    ui->pushButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    QString ip = ui->lineEdit_2->text();
    QString port = ui->lineEdit_3->text();
    client.connectToHost(ip,port.toInt());

    connect(&client,SIGNAL(connected()),this,SLOT(on_connected()));
}

void MainWindow::on_connected()
{
    //already connect server
    ui->pushButton->setDisabled(false);
    connect(&client,SIGNAL(readyRead()),this,SLOT(on_received()));
}

void MainWindow::on_pushButton_clicked()
{
    QString str = ui->lineEdit->text();
    if (client.isWritable()) {
        client.write(str.toUtf8(),str.length());
    }
    ui->lineEdit->clear();
}

void MainWindow::on_received()
{
    QString str = client.readAll();
    ui->textEdit->setText(str);
}
