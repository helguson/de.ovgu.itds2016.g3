#include "Controller.h"

#include "Interval.h"
#include "ThreeDTreeStructureBuilder.h"
#include "ThreeDTree.h"


int main() {

	//Interval::runTest();
	//ThreeDTreeStructureBuilder::runTest();
	//ThreeDTree::runTests();
	
	Controller controller;
	controller.startMainLoop();

	return 0;
}