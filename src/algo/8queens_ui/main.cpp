#include "queens_ui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	queens_ui w;
	w.show();
	return a.exec();
}
