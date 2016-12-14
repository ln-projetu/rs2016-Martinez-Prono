#include <stdio.h>
#include <stdlib.h>
#include "print.h"
#include "option.h"


extern Option *options;


void print_results(header_posix_ustar *header) {
	if (isl(options) == 1) {
		char *desc = print_as_list(header);
		printf("%s\n", desc);
		free(desc);
	}
	else
		printf("%s\n", get_name(header));
}


void print_corrupted() {
	printf("The archive is corrupted\n");
}

void print_no_args() {
	printf("Usage\n\t./ptar -x myArchive.tar\n");
}

void print_cannot_open(char *filename) {
	printf("Cannot open '%s'\n", filename);
}