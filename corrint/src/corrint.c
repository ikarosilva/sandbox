/*
 ============================================================================
 Name        : corrint.c
 Author      : Ikaro Silva
 Version     :
 Copyright   : GPL
 Description : Analysis of Time Series based on Correlation Integral
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/* Function prototypes. */
long input(void);
void get_err(int windowN, int stepSize,int timeLag, double* err, int nFlag, double th);
void countNeighbors(double *th,double *count, int countN, int Nerr, double* err, int nFlag);
void xcorr();
/* End of Function prototypes. */


/* Global variables. */
double *input_data;	/* input data buffer; allocated and filled by input() */
long N=0;
long maxLag=0;
double *Rxx; /*autocorrelation of the input data, with length maxLag */
/* End of Global variables. */


static char *help_strings[] = {
		"usage: corrint [OPTIONS ...]\n",
		"where OPTIONS may include:",
		" -h               print this usage summary",
		" -R               calculates autocorrelation only and exit",
		" -p               generate recurrence data only and exit",
		" -d int           embedded dimension size ",
		" -t int           time lag between states",
		" -s int           time lag within state samples",
		" -r double        distance threshold",
		" -D               Debug Flag, if true prints program detail",
		" -N               Normalize Flag, if true normalize count",
		" -v               Estimates series dimension given an embedded dimension (-d) parameter",
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
	int normalizeFlag=0, corrFlag=0, recurFlag=0, estimateDim=0;
	int windowN;
	register int i;
	//th_arr should be sorted for speed efficiency
	const int countN=12;
	double TH=0;
	double TH_ARR[12]={0.02, 0.1, 0.2, 0.3, 0.4, 0.5, 1.25, 1.5, 2.0 , 2.5, 3.0, 3.5};
	double count[countN];

	for(i=0;i<countN;i++)
		count[i]=0;

	while ((ch = getopt(argc,argv,"hvd:t:s:Rpr:N"))!=EOF )
		switch(ch){
		case 'v':
			estimateDim=1;
			break;
		case 'd':
			dim=atoi(optarg);;
			break;
		case 't':
			timeLag=atoi(optarg);
			break;
		case 's':
			stepSize=atoi(optarg);
			break;
		case 'r':
			TH=atof(optarg);
			break;
		case 'N':
			normalizeFlag=1;
			break;
		case 'R':
			/*Calculate autocorrelation only and exits */
			corrFlag=1;
			break;
		case 'p':
			recurFlag=1;
			break;
		case 'h':
			help();
			break;
		default:
			fprintf(stderr,"Unknown option: '%s'\n",optarg);
			help();
			break;
		}

	//Calculate window size here in case dim is entered by user
	windowN=dim*stepSize;

	/* Define threshold array if not passed by user */
	double* th_arr;
	if(TH == 0 ){
		th_arr=TH_ARR;
	}else{
		th_arr=&TH;
	}
	//Load data into input_data and get the number of samples read
	N=input();


	if(corrFlag){
		/*Calculate autocorrelation and exit */
		xcorr();
		for(i=0;i<maxLag;i++)
			fprintf(stdout,"%f\n",Rxx[i]);
		exit(0);
	}

	//If we are estimating the signal dimension, we need to set the timeLag to the first zero crossing of the autocorrelation function
	//Otherwise it will be either the default or what the user enters
	if(estimateDim){
		xcorr();
		for(i=0;i<maxLag;i++){
			if(Rxx[i] <=0 ){
				timeLag=i;
				break;
			}
		}

		//Give a warning early on if we might not have enough point to reliably
		//estimate the dimension given the embedding parameters
		//This is a conservative minimum (sufficient but not necessary).
		double minPoints= pow(10.0,dim);
		if(N< minPoints)
			fprintf(stderr,"Possibly not have enough points to estimate dimension. Total points: %u, minimum required: %u\n",N,(long) minPoints);

		//Overwrite other parameters accordingly
		normalizeFlag=1;
		recurFlag=0;

	}

	//Find how many distance points we need to calculate and allocate memory accordingly
	int k, errN=0,test=0;
	//TODO: Find  a way to put this in close form.
	// First loop: T=(N-windowN+1)
	// Second loop: K= ?
	for(i=N-1;i>=windowN-1;i--,test++)
		for(k=i-(windowN-1)-timeLag;k>=windowN-1;k--,errN++);

	double *err;
	if ((err = malloc(errN * sizeof(double))) == NULL) {
		fprintf(stderr,"corrint: insufficient memory for error matrix, exiting program\n");
		exit(-1);
	}

	//Calculate the error matrix
	get_err(windowN,stepSize,timeLag,err,recurFlag,*th_arr);

	//Get neighborhood count
	countNeighbors(th_arr,count,countN,errN,err,normalizeFlag);

	//Display results in column format
	if(!estimateDim){
		for(i=0;i<countN;i++){
			fprintf(stdout,"%f \t %f\n",th_arr[i],count[i]);
		}
	}
	//For the dimension estimation case, we also print out the estimate slope value
	if(estimateDim){
		for(i=0;i<countN;i++){
			/* Ignore cases where log(0) */
			if(count[i]==0){
				continue;
			}
			fprintf(stdout,"%f \t %f\n",th_arr[i],log(count[i])/log(th_arr[i]));
		}
		fprintf(stdout,"lag=%u\n",timeLag);
	}

	//Free memory allocated by input
	free(input_data);
	return EXIT_SUCCESS;
}

