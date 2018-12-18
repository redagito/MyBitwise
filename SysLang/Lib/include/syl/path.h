#pragma once

#include <stdlib.h>

#ifndef MAX_PATH
  #ifdef _MAX_PATH
    #define MAX_PATH _MAX_PATH
  #elif defined PATH_MAX
    #define MAX_PATH PATH_MAX
  #else
    #error "No suitable MAX_PATH surrogate"
  #endif
#endif

/**
* File path handling
*/

/**
* Normalizes path delimiters
*/
void path_normalize(char* path);

/**
* Copies to path from source.
* Truncates length to MAX_PATH and normalizes path
*/
void path_copy(char path[MAX_PATH], const char* src);

/**
* Joins src to end of path
*/
void path_join(char path[MAX_PATH], const char* src);

/**
* Returns pointer the file name in path
* Path may not be normalized
*/
const char* path_file(const char* path);

/**
* Returns pointer to file extension in path
*/
const char* path_ext(const char path[MAX_PATH]);

/**
* Creates absolute path
*/
void path_absolute(char path[MAX_PATH]);