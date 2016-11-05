#ifndef PTAR_H
#define PTAR_H

#include "header_posix_ustar.h"


/**
 * Opens the tarball.
 * @param filename The tarball filename.
 */
int open_tar(char* filename);

/**
 * Read a tar file and list its contents and displays useful informations.
 * @param fd File descriptor of this tarball.
 */
int read_tar_file(int fd);

/**
 * Read the contents of a entry in the tar file.
 * @param fd File descriptor of this tarball.
 * @param size_data Size of the entry to read.
 */
void read_data_block(int fd, int size_data);

/**
 * Extract the tarball
 * @param filename Filename of the tarball.
 */
int extract_tar(char *filename);

/**
 * calls the extract_X() method depending on the typeflag
 */
void extract_entry(int fd, header_posix_ustar *header);

/**
 * Extract a regular file in the tarball.
 * @param header The header of the regular file.
 */
void extract_regular_file(int fd, header_posix_ustar *header);

/**
 * Extract the directory in the tarball. More precisly,
 * this method create only the directory.
 */
void extract_directory(int fd, header_posix_ustar *header);


#endif