//Estimate the distance between the states
void get_err(int windowN, int stepSize,int timeLag, double* err, int recurFlag, double th){
	int i, k, z,  index=0;
	double tmpErr;
	//Loop through the data array starting from the end, going to the beginning.
	//For each loop a state vector of size dim and offset timeLag is generated

	for(i=N-1;i>=windowN-1;i--){
		for(k=i-(windowN-1)-timeLag;k>=windowN-1;k--){
			tmpErr=0;
			for(z=0;z<windowN;z+=stepSize){
				tmpErr+=fabs(input_data[i-z]-input_data[k-z]);
			}
			err[index]=tmpErr;

			//for recurrence plots, print any state that has at least one neighbor and exit
			if(recurFlag == 1){
				if(err[index]<th){
					fprintf(stdout,"%u\t%u\n",i,k);
					fprintf(stdout,"%u\t%u\n",k,i);
				}
			}
			index++;
		}
	}
	if(recurFlag){
		exit(0);
	}
}

//Get the number of states within a minimum threshold
void countNeighbors(double *th,double *count_arr, int countN, int Nerr, double* err,int nFlag){
	register int i,k;
	//Loop through the distance matrix and then go over the th_arr
	//in decreasing order for each element that is below the threshold,
	//incrementing to the count

	//This assumes that th is sorted in *increasing* size of threshold values
	for(i=0;i<Nerr;i++){
		for(k=0;k<countN;k++){
			if(err[i]<th[countN-1-k]){
				count_arr[countN-1-k]++;
			}else{
				//Lowest possible threshold reached, break from th_arr count
				break;
			}
		}
	}

	//Normalize the count
	if(nFlag==1){
		for(k=0;k<countN;k++)
			count_arr[k]=count_arr[k]/(double) Nerr;
	}

}


/* Estimate autocorrelation */
void xcorr(){
	volatile long lag, n;
	double R, R0;
	maxLag=N/2;
	Rxx=malloc(maxLag * sizeof(double));

	/*Subtract mean from the time series */
	double dc=0;
	for(n=0;n<N;n++)
		dc+=input_data[n];
	dc=(double) dc/N;

	for(n=0;n<N;n++)
		input_data[n]=input_data[n]-dc;

	for(lag=0;lag<maxLag;lag++){
		for(n=0,R=0,R0=0;n<N-maxLag;n++){
			R+= input_data[n+lag]*input_data[n];
			R0+=input_data[n]*input_data[n];
		}
		Rxx[lag]=R/R0;
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
		printf(stderr,"Error, no data read!");
		exit(-1);
	}
	return (npts);
}
