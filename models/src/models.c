/* file: tes.c	 Ikaro Silva	Oct 10, 2014
		  Last revised:       -

-------------------------------------------------------------------------------
tes: TODO:Description
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

/* Global variables. */
static char *help_strings[] = {
 "usage: models [OPTIONS ...]\n",
 "where OPTIONS may include:",
 " -n name          name of model to run",
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
	char *name;
	int flag=1;
	while (flag && (ch = getopt(argc,argv,"hn:"))!=EOF )
		switch(ch){
		case 'n':
			name=optarg;
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

	conway(argc,argv);
	return EXIT_SUCCESS;
}
