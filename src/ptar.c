#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "header_posix_ustar.h"
#include "block.h"
#include "Option.h"
#include <string.h>

Option *option; //Global var describes launching oiption of program, maybe global var sucks

typedef struct winfo // INFORMATIONS 
{
	long cur; // position du curseur à partir duquel debute les data d'un fichier
	long size; // taille du fichier 
	char *name; // nom du fichier
} winfo;

winfo * createinfo(long cur, long size, char * name) { // Creer une var winfo
	winfo * ptw = (winfo*)malloc(sizeof(winfo));
	char buff[3];
	ptw->cur = cur;
	ptw->size = size;
	ptw->name = (char*)malloc(sizeof(char) * (strlen(name)));
	//printf("%li -- %s\n", strlen(name), name);
	strcpy(ptw->name, name);

	return ptw;
};

void *hello2(void *j) {
	
	//printwinfo((winfo *)j);
	winfo * t = (winfo *)j; // CAst

	printf("NAMMMMMMEEEEEEEEEEEEEEEEE %s\n",t->name);

	char * data = (char *)malloc(sizeof(char) * t->size); // On creer un char * de taille == taille du fichier


	int rd = open("/home/tux/Documents/rs2016-Martinez-Prono/archive.tar", O_RDONLY); // LES threads changent le dossier courrant du processus. Pour l'instant, chemin absolu
	

	lseek(rd, t->cur, SEEK_SET); // On reouvre l'archive tar et on positionne un fd à la position donnée par le header, qui a détécté le début des datas. Un autre solution serait de directement passer un buffer venant du processus principal et contenant les datas du fichier. Ici, cela permet au processus principal de continuer à lire les headers sans perdre de temps à mettre les données data dans un buffer, c'est le thread qui le fait

	int s = read(rd, data, t->size); // On enregistre les datas

	close(s);
	close(rd);
	chdir("/home/tux/Documents/rs2016-Martinez-Prono/EXTRACT/"); // test dir

	int f2 = open(t->name, O_CREAT, S_IRUSR | S_IWUSR); // On creer le fichier
	perror("");
	close(f2);

	int fo = open(t->name, O_WRONLY); // On l'ouvre
	int fw = write(fo, data, t->size); // On écrit dedant
	fsync(fw);
	printf("fd file open : %d %p --- fd file write %d %p\n-- INFO --\n%li -- %li -- %s\n-- DATA --\n%s\n", fo, &fo, fw, &fw, t->cur, t->size, t->name, data);
	close(fw);
	close(fo);

};


int write_file(int fd, header_posix_ustar *header) {
	chdir("EXTRACT"); // TEST DIR

	int type = get_type(header); // Get typeflag of the file
	if (type == 0) { // If it's a file, we write file

		int size_data = get_size(header);
		char* filename = get_name(header);
		char data[size_data];
		read(fd, data, size_data);
		int f2 = open(filename, O_CREAT, S_IRUSR | S_IWUSR); // Create File, file rights irusr, iwusr are random for test
		if (f2 < 0) {
			close(f2);
			perror("");
			printf("Bad Create\n");
			return 1;
		}
		close(f2);
		int fo = open(filename, O_WRONLY);
		if (fo >= 0) {
			int fw = write(fo, data, size_data);
			if (fw < 0) {
				close(fw);
				close(fo);
				perror("");
				printf("Bad Write\n");
				return 1;
			}
			fsync(fw);
			close(fw);
			close(fo);
			lseek(fd, (-1)*size_data, SEEK_CUR); // Move back fd to have a right 512k block reading in read_data_block() method
		}
	}
	else if (type == 5) { // If it's a dir, we create the dir
		printf("--------------------\nCREATED DIR\n--------------------\n\n");
		mkdir(get_name(header), 0700); // dir rights 0700 are random for test

	}
	return 0;
}


void read_data_block(int fd, int size_data) {
	block data_bloc;

	while (size_data > 0) {
		int r = read(fd, &data_bloc, BLOCK_SIZE);
		size_data = size_data - r;
	}
}


int read_tar_file(int fd) {
	pthread_t ph_tab[2];
	// Count zeros block at the end of file
	int nb_zeros_blocks = 0;
	int i;
	header_posix_ustar header;

	while ((i = read(fd, &header, BLOCK_SIZE)) == BLOCK_SIZE) {

		if (nb_zeros_blocks >= 2)				// Is it end of tar file ?
			return 0;
		else {									// Is it a zeros bytes block ?
			if (is_empty(&header))
				nb_zeros_blocks++;
			else {								// If not then it is a data block
				display_header(&header);
				if (isx(option)) {
					//write_file(fd, &header);
					// On recupere la position du curseur dans le tar. Cette position indique le debut des datas.
					winfo * x = createinfo(lseek(fd, 0, SEEK_CUR), get_size(&header), get_name(&header)); // On recupere les infos utiles pour le threads
					int type = get_type(&header);
					if (type == 0)
						pthread_create(&ph_tab[0], NULL, hello2, x); // On lance un tread pour écrire le fichier. On fait tout sur le meme  identification pthread_t pour l'instant, c'est pas genant. On perd juste l'identificateur de tous les threads, sauf le dernier thread.
					else if (type == 5) {
						chdir("/home/tux/Documents/rs2016-Martinez-Prono/EXTRACT/"); // LES threads changent le dossier courrant du processus. Pour l'instant, chemin absolu.
						mkdir(get_name(&header), 0700); // Creer un dossier si le header indique un type dossier
					}
				}
				read_data_block(fd, get_size(&header));
			}
		}
	}
	pthread_join(ph_tab[0], NULL);
	return 0;
}

int main(int argc, char *argv[]) {


	option = create_option();
	int statut = -1;
	if (checkoption(argc, argv, option) == 0) {

		if (isx(option))
			mkdir("EXTRACT", 0700); //TEST DIR
		int fd = -1;

		if (argc > 1) {
			fd = open(argv[argc - 1], O_RDONLY, 0);

			if (fd < 0)
				statut = fd;

			statut = read_tar_file(fd);
		}
		else
			statut = -1;

	}
	free(option);

	exit(statut);
}
