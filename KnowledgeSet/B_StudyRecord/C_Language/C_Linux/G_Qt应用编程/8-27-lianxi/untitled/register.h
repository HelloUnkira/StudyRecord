#ifndef REGISTER_H
#define REGISTER_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>

namespace Ui {
class Register;
}

class Register : public QMainWindow
{
    Q_OBJECT

signals:
    void Sig_Register(QString,QString);
    void Sig_Cancel();

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
