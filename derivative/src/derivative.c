/*
 ============================================================================
 Name        : derivative.c
 Author      : Ikaro Silva
 Version     :
 Copyright   : Copyright 2014
  ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "derivative.h"
#include "central.h"
#include "backward.h"

void help(char *help_strings[])
{
	int i;

	(void)fprintf(stderr, help_strings[0]);
	for (i = 1; help_strings[i] != NULL; i++)
		fprintf(stderr, "%s\n", help_strings[i]);
	exit(-1);
}

long input(double *input_data){
	long maxdat = 0L, npts = 0L;
	double y;
	while (scanf("%lf", &y) == 1) {
		if (npts >= maxdat) {
			double *s;
			maxdat += 50000;	/* allow the input buffer to grow (the increment is arbitrary) */
			if ((s = realloc(input_data, maxdat * sizeof(double))) == NULL) {
				fprintf(stderr,"insufficient memory, exiting program!");
				exit(-1);
			}
			input_data = s;
		}
		input_data[npts] = y;
		npts++;
	}
	if (npts < 1){
		fprintf(stderr,"Error, no data read!");
		exit(-1);
	}
	return (npts);
}


int main(int argc,char* argv[]) {

	static char *help_strings[] = {
			"usage: models [OPTIONS ...]\n",
			"where OPTIONS may include:",
			" -f int           Number of model to run. Options are:",
			"                  0 - Central difference derivative",
			"                  1 - Backward difference derivative",
			" -h               print this usage summary",
			"The standard output is one column.",
			NULL
	};

	char ch;
	int index=0;
	int flag=1;
	//Define array of pointers to functions that can be called
	//This should map to the "-f int" parameter of the executable
	const int indMax=2;
	void (*funcTable[2])(int, char*)={central, backward};

	while (flag && (ch = getopt(argc,argv,"hf:"))!=EOF )
		switch(ch){
		case 'f':
			index=atoi(optarg);
			flag=0;
			break;
		case 'h':
			help(help_strings);
			break;
		default:
			fprintf(stderr,"Unknown option: '%s'\n",optarg);
			help(help_strings);
			break;
		}

	if(index> (indMax-1) ){
		fprintf(stderr,"Unknown option for f: '%s'\n",index);
		help(help_strings);
	}else {
		funcTable[index](argc,argv);
	}
	return EXIT_SUCCESS;
}
