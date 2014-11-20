/*
 ============================================================================
 Name        : visbility.c
 Author      : Ikaro Silva
 Version     :
 Copyright   : GPL
 Description : Visibility Graph
 ============================================================================
 */


/*
 * one.c
 *
 *  Created on: Nov 15, 2014
 *      Author: ikaro
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

long input(void);

/* Global variables. */
long N=0;
double *input_data;	/* input data buffer; allocated and filled by input()
 	 	 	 	 	    x and y series will be interleaved starting with x
 	 	 	 	 	 */


static char *help_strings[] = {
 "usage: visibility [OPTIONS ...]\n",
 "where OPTIONS may include:",
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



int main(int argc,char* argv[]){

	char ch;
	while ((ch = getopt(argc,argv,"h"))!=EOF)
		switch(ch){
		case 'h':
			help();
			break;
		default:
			fprintf(stderr,"Unknown option for one: '%s'\n",optarg);
			help();
		}

	argc-= optind;
	argv += optind;

	N=input();

	int a=0,b=0,c=0;
	double ya, yb, yc;
	double ta, tb, tc;
	long samples=N/2; //data is interleaved
	int* count=malloc(samples*sizeof(int));
	double* Pk=malloc(samples*sizeof(double));

	fprintf(stderr,"N=%u\n",N);
	//Calculate the visibility count
	int isVisible=1;
	for(a=0;a<N-1;a=a+2){
		*(count+a)=0;
		ta=*(input_data+a);
		ya=*(input_data+a+1);
		for(c=a+4;c<N;c=c+2){
			isVisible=1;
			tc=*(input_data+c);
			yc=*(input_data+c+1);
			for(b=a+2;b<c;b=b+2){
				tb=*(input_data+b);
				yb=*(input_data+b+1);
				if ( yc > (yb + (ya -yb)*(tb-tc)/(tb-ta)) ){
					//Visibility broken, exit loop inner loop only
					isVisible=0;
					break;
				}
			}
			if(isVisible==1){
				//C is visible to A. Add count to both nodes
				*(count+a)= *(count+a) + 1;
				*(count+c)= *(count+c) + 1;
			}
		}
		//Add to the degree distribution P(k)
		*(Pk + *(count+a) ) = *( Pk + *(count+a)) + 1 ;
	}


	//Normalize Pk by the total number of nodes
	for(a=0;a<samples;a++)
		*(Pk + a) = *(Pk + a)/samples ;

	//Output the degree distribution
	for(a=0;a<samples;a++){
		printf("%u %f\n",a,Pk);
	}

	return 0;
}



long input()
{
	long maxdat = 0L, npts = 0L;
	double x,y;
	while (scanf("%f %f",&x,&y) == 1) {
		if (npts >= maxdat) {
			double *s;
			maxdat += 50000;	/* allow the input buffer to grow (the increment is arbitrary) */
			if ((s = realloc(input_data, maxdat * sizeof(double))) == NULL) {
				fprintf(stderr,"visibility: insufficient memory, exiting program!");
				exit(-1);
			}
			input_data = s;
		}
		input_data[npts] = x;
		npts++;
		input_data[npts] = y;
		npts++;
	}
	if (npts < 1){
		printf(stderr,"Error, no data read!");
		exit(-1);
	}
	return (npts);
}
