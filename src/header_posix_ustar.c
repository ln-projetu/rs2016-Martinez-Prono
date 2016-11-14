#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header_posix_ustar.h"
#include "block.h"
#include "utils.h"
#define OUTPUT_SEPARATOR "->"
#define BUF_SIZE 250


char* get_name(header_posix_ustar *header) {
	return header->name;
}

char get_type(header_posix_ustar *header) {
	return header->typeflag;
};

mode_t get_mode(header_posix_ustar *header) {
	return (mode_t) octal_to_integer((atoi(header->mode)));
}

uid_t get_uid(header_posix_ustar *header) {
	return (uid_t) octal_to_integer(atoi(header->uid));
}

gid_t get_gid(header_posix_ustar *header) {
	return (gid_t) octal_to_integer(atoi(header->gid));
}

int get_size(header_posix_ustar *header) {
	return octal_to_integer(atoi(header->size));
}

time_t get_mtime(header_posix_ustar *header) {
	return (time_t)octallong_to_integer(atol(header->mtime));
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
	if (header->name[0] == '\0')
		return 1;
	else
		return 0;
}

int is_regular_file(header_posix_ustar *header) {
	return get_type(header) == REGFILE;
}

int is_symblink(header_posix_ustar *header){
	return get_type(header) == SLINK;
}

int is_directory(header_posix_ustar *header) {
	return get_type(header) == DIR;
}

char* print_as_list(header_posix_ustar *header) {
	char right[11] = {'-' , '-' , '-' , '-' , '-' , '-' , '-' , '-' , '-', '-'};

	char uid[8];
	sprintf(uid, "%d", get_uid(header));

	char gid[8];
	sprintf(gid, "%d", get_gid(header));

	char size[12];
	sprintf(size, "%d", get_size(header));

	char *finalstrng = (char*)malloc(sizeof(char) * BUF_SIZE);

	char buf[2];
	buf[0] = (header->mode)[4];
	int ow = strtol(buf, NULL, 10);

	buf[0] = (header->mode)[5];
	int gr = strtol(buf, NULL, 10);

	buf[0] = (header->mode)[6];
	int oth = strtol(buf, NULL, 10);

	int mode[3] = {ow, gr, oth};

	int i;
	for (i = 0; i < 3; i++) {
		switch (mode[i]) {

		case 1:
			right[i * 3 + 3] = 'x';
			break;
		case 2:
			right[i * 3 + 2] = 'w';
			break;
		case 3:
			right[i * 3 + 1] = 'r';
			right[i * 3 + 2] = 'w';
			break;
		case 4:
			right[i * 3 + 1] = 'r';
			break;
		case 5:
			right[i * 3 + 1] = 'r';
			right[i * 3 + 3] = 'x';
			break;
		case 6:
			right[i * 3 + 1] = 'r';
			right[i * 3 + 2] = 'w';
			break;
		case 7:
			right[i * 3 + 1] = 'r';
			right[i * 3 + 2] = 'w';
			right[i * 3 + 3] = 'x';
			break;

		}
	}



	if (is_directory(header))
		right[0] = 'd';
	if (is_symblink(header))
		right[0] = 'l';

	char date[30];

	time_t ti = get_mtime(header);
	struct tm time = *localtime(&ti);

	strftime(date, 30, "%Y-%m-%d %X", &time);

	if (get_type(header) == '2') {
		snprintf(finalstrng, BUF_SIZE, "%s%s%s%s%s%s%s%s%s%s%s%s%s", right, " ", uid, "/", gid, " ", size, " ", date, " ", get_name(header),"->",get_linkname(header));

	}
	else {
		snprintf(finalstrng, BUF_SIZE, "%s%s%s%s%s%s%s%s%s%s%s", right, " ", uid, "/", gid, " ", size, " ", date, " ",get_name(header));
	}

	return finalstrng;
}

header_posix_ustar *create_header() {
	header_posix_ustar* ptr = (header_posix_ustar*)malloc(sizeof(header_posix_ustar));
	return ptr;
}

void display_header(header_posix_ustar *header) {
	char *buf = print_as_list(header);
	printf("%s\n", get_name(header));
	printf(" - type %s %c\n", OUTPUT_SEPARATOR, get_type(header));
	printf(" - mode %s %d \n", OUTPUT_SEPARATOR, atoi(header->mode));
	printf(" - CHAINE LIST %s %s \n", OUTPUT_SEPARATOR, buf);
	printf(" - uid %s %d\n", OUTPUT_SEPARATOR, get_uid(header));
	printf(" - gid %s %d\n", OUTPUT_SEPARATOR, get_gid(header));
	printf(" - size %s %d bytes\n", OUTPUT_SEPARATOR, get_size(header));
	printf(" - mtime %s %d seconds since the start of epoch (1970)\n", OUTPUT_SEPARATOR,(int)get_mtime(header));
	printf(" - checksum %s %d\n", OUTPUT_SEPARATOR, get_checksum(header));
	printf(" - linkname %s %s\n", OUTPUT_SEPARATOR,  get_linkname(header));
	printf(" - is USTAR? %s %d\n", OUTPUT_SEPARATOR,  is_posix_ustar(header));
	printf(" - version %s %c%c\n", OUTPUT_SEPARATOR, get_version(header)[0], get_version(header)[1]);
	printf(" - uname %s %s\n", OUTPUT_SEPARATOR, get_uname(header));
	printf(" - gname %s %s\n\n", OUTPUT_SEPARATOR,  get_gname(header));
	free(buf);
}
