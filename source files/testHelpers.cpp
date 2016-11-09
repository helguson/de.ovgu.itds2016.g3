#include "testHelpers.h"

#include <iostream>

void log(std::string message) {
	std::cout << message << std::endl;
}

std::string toStr(bool value) {
	std::string result;

	if (value == true) {
		result = "true";
	}
	else {
		result = "false";
	}
	return result;
}