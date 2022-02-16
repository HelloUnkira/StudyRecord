/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCreate;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionQuit;
    QAction *actionCopy;
    QAction *actionCut;
    QAction *actionSearch;
    QAction *actionDate_Time;
    QAction *actionLine_Wrap;
    QAction *actionFone;
    QAction *actionColor;
    QAction *actionAbout;
    QAction *actionStatus_Bar;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QVBoxLayout *verticalLayout_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuFormat;
    QMenu *menuCheck;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 300);
        actionCreate = new QAction(MainWindow);
        actionCreate->setObjectName(QStringLiteral("actionCreate"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QStringLiteral("actionCut"));
        actionSearch = new QAction(MainWindow);
        actionSearch->setObjectName(QStringLiteral("actionSearch"));
        actionDate_Time = new QAction(MainWindow);
        actionDate_Time->setObjectName(QStringLiteral("actionDate_Time"));
        actionLine_Wrap = new QAction(MainWindow);
        actionLine_Wrap->setObjectName(QStringLiteral("actionLine_Wrap"));
        actionFone = new QAction(MainWindow);
        actionFone->setObjectName(QStringLiteral("actionFone"));
        actionColor = new QAction(MainWindow);
        actionColor->setObjectName(QStringLiteral("actionColor"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionStatus_Bar = new QAction(MainWindow);
        actionStatus_Bar->setObjectName(QStringLiteral("actionStatus_Bar"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        verticalLayout->addWidget(textEdit);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));

        verticalLayout->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuFormat = new QMenu(menuBar);
        menuFormat->setObjectName(QStringLiteral("menuFormat"));
        menuCheck = new QMenu(menuBar);
        menuCheck->setObjectName(QStringLiteral("menuCheck"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuFormat->menuAction());
        menuBar->addAction(menuCheck->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionCreate);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionQuit);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionSearch);
        menuEdit->addAction(actionDate_Time);
        menuFormat->addAction(actionLine_Wrap);
        menuFormat->addAction(actionFone);
        menuFormat->addAction(actionColor);
        menuCheck->addAction(actionStatus_Bar);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionCreate->setText(QApplication::translate("MainWindow", "Create(&C)", 0));
        actionCreate->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open(&I)", 0));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save(&S)", 0));
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit(&Q)", 0));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", 0));
        actionCut->setText(QApplication::translate("MainWindow", "Cut", 0));
        actionSearch->setText(QApplication::translate("MainWindow", "Search", 0));
        actionDate_Time->setText(QApplication::translate("MainWindow", "Date&Time", 0));
        actionLine_Wrap->setText(QApplication::translate("MainWindow", "Line Wrap", 0));
        actionFone->setText(QApplication::translate("MainWindow", "Fone", 0));
        actionColor->setText(QApplication::translate("MainWindow", "Color", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionStatus_Bar->setText(QApplication::translate("MainWindow", "Status Bar", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File(&F)", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        menuFormat->setTitle(QApplication::translate("MainWindow", "Format", 0));
        menuCheck->setTitle(QApplication::translate("MainWindow", "Check", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
