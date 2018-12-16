#pragma once

/**
* Message logging functionality
*/

/**
* Prints error message
*/
void error(const char* fmt, ...);

/**
* Prints fatal error message and exits the program
*/
void fatal(const char* fmt, ...);