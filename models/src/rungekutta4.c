/* file: rungekutta4.c	 Ikaro Silva	November2, 2014
		  Last revised:       -

-------------------------------------------------------------------------------
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

 double* integrate(double t0, long N, double h, double x0, double (*dx)(double t, double x)){

	 double *y=malloc(sizeof(double)*N);
	 if(y== NULL){
		 fprintf(stderr,"Insufficient memory");
		 exit(1);
	 }

	 y[0]=y0;
	 double k1, k2, k3, k4, t=t0;

	 for(int n=1;n<N;n++){

		 k1=h*dx(t,y[n-1]);
		 k2=h*dx(t+ h/2.0,y[n-1]+ k1/2.0);
		 k3=h*dx(t+ h/2.0,y[n-1]+ k2/2.0);
		 k4=h*dx(t+ h,y[n-1]+ k3);

		 y[n]=y[n-1] + (k1 + 2*k2 + 2*k3 + k4)/6.0;
		 t=t+h;

	 }

	 return y;

 }

