#include "window.hpp"
#include <unistd.h>
#include <iostream>

int main(int argc, char** argv)
{
	indigo::window win({0, 0, 800, 600});
	win.title("DEMO");

	sleep(5);

	return 0;
}
