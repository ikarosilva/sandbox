/*
 * white.c
 *
 *  Created on: Nov 16, 2014
 *      Author: ikaro
 */

/*
 * four.c
 *
 *  Created on: Nov 15, 2014
 *      Author: ikaro
 */
#include "white.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

static char *help_strings[] = {
		"usage: one [OPTIONS ...]\n",
		"where OPTIONS may include:",
		" -N n             number of samples to generate",
		" -h               print this usage summary",
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


void white(int argc,char* argv[]){

	int N=80;
	char ch;
	while ((ch = getopt(argc,argv,"hN:"))!=EOF)
		switch(ch){
		case 'N':
			N=atoi(optarg);
			break;
		case 'h':
			help();
			break;
		default:
			fprintf(stderr,"Unknown option for one: '%s'\n",optarg);
			help();
		}

	argc-= optind;
	argv += optind;
	int n=0;
	double x,y;
	for(;n<N;n++){
		x= (rand()+1.0)/(RAND_MAX+1.0);
		y= (rand()+1.0)/(RAND_MAX+1.0);
		x=sqrt(-2*log(y)) * cos(2*PI*x);
		printf("%.6f\n",x);
	}
}