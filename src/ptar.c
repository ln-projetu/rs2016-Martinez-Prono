#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "header_posix_ustar.h"
#include "block.h"

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
				read_data_block(fd, get_size(&header));
			}	
		}		
	}

	return 0;
}

int main(int argc, char const *argv[]) {
	int statut;
	int fd = -1;

	if(argc > 1) {
		fd = open(argv[1], O_RDONLY, 0);
		
		if (fd < 0)
			statut = fd;

		statut = read_tar_file(fd);
	}
	else
		statut = -1;

	exit(statut);
}