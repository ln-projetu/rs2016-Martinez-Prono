#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "header_posix_ustar.h"
#include "ptar.h"
#include "block.h"
#include "utils.h"
#include "Option.h"
#include <utime.h>
#include <time.h>
#include "w_info.h"



extern Option *options;
extern *thread_tab;


int open_tar(char* filename) {
	return open(filename, O_RDONLY, 0);
}

int read_tar_file(char* filename) {
	int nb_zeros_blocks = 0;		// Count zeros block at the end of file
	int fd = open_tar(filename);

	if (fd != -1) {
		header_posix_ustar *header = create_header();

		while (nb_zeros_blocks < 2) {
			read(fd, header, BLOCK_SIZE);
			if (is_empty(header))
				nb_zeros_blocks++;
			else {
				nb_zeros_blocks = 0;
				print_results(header);
			}

			move_next_512b(fd, get_size(header), 0);
		}
		free(header);
	}
	close(fd);
	return 0;
}

void read_data_block(int fd, int size_data) {
	block data_bloc;
	int r = 0;
	while (size_data > 0) {
		r = read(fd, &data_bloc, BLOCK_SIZE);
		size_data = size_data - r;
	}
}

int extract_tar(char *filename) {
	int nb_zeros_blocks = 0;		// Count zeros block at the end of file
	int fd = open_tar(filename);
	//pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * 1);

	if (fd != -1) {

		while (nb_zeros_blocks < 2) {
			header_posix_ustar *header = create_header();
			read(fd, header, BLOCK_SIZE);
			if (is_empty(header))
				nb_zeros_blocks++;
			else {
				nb_zeros_blocks = 0;
				char* buffer = (char *)malloc(sizeof(char) * get_size(header));
				read(fd, buffer, get_size(header));
				extract_entry(create_w_info(header, buffer));
				print_results(header);
				move_next_512b(fd, get_size(header), 1);
			}
		}
	}
	close(fd);
	return 0;
}


void extract_entry(w_info* info) {
	header_posix_ustar* header =  get_header(info);
	if(DEBUG)

		printf("Extract '%s' -> %c\n", get_name(header), get_type(header));
	
	/*if (is_regular_file(header))
		extract_regular_file(fd, header);

	if (is_directory(header))
		extract_directory(fd, header);

	if (is_symblink(header))
		extract_symblink(fd, header);

	if(is_symblink(header))
		extract_symblink(fd,header);
	*/

	if (is_regular_file(header))
		extract_regular_file(info);		

	if (is_directory(header))
		extract_directory(info);
	if(is_symblink(header))
		extract_symblink(info);


}

void change_date_file(char* name, long seconds) {
	struct utimbuf t;
	time(&t.actime);
	t.modtime = seconds;
	utime(name, &t);
}


void extract_regular_file(w_info* info) {
	header_posix_ustar* header = get_header(info);
	if(DEBUG)
		printf("%s -> %d\n", get_name(header), get_mode(header));
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

void print_results(header_posix_ustar *header) {
	if (isl(options))
		printf("%s\n", print_as_list(header));
	else
		printf("%s\n", get_name(header));
}