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
 * @param filename Filename of this tarball.
 */
int read_tar(char* filename);

/**
 * Extract the tarball
 * @param filename Filename of the tarball.
 */
int extract_tar(char *filename);

/**
 * Decompress a GZ Archive
 * @param filename name of the GZ file
 */
int extract_tar_gz(char *filename);

/**
 * Read the contents of a entry in the tar file.
 * @param fd File descriptor of this tarball.
 * @param size_data Size of the entry to read.
 */
void read_data_block(int fd, int size_data);


char * uncompress_archive(char* filename);


#endif