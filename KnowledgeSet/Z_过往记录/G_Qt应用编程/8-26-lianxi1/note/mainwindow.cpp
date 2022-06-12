#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pathname = "";

    connect(ui->actionQuit,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionCopy,SIGNAL(triggered(bool)),ui->textEdit,SLOT(copy()));
    connect(ui->actionCut,SIGNAL(triggered(bool)),ui->textEdit,SLOT(paste()));

    ui->actionLine_Wrap->setCheckable(true);
    ui->actionLine_Wrap->setChecked(true);

    label.setText("row 1, col 1");
    ui->statusBar->addPermanentWidget(&label);
    connect(ui->textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(on_actionPosision()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionCreate_triggered()
{
    if (ui->textEdit->document()->isModified()) {
        //context is modified now

        //show dialog box
        QMessageBox msgbox;
        msgbox.setText("the document has been modified now");
        msgbox.setInformativeText("do you wanna save now?");
        msgbox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard);
        msgbox.setDefaultButton(QMessageBox::Save);
        msgbox.show();
        int ret = msgbox.exec();
        switch (ret) {
        case QMessageBox::Save:
            //save and clear

            ui->textEdit->clear();
            break;
        case QMessageBox::Cancel:
            //do nothiong
            break;
        case QMessageBox::Discard:
            //clear
            ui->textEdit->clear();
            break;
        default:
            break;
        }
    } else {
        ui->textEdit->clear();
    }
    //base work was done



}

void MainWindow::on_actionOpen_triggered()
{
    if (ui->textEdit->document()->isModified()) {
        //context is modified now

        //show dialog box
        QMessageBox msgbox;
        msgbox.setText("the document has been modified now");
        msgbox.setInformativeText("do you wanna save now?");
        msgbox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard);
        msgbox.setDefaultButton(QMessageBox::Save);
        msgbox.show();
        int ret = msgbox.exec();
        switch (ret) {
        case QMessageBox::Save:
            //save and clear
            on_actionSave_As_triggered();
            ui->textEdit->clear();
            break;
        case QMessageBox::Cancel:
            //do nothiong
            break;
        case QMessageBox::Discard:
            //clear
            ui->textEdit->clear();
            break;
        default:
            break;
        }
    }

    //get g system dialog
    QString filename = QFileDialog::getOpenFileName(this,"open");
    if (filename.isEmpty()) {
        //QMessageBox::warning(this,"warning");
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        //QMessageBox::critical((this,"error","open erreor" + file.errorString()));
        return;
    }
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString str = in.readAll();
    file.close();
    ui->textEdit->setText(str);
}

void MainWindow::on_actionSave_As_triggered()
{
    //get g system dialog
    QString filename = QFileDialog::getSaveFileName(this,"save as");
    if (filename.isEmpty()) {
        //QMessageBox::warning(this,"warning");
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        //QMessageBox::critical((this,"error","open erreor" + file.errorString()));
        return;
    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
    ui->statusBar->showMessage("save success");
}

void MainWindow::on_actionDate_Time_triggered()
{
    QString dt = QDateTime::currentDateTime().toString();
    ui->textEdit->clear();
    ui->textEdit->setText(dt);
}

void MainWindow::on_actionLine_Wrap_triggered(bool checked)
{
    if (checked)
        ui->textEdit->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    else
        ui->textEdit->setWordWrapMode(QTextOption::NoWrap);
}

void MainWindow::on_actionFone_triggered()
{
    bool ok;
    //QFont font = QFontDialog::getFont(ok,QFont("Times",17),this,"Font");
    if (ok) {
    //    ui->textEdit->setFont(font);
    }
}

void MainWindow::on_actionColor_triggered()
{
    QColor color = QColorDialog::getColor(QColor(255,255,255),this,"Color");
    if (color.isValid())
        ui->textEdit->setTextColor(color);
}

void MainWindow::on_actionPosision()
{
    int i = ui->textEdit->textCursor().blockNumber();
    int j = ui->textEdit->textCursor().columnNumber();

    QString str = QString("raw %1 ,col %2").arg(i+1).arg(j+1);
    label.setText(str);
}
