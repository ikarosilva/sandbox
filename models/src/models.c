/* file: models.c	 Ikaro Silva	Oct 10, 2014
		  Last revised:       -

-------------------------------------------------------------------------------
models: Main entry point for generating 1-D signals from specific process models.
Copyright (C) 2014 Ikaro Silva

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA.

You may contact the author by e-mail (ikaro@mit.edu).
_______________________________________________________________________________
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "conway.h"
#include "logistic.h"

/* Global variables. */
static char *help_strings[] = {
		"usage: models [OPTIONS ...]\n",
		"where OPTIONS may include:",
		" -f int           Number of model to run",
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
	const int indMax=2;
	void (*funcTable[2])(int, char*)={ conway, logistic};

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
