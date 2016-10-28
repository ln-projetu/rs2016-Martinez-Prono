#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header_posix_ustar.h"
#include "block.h"
#define OUTPUT_SEPARATOR "->"

char* get_name(header_posix_ustar *tar_header) {
	return tar_header->name;	
}

char* get_mode(header_posix_ustar *tar_header) {
	return tar_header->mode;
}

int get_size(header_posix_ustar *tar_header) {	
	return octal_to_integer(atoi(tar_header->size));
}

char * get_linkname(header_posix_ustar *tar_header) {
	return tar_header->linkname;
}

int is_posix_ustar(header_posix_ustar *tar_header) {
	return strcmp(tar_header->magic, USTAR) == 0;
}

char* get_version(header_posix_ustar *tar_header) {
	return tar_header->version;
}

char* get_uname(header_posix_ustar *tar_header) {
	return tar_header->uname;
}

char* get_gname(header_posix_ustar *tar_header) {
	return tar_header->gname;
}

int is_empty(header_posix_ustar *tar_header) {
	if(tar_header->name[0] == '\0')
		return 1;
	else
		return 0;
}

void display_header(header_posix_ustar *tar_header) {
	printf("name %s %s\n", OUTPUT_SEPARATOR,  get_name(tar_header));
	printf("mode %s %s\n", OUTPUT_SEPARATOR,  get_mode(tar_header));
	printf("size %s %d bytes\n", OUTPUT_SEPARATOR,  get_size(tar_header));
	printf("linkname %s %s\n", OUTPUT_SEPARATOR,  get_linkname(tar_header));
	printf("is USTAR? %s %d\n", OUTPUT_SEPARATOR,  is_posix_ustar(tar_header));
	printf("version %s %c%c\n", OUTPUT_SEPARATOR, get_version(tar_header)[0], get_version(tar_header)[1]);
	printf("uname %s %s\n", OUTPUT_SEPARATOR, get_uname(tar_header));
	printf("gname %s %s\n\n", OUTPUT_SEPARATOR,  get_gname(tar_header));
}