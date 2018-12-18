#pragma once

/**
* Message logging functionality
*/

/**
* Prints error message
*/
void log_warning(const char* fmt, ...);

/**
* Prints error message
*/
void log_error(const char* fmt, ...);

/**
* Prints fatal error message and exits the program
*/
void log_fatal(const char* fmt, ...);