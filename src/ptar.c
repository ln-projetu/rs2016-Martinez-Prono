#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "header_posix_ustar.h"
#include "block.h"
#include "Option.h"
#include <string.h>

int write_file(int fd,int size_data,char filename[]){
	char data[size_data];
	char test[]="testwrite_";
	strcat(filename,test);
	read(fd, data, size_data);
	int f2=open(filename,O_CREAT,S_IRUSR | S_IWUSR);
	if(f2<0){
		close(f2);
		perror("");
		printf("Bad Create\n");
		return 1;
	}
	close(f2);
	int fo=open(filename,O_WRONLY);
	if(fo>=0){
		int fw=write(fo,data,size_data);
		if(fw<0){
			close(fw);
			close(fo);
			perror("");
			printf("Bad Write\n");
			return 1;
		}
		close(fw);
		close(fo);
		lseek(fd,(-1)*size_data,SEEK_CUR); // Move back fd to have a right 512k block reading in write_file() method 
	}
	return 0;
}

void read_data_block(int fd, int size_data, char filename[]) {
	block data_bloc;
	write_file(fd,size_data,filename);
	while(size_data > 0) {
		int r = read(fd, &data_bloc, BLOCK_SIZE);
		size_data = size_data - r;
	}
}


int read_tar_file(int fd) {
	// Count zeros block at the end of file
	int nb_zeros_blocks = 0;
	int i;
	header_posix_ustar header;

	while((i = read(fd, &header, BLOCK_SIZE)) == BLOCK_SIZE) {
		
		if(nb_zeros_blocks >= 2)				// Is it end of tar file ?
			return 0;	
		else {									// Is it a zeros bytes block ?
			if(is_empty(&header))
				nb_zeros_blocks++;			
			else {								// If not then it is a data block
				display_header(&header);				
				read_data_block(fd, get_size(&header),get_name(&header));
			}	
		}		
	}

	return 0;
}

int main(int argc, char *argv[]) {
	Option *option=create_option();
	int statut=-1;
	if(checkoption(argc,argv,option)==0){
		
		
		int fd = -1;

		if(argc > 1) {
			fd = open(argv[argc-1], O_RDONLY, 0);
			
			if (fd < 0)
				statut = fd;

			statut = read_tar_file(fd);
		}
		else
			statut = -1;
			
	}
	free(option);

	exit(statut);
}
