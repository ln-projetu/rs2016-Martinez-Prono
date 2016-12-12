#ifndef WINFO_H
#define WINFO_H

#include "header_posix_ustar.h"


/**
 * Structure for in order to pass essential informations for a thread.
 * In our programm, threads are used to write data (file, directories ...) in the hard disk.
 * We have to use fsync() to be sure that data are really written on the disk and this method
 * can take a lot of time ! (that's why you used threads)
 * 
 * Information required for threads is the header of the entry and its data buffer !
 *
 * 'w_info for write_information'
 *
 *	@author Lucas MARTINEZ
 *	@author Yann PRONO
 */


typedef struct w_info {
	header_posix_ustar* header;
	char* buffer;
} w_info;


/**
 * Malloc a w_info structure
 * @param header Header of the entry to write
 * @return a w_info structure
 */
w_info* create_w_info(header_posix_ustar *header);

/**
 * Free the memory for a w_info structure.
 * @param info Structure to free.
 */
void free_w_info(w_info *info);

/**
 * @param info 
 * @return a pointer to the header_posix_ustar structure in the w_info.
 */
header_posix_ustar* get_header(w_info* info);

/**
 * @param info
 * @return Data to write
 */
char* get_data(w_info* info);


#endif