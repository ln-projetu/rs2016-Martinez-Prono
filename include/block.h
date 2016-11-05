#ifndef BLOCK_H
#define BLOCK_H

#include "definitions.h"


/**
 * Header file representing a data block of 512 bytes.
 *
 * @author Lucas MARTINEZ
 * @author Yann PRONO
 */


typedef struct block {
	char data[BLOCK_SIZE];
} block;


#endif