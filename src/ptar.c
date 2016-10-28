#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "header_posix_ustar.h"
#include "utils.h"

int read_tar_file(const char *filename) {
	int fd = -1;
	fd = open(filename, O_RDONLY, 0);
	if (fd < 0)
		return fd;

	// Count zeros block at the end of file
	int nb_zeros_blocks = 0;
	int i;
	header_posix_ustar tar_header;

	while((i = read(fd, &tar_header, TAR_BLOCK_SIZE)) == TAR_BLOCK_SIZE) {
		
		if(nb_zeros_blocks >= 2)				// Is it end of tar file ?
			return 0;	
		else {									// Is it a zeros bytes block ?
			if(is_empty(&tar_header))
				nb_zeros_blocks++;			
			else								// If not then it is a data block
				display_header(&tar_header);

		return 0;
		}
	}

	return 0;
}


int main(int argc, char const *argv[]) {
	int statut;
	
	if(argc > 1)
		statut = read_tar_file(argv[1]);
	else
		statut = -1;

	exit(statut);
}