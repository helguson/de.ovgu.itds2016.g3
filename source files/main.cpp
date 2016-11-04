#include "Controller.h"
#include "Interval.h"

int main() {
	
	Interval::runTest();
	
	Controller controller;
	controller.startMainLoop();

	return 0;
}