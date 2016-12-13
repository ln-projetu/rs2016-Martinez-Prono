#include <stdio.h>
#include "print.h"
#include "option.h"


extern Option *options;


void print_results(header_posix_ustar *header) {
	if (isl(options))
		printf("%s\n", print_as_list(header));
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