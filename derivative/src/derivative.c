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
#include "central.h"

/* Global variables. */
static char *help_strings[] = {
		"usage: models [OPTIONS ...]\n",
		"where OPTIONS may include:",
		" -f int           Number of model to run. Options are:",
		"                  0 - Central difference derivative",
		" -h               print this usage summary",
		"The standard output is one column.",
		NULL
};

static void help()
{
	int i;

	(void)fprintf(stderr, help_strings[0]);
	for (i = 1; help_strings[i] != NULL; i++)
		fprintf(stderr, "%s\n", help_strings[i]);
	exit(-1);
}

int main(int argc,char* argv[]) {

	char ch;
	int index=0;
	int flag=1;

	//Define array of pointers to functions that can be called
	//This should map to the "-f int" parameter of the executable
	const int indMax=1;
	void (*funcTable[1])(int, char*)={central};

	while (flag && (ch = getopt(argc,argv,"hf:"))!=EOF )
		switch(ch){
		case 'f':
			index=atoi(optarg);
			flag=0;
			break;
		case 'h':
			help();
			break;
		default:
			fprintf(stderr,"Unknown option: '%s'\n",optarg);
			help();
			break;
		}

	if(index> (indMax-1) ){
		fprintf(stderr,"Unknown option for f: '%s'\n",index);
		help();
	}else {
		funcTable[index](argc,argv);
	}
	return EXIT_SUCCESS;
}
