#pragma once

#include <QWidget>
#include "ui_ChessBoardView.h"

class ChessBoardView : public QWidget
{
	Q_OBJECT

public:
	ChessBoardView(QWidget *parent = Q_NULLPTR);
	~ChessBoardView();

protected:
	virtual void paintEvent(QPaintEvent* e) override;

private:
	Ui::ChessBoardView ui;
};
