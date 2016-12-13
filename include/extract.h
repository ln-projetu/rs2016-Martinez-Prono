#ifndef EXTRACT_H
#define EXTRACT_H

#include "w_info.h"


/**
 * calls the extract_X() method depending on the typeflag
 */
void *extract_entry(void *args);
void *extract_entry_nop(void *args);

/**
 * Change the last modified date of a file
 * @param name Filename
 * @param mtime mtime structure with date.
 */
void change_date_file(char* name, time_t mtime);

/**
 * Extract a regular file in the tarball.
 * @param header The header of the regular file.
 */
void extract_regular_file(w_info* info);

/**
 * Extract the directory in the tarball. More precisly,
 * this method create only the directory.
 */
void extract_directory(w_info* info);

/**
 * Extract the symbolic link.
 */
void extract_symblink(w_info* info);


#endif