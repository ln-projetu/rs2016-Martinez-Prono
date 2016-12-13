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
#include "option.h"
#include "extract.h"
#include "w_info.h"

#include <semaphore.h>

#include "print.h"

#define LENGTH_GZ 0x1000


extern Option *options;
extern pthread_t *thread_tab;
extern sem_t *semaphore;
extern int *thread_tab_bool;

int open_tar(char* filename) {
	return open(filename, O_RDONLY, 0);
}

int read_tar(char* filename) {
	int nb_zeros_blocks = 0;		// Count zeros block at the end of file
	int fd = open_tar(filename);
	if (fd == -1) {
		print_cannot_open(filename);
		return -1;
	}

	header_posix_ustar *header = create_header();
	while (nb_zeros_blocks < 2) {
		read(fd, header, BLOCK_SIZE);
		if (is_empty(header))
			nb_zeros_blocks++;
		else {
			if(check_sum(header) == 0) {
				print_corrupted();
				return -1;
			}

			nb_zeros_blocks = 0;
			print_results(header);
		}
		move_next_512b(fd, get_size(header), 0);
	}

	free(header);
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
	if(tar_file == NULL)
		return -1;
	else if (isx(options)==0)
		return 0;
	else
		return extract_tar(tar_file);
}

int extract_tar(char *filename) {
	// Count zeros block at the end of file
	int nb_zeros_blocks = 0;
	header_posix_ustar *header;
	int fd = open_tar(filename);
	//pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * 1);
	int i=0;
	int y=0;
	int sval;
	
	if (fd != -1) {
		sem_getvalue(semaphore,&sval);
		if(DEBUG)
			printf("Before %d\n",sval);
		
		while (nb_zeros_blocks < 2) {
			header = create_header();
			read(fd, header, BLOCK_SIZE);
			//int move = get_size(header);

			if (is_empty(header)) {
				nb_zeros_blocks++;
			}

			else{
				
					nb_zeros_blocks = 0;
					if(check_sum(header) == 0) {
						print_corrupted();
						return -1;
					}
					//char* buffer = (char *)malloc(sizeof(char) * get_size(header));
					w_info* w = create_w_info(header);
					read(fd, w->buffer, get_size(header));

					sem_wait(semaphore);
					
					if(getnbp(options) != 1){
						for(i=0;i<getnbp(options);i++){

							if(thread_tab_bool[i] == 0){
								
								y=i;
								if(DEBUG)
									printf("THREAD FREE IN LOOP %d\n",y);
							}

						}
					}
					
					sem_getvalue(semaphore,&sval);
					if(DEBUG)
						printf("Sema after wait %d\n",y);
					
					w->num_thread=y;
					
					if(DEBUG)
						printf("THREAD SELECTED %d\n",y);
					
					if(getnbp(options) != 1){
						if(thread_tab_bool[y] ==0){
							thread_tab_bool[y]=1;
							pthread_create(&thread_tab[y], NULL, extract_entry, (void*) w);

						}
					}
					else
						pthread_create(thread_tab, NULL, extract_entry, (void*) w);

					
					//extract_entry(create_w_info(header, buffer));
					//print_results(header);
					
					move_next_512b(fd, get_size(header), 1);
					//free(marty);
				

			}
			//free(header);
		}
		

	}
	if(getnbp(options) != 1){
	for(i=0;i<getnbp(options);i++){

		pthread_join(thread_tab[i],NULL);


	}
	}
	else
		pthread_join(*thread_tab,NULL);
	close(fd);
	free(header);
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


	int gz = (*gzopen)(filename, "r");
	if(!gz){
		print_cannot_open(filename);
		return NULL;
	}

	char *file_no_gz = basename(filename);
	int out = open(file_no_gz, O_CREAT | O_RDWR |O_APPEND, 0777);
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