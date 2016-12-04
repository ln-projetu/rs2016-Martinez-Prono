#include <sys/types.h>
#include <unistd.h>
#include "utils.h"


/*int octal_to_integer(int octal) {
	int res = 0;
	int coef = 1;
	while(octal != 0) {
		res = res + (octal % 10) * coef;
		octal = octal/10;
		coef  = coef * 8;
    }

    return res;
}*/

int octal_to_integer(long octal){
	long res = 0;
	long coef = 1;
	while(octal != 0) {
		res = res + (octal % 10) * coef;
		octal = octal/10;
		coef  = coef * 8;
    }

    return (int)res;
}

void move_next_512b(int fd, int size, int write_mode) {
	int i = 0;

	while (i * 512 < size) {
		i++;
	}
	if(write_mode==1)
		lseek(fd, i * 512 - size, SEEK_CUR);
	else
		lseek(fd, i * 512, SEEK_CUR);
}

