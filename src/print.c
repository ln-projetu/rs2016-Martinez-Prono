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