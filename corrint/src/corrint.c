/*
 ============================================================================
 Name        : corrint.c
 Author      : Ikaro Silva
 Version     :
 Copyright   : GPL
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Function prototypes. */
long input(void);

/* Global variables. */
double *input_data;	/* input data buffer; allocated and filled by input() */


static char *help_strings[] = {
		"usage: corrint [OPTIONS ...]\n",
		"where OPTIONS may include:",
		" -h               print this usage summary",
		"The standard output is one column.",
		"The standard input is one column.",
		NULL
};

static void help()
{
	int i;
	for (i = 0; help_strings[i] != NULL; i++)
		fprintf(stderr, "%s\n", help_strings[i]);
	exit(-1);
}

int main(int argc,char* argv[]) {

	long N=input();
	fprintf(stderr,"Read N=%d\n",N);
	long *corr_int;
	if ((corr_int = malloc(N * sizeof(double))) == NULL) {
		fprintf(stderr,"corrint: insufficient memory, exiting program\n");
		exit(-1);
	}

	//Loop through each data point and get the correlation integral
	long n,m,c;
	for(n=0;n<N;n++){
		corr_int[n]=0;
		for(m=n+1;m<N;m++){
			for(c=n+1;c<m;c++){

			}
		}
	}

	//Free memory allocated by input
	free(input_data);
	return EXIT_SUCCESS;
}

/* Read input data, allocating and filling input_data[].
   The return value is the number of points read.

   This function allows the input buffer to grow as large as necessary, up to
   the available memory (assuming that a long int is large enough to address
   any memory location).
 */
long input()
{
	long maxdat = 0L, npts = 0L;
	double y;
	while (scanf("%lf", &y) == 1) {
		if (npts >= maxdat) {
			double *s;
			maxdat += 50000;	/* allow the input buffer to grow (the increment is arbitrary) */
			if ((s = realloc(input_data, maxdat * sizeof(double))) == NULL) {
				fprintf(stderr,"corrint: insufficient memory, exiting program!");
				exit(-1);
			}
			input_data = s;
		}
		input_data[npts] = y;
		npts++;
	}
	if (npts < 1){
		printf(stderr,"%corrint: Error, no data read!");
		exit(-1);
	}
	return (npts);
}
