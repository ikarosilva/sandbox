/* file: neuralimpulse.c	 Ikaro Silva	Oct 31, 2014
		  Last revised:       -

-------------------------------------------------------------------------------
nerualimpulse: Fitzhugh-Nagumo Model for Neural Impulse with a small stochastic input
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

#include "neuralimpulse.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

static char *help_strings[] = {
		"usage: neuralimpulse [OPTIONS ...]\n",
		"where OPTIONS may include:",
		" -N n             number of samples to generate",
		" -s double        simulation step size         ",
		" -p double        power of stochastic input    ",
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
double getGaussian(double mean, double std)
{
	double x= (rand()+1.0)/(RAND_MAX+1.0);
	x=sqrt(-2*log(x)) * cos(2*PI*x);
	return (x+mean)*std;
}

void neuralimpulse(int argc,char* argv[]){

	double x=-0.128;
	double y=0, z=0;
	double dy=0, dx=0;
	int N=10;
	double s=0.01; /*step size*/
	/*Parameters of the model */
	double a=0.7, b=0.8, c=0.3, zm=-0.064, zpow=0.00001;

	char ch;
	while ((ch = getopt(argc,argv,"hN:u:s:p:"))!=EOF)
		switch(ch){
		case 'N':
			N=atoi(optarg);
			break;
		case 's':
			s=atoi(optarg);
			break;
		case 'p':
			zpow=atoi(optarg);
			break;
		case 'h':
			help();
			break;
		default:
			fprintf(stderr,"Unknown option for logistic: '%s'\n",optarg);
			help();
		}

	argc-= optind;
	argv += optind;
    zpow=sqrt(zpow);

	int i;
	for(i=0;i<N;i++){
		z=getGaussian(zm,zpow);
		dx=c*(z + y + x - (x*x*x)/3.0 );
		dy=-(x-a+b*y)/c;
		x=x + (s*dx);
		y=y + (s*dy);
		printf("%.6f\n",x);
	}
}





