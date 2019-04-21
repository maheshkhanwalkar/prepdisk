#include <iostream>
#include <fstream>

#include <string>
#include <iterator>
#include <algorithm>

#include <sys/stat.h>

// Size of MBR region
const int BLOCK_SIZE = 512;

/**
 * Read the raw binary data from a file
 * @param file - file to read from
 * @param buffer - buffer to read into
 * @param size - number of bytes to read (hard limit)
 */
void read_raw(const std::string& file, char* buffer, int size)
{
	struct stat res = {};

	if(stat(file.c_str(), &res) == 0)
	{
		if(res.st_size < size)
		{
			std::cerr << "'" << file << "' is too small. Expected size: >= "
				<< size << " bytes.\n";

			exit(-1);
		}
	}
	else
	{
		std::cerr << "Cannot stat '" << file
			<< "' ! Check file permissions or existence\n";

		exit(-1);
	}

	std::ifstream stream(file, std::ios::in | std::ios::binary);

	stream.read(buffer, size);
	stream.close();
}

/**
 * Perform bitwise OR of 'src' and 'dest', storing into dest
 * @param dest - source #2 + destination
 * @param src  - source #1
 * @param size - size of the array
 */
void or_data(char* dest, const char* src, int size)
{
	for(int i = 0; i < size; i++)
		dest[i] |= src[i];
}

/**
 * Check whether (orig ^ modified) == other
 * @param orig - original data
 * @param modified - modified data (result of or_data)
 * @param other - other data
 * @param size - size of the array
 * @return true, if there was no "corruption"
 */
bool do_check(const char* orig, const char* modified,
		const char* other, int size)
{
	for(int i = 0; i < size; i++)
	{
		bool res = (orig[i] ^ modified[i]) == other[i];

		if(!res) {
			return false;
		}
	}

	return true;
}

/**
 * Commits the data to the beginning of the file (leaving everything else intact)
 * @param file - file to modify
 * @param buffer - buffer to commit
 * @param size - size of buffer
 */
void commit_raw(const std::string& file, const char* buffer, int size)
{
	std::fstream stream(file, std::ios::in | std::ios::out | std::ios::binary);

	stream.write(buffer, size);
	stream.flush();

	stream.close();
}

int main(int argc, const char* argv[])
{
	if(argc != 3)
	{
		std::cerr << "Error. Invalid argument format\n";
		return -1;
	}

	const std::string ldr(argv[1]);
	const std::string disk(argv[2]);

	char* ldrBuf = new char[BLOCK_SIZE];
	char* diskBuf = new char[BLOCK_SIZE];

	// Read the data from disk
	read_raw(ldr, ldrBuf, BLOCK_SIZE);
	read_raw(disk, diskBuf, BLOCK_SIZE);

	// Create a disk copy
	char* diskCpy = new char[BLOCK_SIZE];
	std::copy(diskBuf, diskBuf + BLOCK_SIZE, diskCpy);

	// OR the data together
	or_data(diskCpy, ldrBuf, BLOCK_SIZE);

	// Integrity check: skip the boot signature (last two bytes)
	bool res = do_check(diskBuf, diskCpy, ldrBuf, BLOCK_SIZE - 2);

	if(!res)
	{
		std::cout << "Bootloader potentially causes partition corruption!\n";
		std::cout << "Do you want to continue? (y or n): ";

		std::string input;
		std::cin >> input;

		if(input != "y")
			return -1;
	}

	// Commit changes to disk
	commit_raw(disk, diskCpy, BLOCK_SIZE);
	return 0;
}
