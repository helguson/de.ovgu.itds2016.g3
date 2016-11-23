#include "Controller.h"
#include "ThreeDTreeStructureBuilder.h" 
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	ThreeDTree::runTests();
	Controller controller(argc, argv);
	return app.exec();
}
