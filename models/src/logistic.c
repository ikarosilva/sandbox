
/* file: logistic.c	 Ikaro Silva	Oct 10, 2014
		  Last revised:       -

-------------------------------------------------------------------------------
logistic: TODO:Description
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

#include "logistic.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "white.h"

static char *help_strings[] = {
		"usage: logistic [OPTIONS ...]\n",
		"where OPTIONS may include:",
		" -N n             number of samples to generate",
		" -n pow           measurement noise power (default =0 )",
		" -s pow           state noise power (default =0 )",
		" -h               print this usage summary",
		"The standard output is one column.",
		NULL
};

static void help()
{
	int i;
	for (i = 0; help_strings[i] != NULL; i++)
		fprintf(stderr, "%s\n", help_strings[i]);
	exit(-1);
}

void logistic(int argc,char* argv[]){

	double x=0.2;
	double u=4.0;
	int N=10;
	double n=0, s=0;

	char ch;
	while ((ch = getopt(argc,argv,"hN:u:x:n:s:"))!=EOF)
		switch(ch){
		case 'N':
			N=atoi(optarg);
			break;
		case 'u':
			u=atoi(optarg);
			break;
		case 'x':
			x=atoi(optarg);
			break;
		case 'h':
			help();
			break;
		case 'n':
			n=atof(optarg);
			break;
		case 's':
			s=atof(optarg);
			break;
		default:
			fprintf(stderr,"Unknown option for logistic: '%s'\n",optarg);
			help();
		}

	argc-= optind;
	argv += optind;

	int i;
	double msr=0;
	for(i=0;i<N;i++){
		if(n !=0 ){
			msr=randn(0.0,n);
		}
		if( s == 0){
			x=u*x*(1-x);
		}else{
			x= x + randn(0.0,s);
		}
		printf("%.6f\n",x+msr);

	}
}
