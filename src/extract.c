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

#include <semaphore.h>

#include "print.h"
#include "option.h"


extern sem_t *semaphore;
extern int *thread_tab_bool;
extern Option *options;

void *extract_entry(void *args) {
	int sval;
	w_info *info = (w_info *) args;
	//pthread_t current =  pthread_self();
	if(DEBUG)
		printf("## Current thread : %p\n", (pthread_t *)pthread_self());
	header_posix_ustar *header =  get_header(info);
	print_results(header);

	if(DEBUG)
		printf("Extract '%s' -> %c\n", get_name(header), get_type(header));

	if (is_regular_file(header))
		extract_regular_file(info);

	if (is_directory(header))
		extract_directory(info);
	if(is_symblink(header))
		extract_symblink(info);

	
	thread_tab_bool[info->num_thread]=0;
	
	sem_post(semaphore);
	sem_getvalue(semaphore,&sval);
	if(DEBUG)		
		printf("PST after post %d\n",sval);
	free_w_info(info);
	pthread_exit(NULL);
}

void *extract_entry_nop(void *args) {

	w_info *info = (w_info *) args;


	header_posix_ustar *header =  get_header(info);
	print_results(header);

	if(DEBUG)
		printf("Extract '%s' -> %c\n", get_name(header), get_type(header));

	if (is_regular_file(header))
		extract_regular_file(info);
	if (is_directory(header))
		extract_directory(info);
	if(is_symblink(header))
		extract_symblink(info);

	free_w_info(info);
	return 0;
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
	fsync(out);

	// Need maybe tu put these lines of code in a function...
	fchmod(out, get_mode(header));
	fchown(out, get_uid(header), get_gid(header));
	change_date_file(get_name(header), get_mtime(header));
	close(out);
}

void extract_directory(w_info* info) {
	header_posix_ustar* header = get_header(info);
	mkdir(get_name(header), get_mode(header));
	chown(get_name(header), get_uid(header), get_gid(header));
	change_date_file(get_name(header), get_mtime(header));
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