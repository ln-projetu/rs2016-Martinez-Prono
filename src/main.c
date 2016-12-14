#include <stdio.h>
#include <stdlib.h>
#include "option.h"
#include "ptar.h"
#include "print.h"
#include <pthread.h>
#include <semaphore.h>


Option *options;
pthread_t *thread_tab;
sem_t *semaphore;
int *thread_tab_bool;

/**
 * Main program which
 * triggers actions depending on enabled options.
 *
 * @author Lucas MARTINEZ
 * @author Yann PRONO
 */


 int run(char* filename) {
 	int statut;

	// if no option OR just only list file with details
 	if ((count_options(options) == 0 )|| (isl(options) && count_options(options) == 1))
 		statut = read_tar(filename);
 	else {
 		if (isz(options) && isx(options) == 0)
 			statut = extract_tar_gz(filename);
 		else if (isp(options) ){
 			if(DEBUG == 1)
 				printf("Le nombre de threads est %d\n",getnbp(options) );

 			thread_tab = (pthread_t *)malloc(sizeof(pthread_t)*getnbp(options));
 			thread_tab_bool=(int *)malloc(sizeof(int)*getnbp(options));

 			int i;
 			for(i = 0; i < getnbp(options); i++) {
 				thread_tab_bool[i] = 0;
 				thread_tab[i] = 0;
 			}

 			semaphore = (sem_t*)malloc(sizeof(sem_t));
 			sem_init(semaphore,0,getnbp(options));
 			if (isz(options))
 				statut = extract_tar_gz(filename);
 			else
 				statut = extract_tar(filename);
 		}

 		else if(isx(options) && isp(options) == 0){
 			if (isz(options))
 				statut = extract_tar_gz(filename);
 			else
 				statut = extract_tar_nop(filename);
 		}
 	}

 	return statut;
 }


 int main(int argc, char *argv[]) {
 	int statut = 0;
 	options = create_option();

 	if (checkoption(argc, argv, options) == 0 && argc > 1)
 		statut = run(argv[argc - 1]);
 	else
 		print_no_args();

 	free(thread_tab);
 	free(options);
 	free(thread_tab_bool);
 	free(semaphore);
 	exit(statut);
 }