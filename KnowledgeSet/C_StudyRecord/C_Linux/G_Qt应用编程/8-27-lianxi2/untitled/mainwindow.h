#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_connected();

    void on_readyread();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QTcpServer server;
    QTcpSocket *client;
};

#endif // MAINWINDOW_H
