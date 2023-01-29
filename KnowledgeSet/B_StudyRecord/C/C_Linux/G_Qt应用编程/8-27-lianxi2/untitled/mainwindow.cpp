#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    ui->pushButton->setDisabled(true);
    server.listen(QHostAddress::Any,8088);

    connect(&server,SIGNAL(newConnection()),this,SLOT(on_connected()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connected()
{
    client = server.nextPendingConnection();
    ui->pushButton->setDisabled(false);
    //ui->label->setText("IP: " + client->peerAddress().toString());
    //int t = client->peerPort();
    //std::string port = " " + t;
    //change fail
    //ui->label_2->setText("PORT:  " + QString::fromStdString(port));
    connect(client,SIGNAL(readyRead()),this,SLOT(on_readyread()));
}

void MainWindow::on_readyread()
{
    ui->textEdit->append(client->readAll());
}

void MainWindow::on_pushButton_clicked()
{
    QString str = ui->lineEdit->text();
    client->write(str.toStdString().data());
    ui->lineEdit->clear();
}
