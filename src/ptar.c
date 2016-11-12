#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "header_posix_ustar.h"
#include "ptar.h"
#include "block.h"
#include "utils.h"
#include "Option.h"

extern Option *options;


int open_tar(char* filename) {
	return open(filename, O_RDONLY, 0);
}

int read_tar_file(int fd) {
	int nb_zeros_blocks = 0;		// Count zeros block at the end of file
	int i;
	header_posix_ustar *header = create_header();

	while ((i = read(fd, header, BLOCK_SIZE)) == BLOCK_SIZE) {

		if (nb_zeros_blocks >= 2) {				// Is it end of tar file ?
			free(header);
			return 0;
		}
		else {									// Is it a zeros bytes block ?
			if (is_empty(header))
				nb_zeros_blocks++;
			else {

				if (isl(options)) {
					char *buf = print_as_list(header);
					printf("%s\n", buf);
					free(buf);
				}
				else {
					printf("%s\n", get_name(header)); // If not then it is a data block
					//display_header(header);
				}

				move_next_512b(fd, get_size(header), 0);
			}
		}

	}
	free(header);
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

	if (fd != -1) {
		header_posix_ustar *header = create_header();

		while (nb_zeros_blocks < 2) {
			read(fd, header, BLOCK_SIZE);
			if (is_empty(header))
				nb_zeros_blocks++;
			else {
				nb_zeros_blocks = 0;
				extract_entry(fd, header);
			}
		}
		free(header);
	}
	close(fd);
	return 0;
}

void extract_entry(int fd, header_posix_ustar *header) {
	printf("Extract '%s' -> %c\n", get_name(header), get_type(header));

	if (is_regular_file(header))
		extract_regular_file(fd, header);

	if (is_directory(header))
		extract_directory(fd, header);
}



void extract_regular_file(int fd, header_posix_ustar *header) {


	int out = open(get_name(header),  O_CREAT | O_WRONLY, get_mode(header));
	int size_data = get_size(header);

	char * data = (char *)malloc(sizeof(char) * size_data);
	// Need maybe tu put these lines of code in a function...
	fchmod(out, get_mode(header));
	fchown(out, get_uid(header), get_gid(header));


	read(fd, data, size_data);
	write(out, data, size_data);
	move_next_512b(fd, size_data, 1);
	free(data);
	close(out);
}

void extract_directory(int fd, header_posix_ustar *header) {
	mkdir(get_name(header), get_mode(header));
	// No data to read after the header of a directory.
}

void create_tar_dir(char *name_tar) {

}