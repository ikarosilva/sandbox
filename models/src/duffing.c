/*
 * duffing.c
 *
 *  Created on: Nov 18, 2014
 *      Author: ikaro
 */

/* Model for white blood cell production ( peripheral control in haematopoiesis ) based on:
 *
 * "Pathological Conditions Resulting from Instabilities in Physiological Control Systems"
 * Glass and Mackey, 1979
 *
 */
#include "duffing.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

static char *help_strings[] = {
		"usage: [OPTIONS ...]\n",
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

void duffing(int argc,char* argv[]){

	int N=80;
	char ch;
	while ((ch = getopt(argc,argv,"hN:n:"))!=EOF)
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


	int i;
	double dx=0, x=0, y=0, dy=0, Ts=0.001;
	double F0=0.1;
	double gamma=0.045, a=0.5, b=0.5, omega=2*3.14159265359*0.1417;
	for(i=0;i<N;i++){

		dx=y;
		dy=F0*sin(omega*Ts*i) -gamma*y + a*x -b*x*x*x;

		y= y + Ts*dy;
		x= x + Ts*dx;

		printf("%.6f\n",y);
	}
}
