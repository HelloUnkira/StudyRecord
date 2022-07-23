#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    timer.setInterval(overtime);
    connect(&timer,SIGNAL(timeout()),this,SLOT(timer_Event()));

    timer.start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timer_Event()
{
    qDebug()<<"haha";
}
