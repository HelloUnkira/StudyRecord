#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&client,SIGNAL(readyRead()),this,SLOT(on_read()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    QString ip = ui->lineEdit_2->text();
    QString port = ui->lineEdit_3->text();
}

void MainWindow::on_pushButton_clicked()
{
    QString ip = ui->lineEdit_2->text();
    QString port = ui->lineEdit_3->text();
    QString str = ui->lineEdit->text();
    client.writeDatagram(str.toUtf8(),QHostAddress(ip),quint16(port.toInt()));
}

void MainWindow::on_read()
{
    QByteArray databuf;
    QHostAddress addr;
    quint16 port = 8090;
    client.readDatagram(databuf.data(),databuf.length(),&addr,&port);
    ui->textEdit->setText(QString::fromStdString(databuf.toStdString()));
}
