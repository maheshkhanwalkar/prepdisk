#include <string>
#include <iostream>

#include "boot.hpp"
#include "util.hpp"

void handle_boot(int argc, const char* argv[])
{
	if(argc != 2) {
		std::cerr << "usage: prepdisk --boot ldr disk.img\n";
		exit(-1);
	}

	const std::string ldr(argv[0]);
	const std::string disk(argv[1]);

	const int BLOCK_SIZE = 512;

	char* ldrBuf = new char[BLOCK_SIZE];
	char* diskBuf = new char[BLOCK_SIZE];

	// Read the data
	read_raw(ldr, ldrBuf, BLOCK_SIZE);
	read_raw(disk, diskBuf, BLOCK_SIZE);

	// Create a disk copy
	char* diskCpy = new char[BLOCK_SIZE];
	std::copy(diskBuf, diskBuf + BLOCK_SIZE, diskCpy);

	// Merge the blocks together
	merge_blocks(diskCpy, ldrBuf, BLOCK_SIZE);

	// Integrity check: skip the boot signature (last two bytes)
	bool res = integrity_check(diskBuf, diskCpy, ldrBuf, BLOCK_SIZE - 2);

	if(!res)
	{
		std::cout << "Bootloader potentially causes partition corruption!\n";
		std::cout << "Do you want to continue? (y or n): ";

		std::string input;
		std::cin >> input;

		if(input != "y")
		{
			delete[] ldrBuf;
			delete[] diskBuf;
			delete[] diskCpy;

			exit(-1);
		}

	}

	// Commit changes to disk
	commit_raw(disk, diskCpy, BLOCK_SIZE, 0);
	std::cout << "Committed changes to disk" << std::endl;

	// Cleanup
	delete[] ldrBuf;
	delete[] diskBuf;
	delete[] diskCpy;
}
