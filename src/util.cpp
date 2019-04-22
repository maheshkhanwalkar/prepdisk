#include "util.hpp"
#include <sys/stat.h>

#include <iostream>
#include <fstream>

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

void merge_blocks(char* dest, const char* src, int size)
{
	for(int i = 0; i < size; i++)
		dest[i] |= src[i];
}

bool integrity_check(const char* orig, const char* modified,
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

void commit_raw(const std::string& file, const char* buffer, int size, int pos)
{
	std::fstream stream(file, std::ios::in | std::ios::out | std::ios::binary);
	stream.seekp(pos);

	stream.write(buffer, size);
	stream.flush();

	stream.close();
}