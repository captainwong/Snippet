/********************************************************************************
** Form generated from reading UI file 'queens_ui.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUEENS_UI_H
#define UI_QUEENS_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_queens_uiClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *queens_uiClass)
    {
        if (queens_uiClass->objectName().isEmpty())
            queens_uiClass->setObjectName(QStringLiteral("queens_uiClass"));
        queens_uiClass->resize(600, 400);
        menuBar = new QMenuBar(queens_uiClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        queens_uiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(queens_uiClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        queens_uiClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(queens_uiClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        queens_uiClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(queens_uiClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        queens_uiClass->setStatusBar(statusBar);

        retranslateUi(queens_uiClass);

        QMetaObject::connectSlotsByName(queens_uiClass);
    } // setupUi

    void retranslateUi(QMainWindow *queens_uiClass)
    {
        queens_uiClass->setWindowTitle(QApplication::translate("queens_uiClass", "queens_ui", nullptr));
    } // retranslateUi

};

namespace Ui {
    class queens_uiClass: public Ui_queens_uiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUEENS_UI_H
