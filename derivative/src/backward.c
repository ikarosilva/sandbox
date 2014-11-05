/*
 * forward.c
 *
 *  Created on: Nov 4, 2014
 *      Author: Ikaro Silva
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "derivative.h"
#include "backward.h"

void backward(int argc,char* argv[]){

	static char *help_strings[] = {
			"usage: backward [OPTIONS ...]\n",
			"where OPTIONS may include:",
			" -p n          Nth derivative ( 1 <= n <= 4 )",
			" -s d          sampling interval time of input series",
			" -h            print this usage summary",
			"The standard output is one column.",
			NULL
	};

	int p=1, Ts;
	char ch;
	double *input_data;	/* input data buffer; allocated and filled by input() */
	long N=0;

	while ((ch = getopt(argc,argv,"hp:s:"))!=EOF)
		switch(ch){
		case 'p':
			p= atoi(optarg);
			break;
		case 's':
			Ts= atof(optarg);
			break;
		case 'h':
			help(help_strings);
			break;
		default:
			fprintf(stderr,"Unknown option for central: '%s'\n",optarg);
			help(help_strings);
		}

	argc-= optind;
	argv += optind;
	/*Load data into input_data and get the number of samples read*/
	N=input(input_data);

	/*Estimate Derivatives using Backward Difference Method from
	 *  "Numerical Methods in Biomedical Engineering", Dunn et all
	 *
	 *	With error of order O(Ts)^2
	 *  */

	int n, dx, xdiff;
	if(p==1){
		xdiff=1/(2*Ts);
		for(n=2;n<N;n++){
			dx= xdiff*( 3*input_data[n] - 4*input_data[n-1] + input_data[n-2]);
			fprintf(stdout,"%f \t %f\n",n*Ts,dx);
		}
	}else if(p==2){
		xdiff=1/(Ts*Ts);
		for(n=3;n<N;n++){
			dx= xdiff*(2*input_data[n] - 5*input_data[n-1] + 4*input_data[n-2] - input_data[n-3]);
			fprintf(stdout,"%f \t %f\n",n*Ts,dx);
		}
	}else if(p==3){
		xdiff=1/(2*Ts*Ts*Ts);
		for(n=4;n<N;n++){
			dx= xdiff*( 5*input_data[n] - 18*input_data[n-1] + 24*input_data[n-2] - 14*input_data[n-3] + 3*input_data[n-4] );
			fprintf(stdout,"%f \t %f\n",n*Ts,dx);
		}
	}else if(p==4){
		xdiff=1/(Ts*Ts*Ts*Ts);
		for(n=5;n<N;n++){
			dx= xdiff*(3*input_data[n] - 14*input_data[n-1] + 26*input_data[n-2] - 24*input_data[n-3] + 11*input_data[n-4] - 2*input_data[n-5] );
			fprintf(stdout,"%f \t %f\n",n*Ts,dx);
		}
	}else{
		fprintf(stderr,"Unknown derivative order: %f",p);
	}
}
