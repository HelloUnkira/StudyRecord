#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QString>
#include <QByteArray>
#include <QDebug>

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

    void on_receive_data();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QUdpSocket server;
    QHostAddress addr;
};

#endif // MAINWINDOW_H
