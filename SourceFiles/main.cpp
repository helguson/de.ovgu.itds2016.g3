#include "Controller.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{

	Controller controller(argc, argv);
	return controller.startApplication();
}
