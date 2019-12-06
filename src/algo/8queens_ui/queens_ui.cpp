#include "queens_ui.h"




queens_ui::queens_ui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//resize(550, 450);

	view_ = new View(this);
	setCentralWidget(view_);
	setFixedSize(550, 450);
}
