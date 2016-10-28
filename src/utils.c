#ifndef UTILS_H
#define UTILS_H


int octal_to_integer(int octal) {
	int res = 0;
	int coef = 1;
	while(octal != 0) {
		res = res + (octal % 10) * coef;
		octal = octal/10;
		coef  = coef * 8;
    }

    return res;
}


#endif