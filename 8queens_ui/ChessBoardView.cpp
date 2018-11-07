#include "ChessBoardView.h"
#include <QEvent>
#include <QPainter>
#include <QColor>

ChessBoardView::ChessBoardView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setFixedSize(450, 450);

}

ChessBoardView::~ChessBoardView()
{
}


void ChessBoardView::paintEvent(QPaintEvent * e)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	bool is_black = false;
	QColor white(Qt::white);
	QColor black(Qt::black);

	painter.setPen(QPen(white, 1));

	for (int row = 0; row < 8; row++) {
		bool last_is_black = is_black;
		for (int col = 0; col < 8; col++) {
			painter.setBrush(last_is_black ? black : white);
			int x = col * 50;
			int y = row * 50;
			painter.drawRect(x, y, 50, 50);
			last_is_black = !last_is_black;
		}

		is_black = !is_black;
	}
}

