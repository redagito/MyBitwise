#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "path.h"

/**
* Directory list iterator
*/
typedef struct dir_list_iter_t
{
	bool valid;
	bool error;
	
	char base[MAX_PATH];
	char name[MAX_PATH];
	size_t size;
	bool is_dir;

	void* handle;
} dir_list_iter_t;

/**
* Free iterator resources
*/
void dir_list_free(dir_list_iter_t* iter);

void dir_list_next(dir_list_iter_t* iter);

/**
* Initialize directory iterator for provided path
*/
void dir_list(dir_list_iter_t* iter, const char* path);

/**
* True if the directory should be excluded/skipped when iterating
*/
bool dir_excluded(dir_list_iter_t* iter);

bool dir_list_subdir(dir_list_iter_t* iter);

const char** dir_list_buffer(const char* filespec);