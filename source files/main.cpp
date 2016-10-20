#include <iostream>
#include <string>
#include <functional>

#include "GlfwWindowView.h"

void log(std::string message) {
	std::cout << message << std::endl;
}

int main() {

	GlfwWindowView view = GlfwWindowView();
	log("created GlfwWindowView");

	// TODO: create PointModel
	// TODO: create Interactor
	// TODO: create Presenter

	bool shouldCloseWindow = false;

	std::function<void()> onDemandClosingOfWindow = [&shouldCloseWindow]()->void {
		shouldCloseWindow = true;
		log("called callback to stop main loop");
	};

	view.setOnDemandClosingOfWindowCallbackTo(onDemandClosingOfWindow);

	log("starting main loop");
	while (!shouldCloseWindow) {

		view.render();
	}

	log("stoped main loop");

	return 0;
}