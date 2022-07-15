#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void Register::on_pushButton_2_clicked()
{
    QString username = ui->lineEdit->text();
    QString passwd = ui->lineEdit_2->text();
    QString passwd2 = ui->lineEdit_3->text();

    if (username.isEmpty() || passwd.isEmpty() || passwd2.isEmpty()) {
        QMessageBox::critical(this,"fail","your imformation lose");
        return;
    }

    if (passwd != passwd2) {
        QMessageBox::critical(this,"fail","your passwd is not same");
        return;
    }
    emit Sig_Register(username,passwd);

}

void Register::on_pushButton_clicked()
{
    this->hide();
    emit Sig_Cancel();
}
