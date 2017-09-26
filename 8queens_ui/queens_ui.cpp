#include "queens_ui.h"
#include <QLayout>



queens_ui::queens_ui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	resize(550, 450);

	QVBoxLayout* vert = new QVBoxLayout(this);
	vert->addStretch();

	QHBoxLayout *layout = new QHBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);
	//setLayout(layout);
	vert->addLayout(layout);

	chess_board_ = new ChessBoardView(this);
	control_panel_ = new ControlPanelView(this);

	layout->addWidget(chess_board_);
	layout->addWidget(control_panel_);
}
