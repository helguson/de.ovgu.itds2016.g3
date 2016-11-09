#include "Controller.h"

#include "Interval.h"
#include "ThreeDTreeStructureBuilder.h"


int main() {

	//Interval::runTest();
	ThreeDTreeStructureBuilder::runTest();
	
	Controller controller;
	controller.startMainLoop();

	return 0;
}