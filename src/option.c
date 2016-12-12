#include "option.h"
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>


Option *create_option() {
	Option * pto = (Option *)malloc(sizeof(Option));
	pto->extract = 0;
	pto->list = 0;
	pto->pthread = 0;
	pto->gzip = 0;
	pto->nb_thread = 0;
	return pto;
}

void setx(Option *a) {
	a->extract = 1;
};

void setl(Option *a) {
	a->list = 1;
};

void setp(Option *a) {
	a->pthread = 1;
};

void setz(Option *a) {
	a->gzip = 1;
};

void setnb(Option *a, int nb) {
	a->nb_thread = nb;
};

int isl(Option *a) {
	return a->list;
};

int isx(Option *a) {
	return a->extract;
};

int isp(Option *a) {
	return a->pthread;
};

int isz(Option *a) {
	return a->gzip;
};

int getnbp(Option *a) {
	return a->nb_thread;
};

int checkoption(int argc, char *argv[], Option *a) {
	int c;
	char opt[] = "xlpz";
	opterr = 0;
	while ( (c = getopt(argc, argv, opt)) != (-1) ) {

		/*printf("%d -- %d -- %c\n",optind,opterr, c);*/
		if (c == '?') {
			printf("OPTION NON RECONNUE\n");
			return 1;
		}
		else {
			switch (c) {
			case 'x':
				setx(a);
				break;
			case 'l':
				setl(a);
				break;
			case 'p':
				setp(a);
				break;
			case 'z':
				setz(a);
				break;
			}
		}
	}
	/*printf("%d --\n",optind);*/
	if (isp(a) == 1) {
		int nb_thread = atoi(argv[optind]);
		if (nb_thread == 0 || optind == argc - 1) {
			printf("invalid number of threads\n");
			return 1;
		}
		else if(isp(a)==1 && isx(a) == 0){
			printf("Nombre de threads définit, mais sans extraction demandée, pas de sens\n");
			return 1;
		}
		else
			setnb(a, nb_thread);
	}
	return 0;
};

int count_options(Option *options) {
	return (isl(options) + isp(options) + isx(options) + isz(options));
}