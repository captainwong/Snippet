/********************************************************************************
** Form generated from reading UI file 'ChessBoardView.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHESSBOARDVIEW_H
#define UI_CHESSBOARDVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChessBoardView
{
public:

    void setupUi(QWidget *ChessBoardView)
    {
        if (ChessBoardView->objectName().isEmpty())
            ChessBoardView->setObjectName(QStringLiteral("ChessBoardView"));
        ChessBoardView->resize(400, 300);

        retranslateUi(ChessBoardView);

        QMetaObject::connectSlotsByName(ChessBoardView);
    } // setupUi

    void retranslateUi(QWidget *ChessBoardView)
    {
        ChessBoardView->setWindowTitle(QApplication::translate("ChessBoardView", "ChessBoardView", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChessBoardView: public Ui_ChessBoardView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHESSBOARDVIEW_H
