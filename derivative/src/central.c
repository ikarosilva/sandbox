/*
 * central.c
 *
 *  Created on: Nov 4, 2014
 *      Author: Ikaro Silva
 */

#include "central.h"
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
		" -p n          Nth derivative ( 1 <= n <= 4 )",
		" -s d          sampling interval time of input series",
		" -h            print this usage summary",
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

void central(int argc,char* argv[]){

	int p=1, Ts;
	char ch;
	while ((ch = getopt(argc,argv,"hp:s:"))!=EOF)
		switch(ch){
		case 'p':
			p= atoi(optarg);
			break;
		case 's':
			Ts= atof(optarg);
			break;
		case 'h':
			help();
			break;
		default:
			fprintf(stderr,"Unknown option for central: '%s'\n",optarg);
			help();
		}

	argc-= optind;
	argv += optind;
	/*Load data into input_data and get the number of samples read*/
	N=input();

	/*Estimate Derivatives using Central Difference Method from
	 *  "Numerical Methods in Biomedical Engineering", Dunn et all
	 *
	 *	With error of order O(Ts)^4
	 *  */

	int n, dx, xdiff;
	if(p==1){
		xdiff=1/(12*Ts);
		for(n=2;n<(N-2);n++){
			dx= xdiff*( -input_data[n+2] + 8*input_data[n+1] - 8*input_data[n-1] + input_data[n-2] );
			fprintf(stdout,"%f \t %f\n",n*Ts,dx);
		}
	}else if(p==2){
		xdiff=1/(12*Ts*Ts);
		for(n=1;n<(N-1);n++){
			dx= xdiff*( -input_data[n+2] + 16*input_data[n+1] - 30*input_data[n] + 16*input_data[n-1] - input_data[n-2] );
			fprintf(stdout,"%f \t %f\n",n*Ts,dx);
		}
	}else if(p==3){
		xdiff=1/(8*Ts*Ts*Ts);
		for(n=2;n<(N-2);n++){
			dx= xdiff*( -input_data[n+3] + 8*input_data[n+2] - 13*input_data[n+1] + 13*input_data[n-1] - 8*input_data[n-2] + input_data[n-3] );
			fprintf(stdout,"%f \t %f\n",n*Ts,dx);
		}
	}else if(p==4){
		xdiff=1/(6*Ts*Ts*Ts*Ts);
		for(n=1;n<(N-1);n++){
			dx= xdiff*(-input_data[n+3] + 12*input_data[n+2] - 39*input_data[n+1] + 56*input_datap[n] - 39*input_data[n-1] + 12*input_data[n-2] - input_data[n-3] );
			fprintf(stdout,"%f \t %f\n",n*Ts,dx);
		}
	}else{
		fprintf(stderr,"Unknown derivative order: %f",p);
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
