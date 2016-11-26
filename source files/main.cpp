#include "Controller.h"

#include "Interval.h"
#include "Interval3d.h"
#include "ThreeDTreeStructureBuilder.h"
#include "ThreeDTree.h"


int main() {

	//Interval::runTest();
	//Interval3d::runTest();
	//ThreeDTreeStructureBuilder::runTest();
	ThreeDTree::runTests();
	
	Controller controller;
	controller.startMainLoop();

	return 0;
}