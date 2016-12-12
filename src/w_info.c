#include <stdio.h>
#include <stdlib.h>
#include "w_info.h"


w_info* create_w_info(header_posix_ustar *header) {
	w_info* ptr;
	ptr = (w_info *) malloc(sizeof(w_info));

	if(ptr == NULL)
		return NULL;

	ptr->buffer = (char *) malloc(sizeof(char) * get_size(header));
	ptr->header = header;
	return ptr;
}

void free_w_info(w_info *info) {
	free(info->header);
	free(info->buffer);
	free(info);
}

header_posix_ustar* get_header(w_info* info) {
	return info->header;
}

char* get_data(w_info* info) {
	return info->buffer;
}