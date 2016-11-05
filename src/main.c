#include <stdio.h>
#include <stdlib.h>
#include "Option.h"
#include "ptar.h"

/**
 * Main program which
 * triggers actions depending on enabled options.
 *
 * @author Lucas MARTINEZ
 * @author Yann PRONO
 */


int main(int argc, char *argv[]) {
	int statut = -1;
	Option *options;
	// First of all, check enabled options
	options = create_option();
	if (checkoption(argc, argv, options) == 0) {

		// Extract tarball
		if (isx(options))
			extract_tar(argv[argc - 1]);
	}

	free(options);
	exit(statut);
}