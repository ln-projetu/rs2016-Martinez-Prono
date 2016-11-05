#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header_posix_ustar.h"
#include "block.h"
#define OUTPUT_SEPARATOR "->"


char* get_name(header_posix_ustar *header) {
	return header->name;
}

char get_type(header_posix_ustar *header){
	return header->typeflag;
};

char* get_mode(header_posix_ustar *header) {
	return header->mode;
}

int get_uid(header_posix_ustar *header) {
	return octal_to_integer(atoi(header->uid));
}

int get_gid(header_posix_ustar *header) {
	return octal_to_integer(atoi(header->gid));
}

int get_size(header_posix_ustar *header) {
	return octal_to_integer(atoi(header->size));
}

int get_mtime(header_posix_ustar *header) {
	return octal_to_integer(atoi(header->mtime));
}

int get_checksum(header_posix_ustar *header) {
	return octal_to_integer(atoi(header->checksum));
}

char* get_linkname(header_posix_ustar *header) {
	return header->linkname;
}

int is_posix_ustar(header_posix_ustar *header) {
	return strcmp(header->magic, USTAR) == 0;
}

char* get_version(header_posix_ustar *header) {
	return header->version;
}

char* get_uname(header_posix_ustar *header) {
	return header->uname;
}

char* get_gname(header_posix_ustar *header) {
	return header->gname;
}

int is_empty(header_posix_ustar *header) {
	if(header->name[0] == '\0')
		return 1;
	else
		return 0;
}

int is_regular_file(header_posix_ustar *header) {
	return get_type(header) == REGFILE;
}

int is_directory(header_posix_ustar *header) {
	return get_type(header) == DIR;
}

void display_header(header_posix_ustar *header) {
	printf("%s\n", get_name(header));
	printf(" - type %s %c\n", OUTPUT_SEPARATOR,  get_type(header));
	printf(" - mode %s %s\n", OUTPUT_SEPARATOR,  get_mode(header));
	printf(" - uid %s %d\n", OUTPUT_SEPARATOR,  get_uid(header));
	printf(" - gid %s %d\n", OUTPUT_SEPARATOR,  get_gid(header));
	printf(" - size %s %d bytes\n", OUTPUT_SEPARATOR,  get_size(header));
	printf(" - mtime %s %d seconds since the start of epoch (1970)\n", OUTPUT_SEPARATOR, get_mtime(header));
	printf(" - checksum %s %d\n", OUTPUT_SEPARATOR, get_checksum(header));
	printf(" - linkname %s %s\n", OUTPUT_SEPARATOR,  get_linkname(header));
	printf(" - is USTAR? %s %d\n", OUTPUT_SEPARATOR,  is_posix_ustar(header));
	printf(" - version %s %c%c\n", OUTPUT_SEPARATOR, get_version(header)[0], get_version(header)[1]);
	printf(" - uname %s %s\n", OUTPUT_SEPARATOR, get_uname(header));
	printf(" - gname %s %s\n\n", OUTPUT_SEPARATOR,  get_gname(header));
}
