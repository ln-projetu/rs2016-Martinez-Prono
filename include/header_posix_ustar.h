#ifndef HEADER_POSIX_UPSTAR_H
#define HEADER_POSIX_UPSTAR_H

#define USTAR "ustar\x00"

/**
 * Structure for the POSIX ustar archive header.
 * 
 *	@author Lucas MARTINEZ
 *	@author Yann PRONO
 *
 */

typedef struct header_posix_ustar {
	char name[100];			// File name 			trailing NUL
	char mode[8];			// File mode 			octal number in ASCII
	char uid[8];			// User id, 			octal number in ASCII
	char gid[8];			// Group id,   			octal number in ASCII
	char size[12];			// Size of file			octal number in ASCII
	char mtime[12];			// Modification time	octal number in ASCII
	char checksum[8];		// Checksum 			octal number in ASCII
	char typeflag[1];		// Type of entry		octal number in ASCII
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
 * @param tar_header The tar header.
 * @return The filename of the tar archive.
 */
char* get_name(header_posix_ustar *tar_header);

/**
 * @param tar_header The tar header.
 * @return Permissions of the tar archive (octal representation).
 */
char* get_mode(header_posix_ustar *tar_header);

int get_size(header_posix_ustar *tar_header);

int is_ustar(header_posix_ustar *tar_header);

/**
 * @param tar_header The tar header.
 * @return The user name of the tar archive.
 */
char* get_uname(header_posix_ustar *tar_header);

/**
 * @param tar_header The tar header.
 * @return The group name of the tar archive.
 */
char* get_gname(header_posix_ustar *tar_header);


/**
 * Writes in the STDOUT all information about the tar header
 * @param tar_header The tar header.
 */
void display_header(header_posix_ustar *tar_header);

#endif

