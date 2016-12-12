#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include "header_posix_ustar.h"
#include "ptar.h"
#include "utils.h"
#include "Option.h"
#include "extract.h"
#include "w_info.h"
#include <semaphore.h>
#define LENGTH_GZ 0x1000


extern Option *options;
extern pthread_t *thread_tab;
extern sem_t *semaphore;


int open_tar(char* filename) {
	return open(filename, O_RDONLY, 0);
}

int read_tar(char* filename) {
	int nb_zeros_blocks = 0;		// Count zeros block at the end of file
	int fd = open_tar(filename);

	if (fd != -1) {
		header_posix_ustar *header = create_header();
		while (nb_zeros_blocks < 2) {
			read(fd, header, BLOCK_SIZE);
			if (is_empty(header))
				nb_zeros_blocks++;
			else {
				if(get_checksum(header) != calculate_checksum(header)) {
					printf("The archive is corrupted\n");
					return -1;
				}

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
	int r = 0;
	while (size_data > 0) {
		r = read(fd, NULL, BLOCK_SIZE);
		size_data = size_data - r;
	}
}

int extract_tar_gz(char *filename) {
	char *tar_file = uncompress_archive(filename);
	return extract_tar(tar_file);
}

int extract_tar(char *filename) {
	// Count zeros block at the end of file
	int nb_zeros_blocks = 0;
	int fd = open_tar(filename);
	//pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * 1);
	int i=0;
	int y;
	int sval;
	
	if (fd != -1) {
		sem_getvalue(semaphore,&sval);
		printf("Before %d\n",sval);
		
		while (nb_zeros_blocks < 2) {
			header_posix_ustar *header = create_header();
			read(fd, header, BLOCK_SIZE);

			if (is_empty(header))
				nb_zeros_blocks++;
			else{
				
					nb_zeros_blocks = 0;
					char* buffer = (char *)malloc(sizeof(char) * get_size(header));
					read(fd, buffer, get_size(header));
					w_info* w = create_w_info(header, buffer);
					if(thread_tab[i] == NULL)
						printf("THREAD NULL in extract\n");
					for(i=0;i<getnbp(options);i++){

						if(thread_tab[i] == NULL)
							y=i;

					}
					sem_wait(semaphore);
					sem_getvalue(semaphore,&sval);
					
					printf("After %d\n",sval);
					pthread_create(&thread_tab[y], NULL, extract_entry, (void*) w);

					i++;
					//extract_entry(create_w_info(header, buffer));
					print_results(header);
					move_next_512b(fd, get_size(header), 1);
					//free(marty);
				
			}

		}
		

	}
	for(i=0;i<getnbp(options);i++){

		pthread_join(thread_tab[i],NULL);

	}
	close(fd);
	//pthread_exit(NULL);
	return 0;
}

char *uncompress_archive(char* filename) {
	void *handle;
	int (*gzopen)	(char *path, char *mode);
	int (*gzread)	(int fd, void *buf, unsigned int len);
	int (*gzeof)	(int fd);
	int (*gzclose)	(int fd);
	handle 	= dlopen("libz.so", RTLD_NOW);
	gzopen 	= dlsym(handle, "gzopen");
	gzread 	= dlsym(handle, "gzread");
	gzeof 	= dlsym(handle, "gzeof");
	gzclose = dlsym(handle, "gzclose");

	char *file_no_gz = basename(filename);
	int out = open(file_no_gz, O_CREAT | O_RDWR |O_APPEND, 0777);
	int gz = (*gzopen)(filename, "r");
	int bytes_read;
	unsigned char buffer[LENGTH_GZ];

    while (1) {
		bytes_read = (*gzread)(gz, buffer, LENGTH_GZ - 1);
		buffer[bytes_read] = '\0';
		write(out, buffer, bytes_read - 1);
		if (bytes_read < LENGTH_GZ - 1 && (*gzeof)(gz))
			break;
    }
    close(out);
	(*gzclose)(gz);

	return file_no_gz;
}

void print_results(header_posix_ustar *header) {
	if (isl(options))
		printf("%s\n", print_as_list(header));
	else
		printf("%s\n", get_name(header));
}