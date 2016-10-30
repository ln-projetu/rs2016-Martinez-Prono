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

Option *option; //Global var describes launching oiption of program, maybe global var sucks


int write_file(int fd, header_posix_ustar *header){
	chdir("EXTRACT"); // TEST DIR

	int type=get_type(header); // Get typeflag of the file
if(type==0){ // If it's a file, we write file
	
	int size_data=get_size(header);
	char* filename=get_name(header);
	char data[size_data];
	read(fd, data, size_data);
	int f2=open(filename,O_CREAT,S_IRUSR | S_IWUSR); // Create File, file rights irusr, iwusr are random for test
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
		fsync(fw);
		close(fw);
		close(fo);
		lseek(fd,(-1)*size_data,SEEK_CUR); // Move back fd to have a right 512k block reading in read_data_block() method 
	}
}
else if(type==5){ // If it's a dir, we create the dir
	printf("--------------------\nCREATED DIR\n--------------------\n\n");
	mkdir(get_name(header), 0700); // dir rights 0700 are random for test
	
}
	return 0;
}

void read_data_block(int fd, int size_data) {
	block data_bloc;
	
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
				if(isx(option))
					write_file(fd,&header);			
				read_data_block(fd, get_size(&header));
			}	
		}		
	}

	return 0;
}

int main(int argc, char *argv[]) {
	
	
	option=create_option();
	int statut=-1;
	if(checkoption(argc,argv,option)==0){
		
		if(isx(option))
			mkdir("EXTRACT", 0700); //TEST DIR
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
