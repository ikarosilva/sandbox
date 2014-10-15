/*
 ============================================================================
 Name        : corrint.c
 Author      : Ikaro Silva
 Version     :
 Copyright   : GPL
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/* Function prototypes. */
long input(void);
void get_err(int windowN, int stepSize,int timeLag, double* err);
void countNeighbors(double *th,unsigned int *count, int countN, int Nerr, double* err);
/* End of Function prototypes. */


/* Global variables. */
double *input_data;	/* input data buffer; allocated and filled by input() */
long N=0;
int debugFlag=0;
/* End of Global variables. */


static char *help_strings[] = {
		"usage: corrint [OPTIONS ...]\n",
		"where OPTIONS may include:",
		" -h               print this usage summary",
		" -D               Debug Flag, if true prints program detail",
		"The standard output is one column.",
		"The standard input is one column.",
		NULL
};

static void help()
{
	int i;
	for (i = 0; help_strings[i] != NULL; i++)
		fprintf(stderr, "%s\n", help_strings[i]);
	exit(-1);
}

int main(int argc,char* argv[]) {

	//Define the parameters of the correlation integral
	int timeLag=2; //Offset with respect only to the first state
	int dim=2;
	char ch;
	int stepSize=1;
	int windowN=dim*stepSize;
	const int countN=6;
	register int i;
	//th_arr should be sorted for speed efficiency
	double th_arr[]={0.02, 0.01, 0.2, 0.3, 0.4,0.5};
	unsigned int count[]={0, 0, 0, 0, 0,0};

	while ((ch = getopt(argc,argv,"hD"))!=EOF )
		switch(ch){
		case 'D':
			debugFlag=1;
			break;
		case 'h':
			help();
			break;
		default:
			fprintf(stderr,"Unknown option: '%s'\n",optarg);
			help();
			break;
		}

	//Load data into input_data and get the number of samples read
	N=input();
	if(debugFlag){
		for(i=0;i<N;i++)
			fprintf(stderr,"data[%d]=%f\n",i,input_data[i]);
	}

	//Find how many distance points we need to calculate and allocate memory accordingly
	int errN;
	int vectorSpan=dim*stepSize;
	int lastPoint=N-vectorSpan;
	int startStatePoint=(lastPoint-timeLag);
	int countMe=0;
	//TODO: Find  a way to put this in close form.
	// N + (N-stepSize) + ... + 1
	for(i=startStatePoint;i>=0;i=i-stepSize){
		if(i-vectorSpan<0){
			break;
		}
		countMe++;
	}

	errN=(lastPoint-timeLag)*countMe;
	if(debugFlag){
		fprintf(stderr,"errN=(lastPoint-timeLag)*countMe=(%d-%d)*%d= %d\n",
				lastPoint,timeLag,countMe,errN);
	}
	double *err;
	if ((err = malloc(errN * sizeof(double))) == NULL) {
		fprintf(stderr,"corrint: insufficient memory for error matrix, exiting program\n");
		exit(-1);
	}

	//Calculate the error matrix
	get_err(windowN,stepSize,timeLag,err);

	//Get neighborhood count
	countNeighbors(th_arr,count,countN,errN,err);

	//Display results in column format
	for(i=0;i<countN;i++){
		fprintf(stdout,"%f \t %d\n",th_arr[i],count[i]);
	}


	//Free memory allocated by input
	free(input_data);
	return EXIT_SUCCESS;
}

//Estimate the distance between the states
void get_err(int windowN, int stepSize,int timeLag, double* err){
	int i, k, z,  index=0;
	double tmpErr;
	//Loop through the data array starting from the end, going to the beginning.
	//For each loop a state vector of size dim and offset timeLag is generated
	if(debugFlag){
		fprintf(stderr,"windowN=%d, stepSize=%d,timeLag=%d\n",windowN,stepSize,timeLag);
	}
	for(i=N-1;i>=windowN-1;i--){
		for(k=i-(windowN-1)-timeLag;k>=windowN-1;k--){
			tmpErr=0;
			for(z=0;z<windowN;z+=stepSize){
				tmpErr+=fabs(input_data[i-z]-input_data[k-z]);
				if(debugFlag){
					fprintf(stderr,"data[%d]-data[%d]\t",(i-z),(k-z),input_data[i-z],input_data[k-z],(input_data[i-z]-input_data[k-z]));
				}
			}
			if(debugFlag){
				fprintf(stderr,"i=%d,k=%d,index=%d, tmpErr=%f\n",i,k,index,tmpErr);
			}
			err[index]=tmpErr;
			index++;
		}
	}
}

//Get the number of states within a minimum threshold
void countNeighbors(double *th,unsigned int *count_arr, int countN, int Nerr, double* err){
	int i,k;
	//Loop through the distance matrix and then go over the th_arr
	//in decreasing order for each element that is below the threshold,
	//incrementing to the count

	//This assumes that th is sorted in *increasing* size of threshold values
	for(i=0;i<Nerr;i++){
		if(debugFlag){
			fprintf(stderr,"count \t\t err[%d]=%f\n",i,err[i]);
		}
		for(k=0;k<countN;k++){
			if(err[i]<th[countN-1-k]){
				count_arr[countN-1-k]++;
				if(debugFlag){
					fprintf(stderr,"count \t\t i=%d,count_arr[%d]=%d\n",i,(countN-1-k),count_arr[countN-1-k]);
				}
			}else{
				//Lowest possible threshold reached, break from th_arr count
				break;
			}
		}
	}

}


/* Read input data, allocating and filling input_data[].
   The return value is the number of points read.

   This function allows the input buffer to grow as large as necessary, up to
   the available memory (assuming that a long int is large enough to address
   any memory location).
 */
long input()
{
	long maxdat = 0L, npts = 0L;
	double y;
	while (scanf("%lf", &y) == 1) {
		if (npts >= maxdat) {
			double *s;
			maxdat += 50000;	/* allow the input buffer to grow (the increment is arbitrary) */
			if ((s = realloc(input_data, maxdat * sizeof(double))) == NULL) {
				fprintf(stderr,"corrint: insufficient memory, exiting program!");
				exit(-1);
			}
			input_data = s;
		}
		input_data[npts] = y;
		npts++;
	}
	if (npts < 1){
		printf(stderr,"%corrint: Error, no data read!");
		exit(-1);
	}
	return (npts);
}
