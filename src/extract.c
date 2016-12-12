#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dlfcn.h>
#include <utime.h>
#include <time.h>
#include "extract.h"


void *extract_entry(void *args) {
	w_info *info = (w_info *) args;
	pthread_t current =  pthread_self();
	if(DEBUG)
		printf("## Current thread : %p\n", &current);
	header_posix_ustar *header =  get_header(info);

	if(DEBUG)
		printf("Extract '%s' -> %c\n", get_name(header), get_type(header));

	if (is_regular_file(header))
		extract_regular_file(info);

	if (is_directory(header))
		extract_directory(info);
	if(is_symblink(header))
		extract_symblink(info);

	pthread_exit(NULL);
}

void change_date_file(char* name, long seconds) {
	struct utimbuf t;
	time(&t.actime);
	t.modtime = seconds;
	utime(name, &t);
}


void extract_regular_file(w_info* info) {
	header_posix_ustar* header = get_header(info);

	int out = open(get_name(header),  O_CREAT | O_WRONLY);
	write(out, get_data(info), get_size(header));
	//fsync(out);
	// Need maybe tu put these lines of code in a function...
	fchmod(out, get_mode(header));
	fchown(out, get_uid(header), get_gid(header));
	change_date_file(get_name(header), get_mtime(header));
	close(out);
}

void extract_directory(w_info* info) {
	header_posix_ustar* header = get_header(info);
	mkdir(get_name(header), get_mode(header));
	int out = open(get_name(header),  O_CREAT | O_WRONLY);
	fchown(out, get_uid(header), get_gid(header));
	change_date_file(get_name(header), get_mtime(header));
	close(out);
	// No data to read after the header of a directory.
}


void extract_symblink(w_info* info) {
	header_posix_ustar* header = get_header(info);

	symlink(get_linkname(header),get_name(header));
	int out = open(get_name(header),O_WRONLY);
	fchmod(out, get_mode(header));
	fchown(out, get_uid(header), get_gid(header));
	change_date_file(get_name(header), get_mtime(header));
	close(out);
}