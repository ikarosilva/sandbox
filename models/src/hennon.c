
/* file: logistic.c	 Ikaro Silva	Oct 10, 2014
		  Last revised:       -

-------------------------------------------------------------------------------
hennon: TODO:Description
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

#include "hennon.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "white.h"

static char *help_strings[] = {
		"usage: hennon [OPTIONS ...]\n",
		"where OPTIONS may include:",
		" -N n             number of samples to generate",
		" -n pow           measurement noise power (default =0 )",
		" -s pow           state noise power (default =0 )",
		" -a a             hennon parameter (default= 1.4 )",
		" -b b             hennon parameter (default= 0.3 )",
		" -x x0			   initial condition for x(default 0.2)",
		" -y y0            initial condition for y (default 0.2)"
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

void hennon(int argc,char* argv[]){

	double x=0.2, y=0.2, a=1.4, b=0.3;
	int N=10;
	double n=0, s=0;

	char ch;
	while ((ch = getopt(argc,argv,"hN:a:b:x:y:n:s:"))!=EOF)
		switch(ch){
		case 'N':
			N=atoi(optarg);
			break;
		case 'a':
			a=atof(optarg);
			break;
		case 'b':
			b=atof(optarg);
			break;
		case 'x':
			x=atof(optarg);
			break;
		case 'y':
			y=atof(optarg);
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
			fprintf(stderr,"Unknown option for hennon: '%s'\n",optarg);
			help();
		}
	argc-= optind;
	argv += optind;

	int i;
	double msr=0, xold=0, yold=0;
	for(i=0;i<N;i++){
		if(n !=0 ){
			msr=randn(0.0,n);
		}
		xold=x;
		yold=y;
		x=a - xold*xold + b*yold;
		y=xold;

		printf("%.6f\n",x);

	}
}
