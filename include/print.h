#ifndef PRINT_H
#define PRINT_H

#include "header_posix_ustar.h"


/**
 * tiny method which print information about the current entry.
 * Prints only of the -l flag is enabled.
 */
void print_results(header_posix_ustar *header);


#endif