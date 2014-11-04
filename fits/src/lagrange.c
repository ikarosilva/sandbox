/*
 * lagrange.c
 *
 *  Created on: Nov 4, 2014
 *      Author: Ikaro Silva
 */

#include "lagrange.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Function prototypes. */
long input(void);
/* End of Function prototypes. */

/* Global variables. */
double *input_data;	/* input data buffer; allocated and filled by input() */
long N=0;

static char *help_strings[] = {
		"usage: conway [OPTIONS ...]\n",
		"where OPTIONS may include:",
		" -s n           number of interpolating points between samples",
		" -h             print this usage summary",
		"The standard output is one column.",
		NULL
};

static void help()
{
	int i;

	fprintf(stderr, help_strings[0]);
	for (i = 1; help_strings[i] != NULL; i++)
		fprintf(stderr, "%s\n", help_strings[i]);
	exit(-1);
}

void lagrange(int argc,char* argv[]){

	double step=0.5;
	char ch;
	while ((ch = getopt(argc,argv,"hs:"))!=EOF)
		switch(ch){
		case 's':
			step= (double) 1/atoi(optarg);
			break;
		case 'h':
			help();
			break;
		default:
			fprintf(stderr,"Unknown option for lagrange: '%s'\n",optarg);
			help();
		}

	argc-= optind;
	argv += optind;
	/*Load data into input_data and get the number of samples read*/
	N=input();

	/*Interpolate data with Lagrange polynomials as described in
	 "Numerical Methods in Biomedical Engineering", Dunn et all
	 */
	int n, p;

	/*Get the polynomial coefficients */
	double L[N], den;
	for(n=0;n<N;n++){
		den=1;
		for(p=0;p<N;p++){
			if(n != p){
				den= den * (n - p);
			}
		}
		L[n]=input_data[p]/den;
	}

	/*Process interpolation points */
	double x, y, num;
	for(x=0;x < N;x+=step){
		y=0;
		num=1;
		for(n=0;n<N;n++){
			for(p=0;p<N;p++){
				if(n != p){
					num= num * (x - p);
				}
			}
			y=y + num*L[n];
		}
		fprintf(stdout,"%f \t %f\n",x,y);
	}
}


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
