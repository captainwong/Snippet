#include "ControlPanelView.h"

ControlPanelView::ControlPanelView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setFixedSize(100, 450);
}

ControlPanelView::~ControlPanelView()
{
}
