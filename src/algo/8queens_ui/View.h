#pragma once

#include <QWidget>
#include "ChessBoardView.h"
#include "ControlPanelView.h"

class View : public QWidget
{
	Q_OBJECT

public:
	View(QWidget *parent);
	~View();

private:
	ChessBoardView* chess_board_ = {};
	ControlPanelView* control_panel_ = {};
};
