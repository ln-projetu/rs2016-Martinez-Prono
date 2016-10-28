#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "header_posix_ustar.h"

#define TAR_BLOCK_SIZE 512


int main(int argc, char const *argv[]) {

	if(argc > 1) {
		int fd = -1;
		// Open the file
		fd = open(argv[1], O_RDONLY, 0);

		if (fd != -1) {
			printf("can open %s\n", argv[1]);
			header_posix_ustar tar_header;
			// Read first 512 bytes block
			read(fd, &tar_header, TAR_BLOCK_SIZE);
			
			display_header(&tar_header);
		}
	}

	exit(0);
}