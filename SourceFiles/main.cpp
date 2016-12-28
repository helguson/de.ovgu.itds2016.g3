#include "Controller.h"

#include "LineCommand.h"

int main(int argc, char *argv[])
{

	std::string s1 = "hello";
	std::string s2 = decodeFrom<std::string>(s2);

	int i = decodeFrom<int>(s1);
	double d = decodeFrom<double>(s1);

	process<>();
	//process<int, int>(s1, s1);
	//process<std::string, int, float>(s1, s1, s1);


	Controller controller(argc, argv);
	return controller.startApplication();
}
