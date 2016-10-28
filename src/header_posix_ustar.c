#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header_posix_ustar.h"
#define OUTPUT_SEPARATOR "->"

char* get_name(header_posix_ustar *tar_header) {
	return tar_header->name;	
}

char* get_mode(header_posix_ustar *tar_header) {
	return tar_header->mode;
}

int get_size(header_posix_ustar *tar_header) {
	return atoi(tar_header->size);
}

int is_ustar(header_posix_ustar *tar_header) {
	printf("#%s#  %zu\n", tar_header->magic, strlen(tar_header->magic));
	printf("#%s#  %zu\n", USTAR, strlen(USTAR));
	return strcmp(tar_header->magic, USTAR);
}

char* get_uname(header_posix_ustar *tar_header) {
	return tar_header->uname;
}

char* get_gname(header_posix_ustar *tar_header) {
	return tar_header->gname;
}

void display_header(header_posix_ustar *tar_header) {
	printf("name %s %s\n", OUTPUT_SEPARATOR,  get_name(tar_header));
	printf("mode %s %s\n", OUTPUT_SEPARATOR,  get_mode(tar_header));
	printf("size (not good value) %s %d\n", OUTPUT_SEPARATOR,  get_size(tar_header));
	printf("is ustar %s %d\n", OUTPUT_SEPARATOR,  is_ustar(tar_header));
	printf("uname %s %s\n", OUTPUT_SEPARATOR, get_uname(tar_header));
	printf("gname %s %s\n", OUTPUT_SEPARATOR,  get_gname(tar_header));
}