/* file: conway.c	 Ikaro Silva	Oct 10, 2014
		  Last revised:       -

-------------------------------------------------------------------------------
conway: TODO:Description
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

#include "conway.h"


void conway(int N){

	double output;
	int a[N];
	int n=0;
	for(;n<N;n++){
		if(n<3){
			a[n]=1;
			output=1;
		}else{
			a[n]=a[a[n-1]]+ a[n-a[n-1]];
			output=(double) a[n] - ((double)n)/2;
		}
		printf("%f\n",output);
	}
}
