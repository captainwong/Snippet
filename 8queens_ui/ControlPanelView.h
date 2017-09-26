#pragma once

#include <QWidget>
#include "ui_ControlPanelView.h"

class ControlPanelView : public QWidget
{
	Q_OBJECT

public:
	ControlPanelView(QWidget *parent = Q_NULLPTR);
	~ControlPanelView();

private:
	Ui::ControlPanelView ui;
};
