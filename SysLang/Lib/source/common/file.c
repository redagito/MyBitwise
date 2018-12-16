#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "syl/common/file.h"

#include <stdio.h>
#include <stdlib.h>

#include "syl/common/memory.h"

char* file_read(const char* path)
{
	FILE* file = fopen(path, "rb");
	if (file == NULL)
	{
		return NULL;
	}

	// File size
	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	fseek(file, 0, SEEK_SET);
	if (length <= 0) 
	{
		return NULL;
	}

	// Read into newly allocated buffer
	// Buffer size including null terminator
	char* buffer = xmalloc(length + 1);
	if (fread(buffer, length, 1, file) != 1)
	{
		fclose(file);
		free(buffer);
		return NULL;
	}
	fclose(file);

	// Null terminator
	buffer[length] = 0;
	return buffer;
}

bool file_write(const char* path, const char* buffer, size_t length)
{
	FILE* file = fopen(path, "w");
	if (file == NULL)
	{
		return false;
	}

	size_t written = fwrite(buffer, length, 1, file);
	fclose(file);
	return written == 1;
}