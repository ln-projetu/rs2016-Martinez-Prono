#ifndef HEADER_POSIX_UPSTAR_H
#define HEADER_POSIX_UPSTAR_H

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

#HEADER_POSIX_UPSTAR_H