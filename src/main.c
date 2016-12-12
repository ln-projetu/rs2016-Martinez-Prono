#include <stdio.h>
#include <stdlib.h>
#include "Option.h"
#include "ptar.h"
#include <pthread.h>

/**
 * Main program which
 * triggers actions depending on enabled options.
 *
 * @author Lucas MARTINEZ
 * @author Yann PRONO
 */
Option *options;
pthread_t *thread_tab;


int main(int argc, char *argv[]) {
	int statut = 0;
	options = create_option();

	if (checkoption(argc, argv, options) == 0) {
		// if no option OR just only list file with details
		if (count_options(options) == 0 || (isl(options) && count_options(options) == 1))
			statut = read_tar(argv[argc - 1]);
		else {

			if (isp(options) )
				thread_tab = (pthread_t *)malloc(sizeof(pthread_t)*getnbp(options));

			else if (isz(options))
				extract_tar_gz(argv[argc - 1]);
			else if(isx(options))
				extract_tar(argv[argc - 1]);
		}
	}

	free(options);
	exit(statut);
}