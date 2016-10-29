#ifndef OPTION_H
#define OPTION_H

typedef struct Option{
	int extract;
	int list;
	int pthread;
	int gzip;
	int nb_thread;
}Option;


Option *create_option();

void setx(Option *a);
void setl(Option *a);
void setp(Option *a);
void setz(Option *a);
void setnb(Option *a, int nb);

int isx(Option *a);
int isl(Option *a);
int isp(Option *a);
int isz(Option *a);
int getnbp(Option *a);

int checkoption(int argc, char *argv[],Option *a);

#endif
