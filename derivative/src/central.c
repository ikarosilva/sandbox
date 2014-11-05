/*
 * central.c
 *
 *  Created on: Nov 4, 2014
 *      Author: Ikaro Silva
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "derivative.h"
#include "central.h"

void central(int argc,char* argv[]){

	static char *help_strings[] = {
			"usage: central [OPTIONS ...]\n",
			"where OPTIONS may include:",
			" -p n          Nth derivative ( 1 <= n <= 4 )",
			" -s d          sampling interval time of input series",
			" -h            print this usage summary",
			"The standard output is one column.",
			NULL
	};

	int p=1;
	double Ts;
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

	/*Estimate Derivatives using Central Difference Method from
	 *  "Numerical Methods in Biomedical Engineering", Dunn et all
	 *
	 *	With error of order O(Ts)^4
	 *  */

	int n;
	double dx, xdiff;
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
			dx= xdiff*(-input_data[n+3] + 12*input_data[n+2] - 39*input_data[n+1] + 56*input_data[n] - 39*input_data[n-1] + 12*input_data[n-2] - input_data[n-3] );
			fprintf(stdout,"%f \t %f\n",n*Ts,dx);
		}
	}else{
		fprintf(stderr,"Unknown derivative order: %u",p);
	}
}
