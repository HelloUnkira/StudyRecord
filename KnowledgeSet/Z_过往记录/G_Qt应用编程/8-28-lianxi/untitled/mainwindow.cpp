#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"mouse double click event:";
    if (event->button() == Qt::LeftButton)
              qDebug()<<"left button";
    else if (event->button()==Qt::RightButton)
            qDebug()<<"right button";
    else if (event->button()==Qt::MidButton)
            qDebug()<<"middle button";
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<"mouse move event";
//    QPoint point = event->pos();
//    qDebug()<<"x pos :"<<QString::number(event->x());
//    qDebug()<<"y pos :"<<QString::number(event->y());
//    qDebug()<<point;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
//    qDebug()<<"mouse press click event:";
//    if (event->button() == Qt::LeftButton)
//              qDebug()<<"left button";
//    else if (event->button()==Qt::RightButton)
//            qDebug()<<"right button";
//    else if (event->button()==Qt::MidButton)
//            qDebug()<<"middle button";
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
//    qDebug()<<"mouse release click event:";
//    if (event->button() == Qt::LeftButton)
//              qDebug()<<"left button";
//    else if (event->button()==Qt::RightButton)
//            qDebug()<<"right button";
//    else if (event->button()==Qt::MidButton)
    //            qDebug()<<"middle button";
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    qDebug()<<"wheel event";
     if (event->delta() > 0)
         qDebug()<<"wheel up roll";
     else
         qDebug()<<"wheel down roll";
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"key press event";
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    qDebug()<<"key release event";
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    qDebug()<<"paint event";
}


