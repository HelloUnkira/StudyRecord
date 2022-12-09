#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QString>
#include <QDateTime>
#include <QFontDialog>
#include <QColorDialog>
#include <QFont>
#include <QColor>
#include <QLabel>

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
    void on_actionCreate_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_As_triggered();

    void on_actionDate_Time_triggered();

    void on_actionLine_Wrap_triggered(bool checked);

    void on_actionFone_triggered();

    void on_actionColor_triggered();

    void  on_actionPosision();



private:
    Ui::MainWindow *ui;
    QString pathname;
    QLabel label;
};

#endif // MAINWINDOW_H
