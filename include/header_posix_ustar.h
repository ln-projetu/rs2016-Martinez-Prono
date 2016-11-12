#ifndef HEADER_POSIX_UPSTAR_H
#define HEADER_POSIX_UPSTAR_H

#include <sys/stat.h>
#include "definitions.h"
#include "utils.h"
#define USTAR "ustar\x00"
#define VERSION "00"
#define REGFILE '0'
#define DIR '5'


/**
 * Structure for the POSIX ustar archive header.
 *
 *	@author Lucas MARTINEZ
 *	@author Yann PRONO
 */


typedef struct header_posix_ustar {
	char name[100];			// File name 			trailing NUL
	char mode[8];			// File mode 			octal number in ASCII
	char uid[8];			// User id, 			octal number in ASCII
	char gid[8];			// Group id,   			octal number in ASCII
	char size[12];			// Size of file			octal number in ASCII
	char mtime[12];			// Modification time	octal number in ASCII
	char checksum[8];		// Checksum 			octal number in ASCII
	char typeflag;		  	// Type of entry		octal number in ASCII
	char linkname[100];		//
	char magic[6];			// "ustar"				Followed by NUL byte
	char version[2];		// "00"					ASCII digit zero
	char uname[32];			// User name  			Null-terminated ASCII strings
	char gname[32];			// Group name			Null-terminated ASCII strings
	char devmajor[8];		// Device major number
	char devminor[8];		// Device minor number
	char prefix[155];		// Prefix if filename to long
	char pad[12];			// ???
} header_posix_ustar;


/**
 * @return The filename represented by this header.
 */
char* get_name(header_posix_ustar *header);

char* print_as_list(header_posix_ustar *header);

header_posix_ustar* create_header();

/**
 * @return the typeflag of the header
 * @TODO we manage only regular files and directory for the moment...
 */
char get_type(header_posix_ustar *header);

/**
 * @return Permissions of the file (octal representation).
 */
mode_t get_mode(header_posix_ustar *header);

/**
 * @return The user ID of this file (decimal).
 */
uid_t get_uid(header_posix_ustar *header);

/**
 * @return The group ID of this file (decimal).
 */
gid_t get_gid(header_posix_ustar *header);

/**
 * @return the size of the file in bytes.
 */
int get_size(header_posix_ustar *header);

/**
 * @return the modification time of the file.
 */
time_t get_mtime(header_posix_ustar *header);

/**
 * @return the checksum of the file.
 */
int get_checksum(header_posix_ustar *header);

/**
 * @return
 */
char* get_linkname(header_posix_ustar *header);

/**
 * @return 0 if the tar is POSIX ustar standard archive.
 */
int is_posix_ustar(header_posix_ustar *header);

/**
 * Be careful, due to the structure of the header,
 * calling this method will also return you the following field (the uname).
 * So you have to get only the two first characters.
 *
 * @return The version of the tar header.
 *
 */
char* get_version(header_posix_ustar *header);

/**
 * @return The user name of the file.
 */
char* get_uname(header_posix_ustar *header);

/**
 * @return The group name of the file
 */
char* get_gname(header_posix_ustar *header);

/**
 * Writes in the STDOUT all information about the tar header
 * @param header The tar header.
 */
void display_header(header_posix_ustar *header);

/**
 * Method not TESTED !
 * @return 1 if the given block is full of zeros bytes.
 */
int is_empty(header_posix_ustar *header);

/**
 * @return 1 if this header represents a regular file => '0'
 */
int is_regular_file(header_posix_ustar *header);

/**
 * @return 1 if the given header represents a directory => '5'
 */
int is_directory(header_posix_ustar *header);


#endif