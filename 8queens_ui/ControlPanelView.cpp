#include "ControlPanelView.h"

ControlPanelView::ControlPanelView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	resize(100, 450);
}

ControlPanelView::~ControlPanelView()
{
}
