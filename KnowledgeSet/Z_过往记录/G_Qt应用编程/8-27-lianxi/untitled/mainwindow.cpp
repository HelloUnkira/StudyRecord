#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("untitled.db");

//    QSQLITE  don't need setting these paraments
//    db.setHostName("admin");
//    db.setUserName("haha");
//    db.setPassword("123456");
    bool ok = db.open();
    if (!ok) {
        qDebug()<<"Sqlite open failed";
        return;
    }

    QSqlQuery query;
    //other operation as same like this
    ok = query.exec("create table user(username text primary key,password text not null);");
    if (!ok) {
        qDebug()<<"create table failed";
    }

    query.exec("insert into user(username,password) values('111','111')");
    if (query.next())
        qDebug() << query.value(0)<<" "<<query.value(1);

    connect(&r,SIGNAL(Sig_Register(QString,QString)),this,SLOT(on_sig_register(QString,QString)));
    connect(&r,SIGNAL(Sig_Cancel()),this,SLOT(on_sig_cancel()));

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    QString passwd = ui->lineEdit_2->text();

    //
    QSqlQuery query;
    //about search operation
    query.exec("select * from user;");
    //if the return is not null,existing result set,using next()
    for (int rowNum=0; query.next(); rowNum++) {
        QString u_name = query.value(0).toString();
        QString u_passwd = query.value(1).toString();
        if (name == u_name && u_passwd == passwd)
        {
                this->close();
                //other window show()
                qDebug()<<"you already log in now";
                return;
        }
    }
    QMessageBox::critical(this,"login fail","your operation is fail");
}

void MainWindow::on_pushButton_2_clicked()
{
    this->hide();
    //other window show()
    r.show();
}

void MainWindow::on_sig_register(QString name, QString passwd)
{
    QString sql = QString("insert into user(username,password) values('%1','%2');").arg(name).arg(passwd);
    //qDebug()<<sql;
    QSqlQuery query;
    bool ok = query.exec(sql);
    if (ok)
        QMessageBox::critical(this,"finish","your register is finished");
    else
        QMessageBox::critical(this,"fail","your register is fail");
}

void MainWindow::on_sig_cancel()
{
    this->show();
}



