/********************************************************************************
** Form generated from reading UI file 'ControlPanelView.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPANELVIEW_H
#define UI_CONTROLPANELVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlPanelView
{
public:

    void setupUi(QWidget *ControlPanelView)
    {
        if (ControlPanelView->objectName().isEmpty())
            ControlPanelView->setObjectName(QStringLiteral("ControlPanelView"));
        ControlPanelView->resize(400, 300);

        retranslateUi(ControlPanelView);

        QMetaObject::connectSlotsByName(ControlPanelView);
    } // setupUi

    void retranslateUi(QWidget *ControlPanelView)
    {
        ControlPanelView->setWindowTitle(QApplication::translate("ControlPanelView", "ControlPanelView", 0));
    } // retranslateUi

};

namespace Ui {
    class ControlPanelView: public Ui_ControlPanelView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPANELVIEW_H
