#pragma once
#include <string>

/**
 * Read the raw binary data from a file
 * @param file - file to read from
 * @param buffer - buffer to read into
 * @param size - number of bytes to read (hard limit)
 */
void read_raw(const std::string& file, char* buffer, int size);


/**
 * Perform bitwise OR of 'src' and 'dest', storing into dest
 * @param dest - source #2 + destination
 * @param src  - source #1
 * @param size - size of the array
 */
void merge_blocks(char* dest, const char* src, int size);


/**
 * Check whether (orig ^ modified) == other
 * @param orig - original data
 * @param modified - modified data (result of or_data)
 * @param other - other data
 * @param size - size of the array
 * @return true, if there was no "corruption"
 */
bool integrity_check(const char* orig, const char* modified,
			  const char* other, int size);

/**
 * Commits the data to the file (leaving everything else intact)
 * @param file - file to modify
 * @param buffer - buffer to commit
 * @param size - size of buffer
 * @param pos - position to seek to
 */
void commit_raw(const std::string& file, const char* buffer,
		int size, int pos);
