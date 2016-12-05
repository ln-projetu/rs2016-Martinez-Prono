#include <stdio.h>
#include <stdlib.h>
#include "w_info.h"

w_info* create_w_info(header_posix_ustar *header, char* buffer) {
	w_info* ptr;
	ptr = (w_info *) malloc(sizeof(w_info));
	if(ptr == NULL)
		return NULL;

	ptr->header = header;
	ptr->buffer = buffer;
	return ptr;
}

void free_w_info(w_info *info) {
	free(info->header);
	free(info->buffer);
	free(info);
}