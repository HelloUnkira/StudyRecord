#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    ui->pushButton->setDisabled(true);
    server.bind(QHostAddress("192.168.31.239"),8090);
    connect(&server,SIGNAL(readyRead()),this,SLOT(on_receive_data()));
}

MainWindow::~MainWindow()
{
    server.close();
    delete ui;
}

void MainWindow::on_receive_data()
{
    QByteArray databuf;
    ui->pushButton->setDisabled(false);
    quint16 port = 8090;
    server.readDatagram(databuf.data(),databuf.length(),&addr,&port);
    ui->textEdit->setText(databuf);
}

void MainWindow::on_pushButton_clicked()
{
    QString str = ui->lineEdit->text();
    server.writeDatagram(str.toUtf8(),addr,quint16(8090));
}
