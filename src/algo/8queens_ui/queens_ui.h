#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_queens_ui.h"
#include "View.h"


class queens_ui : public QMainWindow
{
	Q_OBJECT

public:
	queens_ui(QWidget *parent = Q_NULLPTR);



private:
	Ui::queens_uiClass ui;
	View* view_ = {};
};
