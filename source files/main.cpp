#include <iostream>
#include <string>

#include "Controller.h"

void log(std::string message) {
	std::cout << message << std::endl;
}

int main() {
	Controller controller;

	controller.startMainLoop();

	return 0;
}