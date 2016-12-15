#include <stdio.h>
#include <stdlib.h>
#include "w_info.h"


w_info* create_w_info(header_posix_ustar *header) {
	w_info* ptr;
	ptr = (w_info *) malloc(sizeof(w_info));

	if(ptr == NULL)
		return NULL;

	if(get_size(header) != 0)
		ptr->buffer = (char *) malloc(sizeof(char) * get_size(header));
	ptr->header = header;
	return ptr;
}

void free_w_info(w_info *info) {
	if(info != NULL) {
		if(get_size(get_header(info)) != 0) {
			free(info->buffer);
		}
		free(info->header);
		free(info);
	}
}

header_posix_ustar* get_header(w_info* info) {
	return info->header;
}

char* get_data(w_info* info) {
	return info->buffer;
}