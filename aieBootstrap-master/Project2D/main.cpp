#include "Application2D.h"

int main() {
	
	// allocation
	auto app = new Application2D();

	// initialise and loop
	/*app->run("AIE", 1280, 720, false);*/
	app->run("AIE", 1600, 1000, false);

	// deallocation
	delete app;

	return 0;
}