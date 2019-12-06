#include "View.h"
#include <QLayout>

View::View(QWidget *parent)
	: QWidget(parent)
{
	//QVBoxLayout* vert = new QVBoxLayout(this);
	//vert->addStretch();

	QHBoxLayout *layout = new QHBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);
	setLayout(layout);
	//vert->addLayout(layout);

	chess_board_ = new ChessBoardView();
	control_panel_ = new ControlPanelView();

	layout->addWidget(chess_board_, 1);
	layout->addWidget(control_panel_, 0);

	layout->setSizeConstraint(QLayout::SetFixedSize);
}

View::~View()
{
}
