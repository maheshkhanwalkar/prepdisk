#include "data.hpp"
#include "util.hpp"

#include <iostream>
#include <fstream>

void handle_data(int argc, const char* argv[])
{
	if(argc < 3 || argc > 4) {
		std::cerr << "usage: prepdisk --data data.bin disk.img pos [optional size]\n";
		exit(-1);
	}

	std::string data(argv[0]);
	std::string disk(argv[1]);

	int pos = std::stoi(argv[2]);
	int size = get_size(data);

	if(size == -1) {
		std::cerr << "Cannot stat '" << data << "' ! Check permissions or existence\n";
		exit(-1);
	}

	int diskSize = get_size(disk);

	if(diskSize < pos) {
		std::cerr << "Specified position is larger than provided disk image\n";
		exit(-1);
	}

	if(argc == 4) {
		size = std::stoi(argv[3]);
	}

	char* dataBuf = new char[size];
	read_raw(data, dataBuf, size);

	commit_raw(disk, dataBuf, size, pos);
	std::cout << "Committed data to disk" << std::endl;

	delete[] dataBuf;
}
