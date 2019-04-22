#include <iostream>

#include "boot.hpp"
#include "data.hpp"

int main(int argc, const char* argv[])
{
	if(argc == 1)
	{
		std::cerr << "Error. Invalid argument format\n";
		return -1;
	}

	std::string top(argv[1]);

	if(top == "--boot") {
		handle_boot(argc - 2, argv + 2);
		return 0;
	}

	if(top == "--data") {
		handle_data(argc - 2, argv + 2);
		return 0;
	}

	std::cerr << "Error. Unknown argument\n";
	return -1;
}
