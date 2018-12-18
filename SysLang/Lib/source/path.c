#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "syl/path.h"

#include <string.h>
#include <stdio.h>

// OS specific includes
#ifdef _WIN32
  #include <io.h>
  #include <errno.h>
#else
  #include <dirent.h>
#endif

void path_normalize(char* path)
{
	char* curr = path;
	for (curr = path; *curr != 0; ++curr)
	{
		// Replace backslashes
		if (*curr == '\\')
		{
			*curr = '/';
		}
	}
	// Removes separator at end
	// TODO Why not just leave the separator at the end
	// so we know its a path without a file?
	if (curr != path && curr[-1] == '/')
	{
		curr[-1] = 0;
	}
}

void path_copy(char path[MAX_PATH], const char* src)
{
	strncpy(path, src, MAX_PATH);
	path[MAX_PATH - 1] = 0;
	path_normalize(path);
}

void path_join(char path[MAX_PATH], const char* src)
{
	// End of path
	char* curr = path + strlen(path);

	// Move to separator
	if (curr != path && curr[-1] == '/')
	{
		--curr;
	}

	// Move after separator
	if (*src == '/' || *src == '\\')
	{
		++src;
	}

	// Append separator and src
	snprintf(curr, path + MAX_PATH - curr, "/%s", src);
	path_normalize(path);
}

const char* path_file(const char* path)
{
	// Start at end of path, move backwards and search for '/'
	for (const char* curr = path + strlen(path); curr != path; --curr)
	{
		// Might not be normalized path
		if (curr[-1] == '/' || curr[-1] == '\\')
		{
			// Found separator
			return curr;
		}
	}
	return path;
}

const char* path_ext(const char path[MAX_PATH])
{
	// Start at end of path, move backwards and search for '.'
	for (const char* curr = path + strlen(path); curr != path; --curr)
	{
		if (curr[-1] == '.')
		{
			// Found extension
			return curr;
		}
		else if (curr[-1] == '/')
		{
			// Found separator, no extension found
			return path;
		}
	}
	return path;
}

void path_absolute(char path[MAX_PATH])
{
	char rel_path[MAX_PATH];
	path_copy(rel_path, path);
#ifdef _WIN32
	_fullpath(path, rel_path, MAX_PATH);
#else
	realpath(rel_path, path);
#endif
	path_normalize(path);
}