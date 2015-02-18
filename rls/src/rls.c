/*
 ============================================================================
 Name        : rls.c
 Author      : Ikaro
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/* Function prototypes. */
/* End of Function prototypes. */


/* Global variables. */
double *input_data;	/* input data buffer; allocated and filled by input() */
long N=0;
long maxLag=0;
double *Rxx; /*autocorrelation of the input data, with length maxLag */
/* End of Global variables. */


static char *help_strings[] = {
		"usage: rls [OPTIONS ...]\n",
		"where OPTIONS may include:",
		" -h               print this usage summary",
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

	//Define the parameters
	int timeLag=2; //Offset with respect only to the first state
	int dim=2;
	char ch;
	//th_arr should be sorted for speed efficiency
	const int countN=19;
	double TH=-1;
	double TH_ARR[19]={0.02, 0.1, 0.2, 0.3, 0.4, 0.5,0.75, 1.25, 1.5, 1.75, 2.0 , 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0};
	double count[countN];
	int neighbors=10;

	for(i=0;i<countN;i++)
		count[i]=0;

	while ((ch = getopt(argc,argv,"hvd:t:s:Rpr:Nn:PSaA"))!=EOF )
		switch(ch){
		case 'v':
			estimateDim=1;
			break;
		case 'n':
			neighbors=atoi(optarg);
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
		case 'P':
			predictSecondHalf=1;
			break;
		case 'S':
			smoothFlag=1;
			break;
		case 'a':
			autoEstimateDim=1;
			break;
		case 'A':
			linearStateStim=1;
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

	/* Define single threshold value if passed by user, otherwise use standard array */
	double* th_arr=TH_ARR;
	if(TH != -1 ){
		*th_arr=TH;
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
	if(estimateDim || (timeLag<0)){
		xcorr();
		for(i=0;i<maxLag;i++){
			if(Rxx[i] <=0 ){
				timeLag=i;
				break;
			}
		}
	}
	if(estimateDim){
		//Give a warning early on if we might not have enough point to reliably
		//estimate the dimension given the embedding parameters
		//This is a conservative minimum (sufficient but not necessary).
		double minPoints= pow(10.0,dim);
		if(N< minPoints)
			fprintf(stderr,"Possibly not have enough points to estimate dimension. Total points: %u, sufficient minimum required: %u\n",N,(long) minPoints);

		//Overwrite other parameters accordingly
		normalizeFlag=1;
		recurFlag=0;

	}

	//Find how many distance points we need to calculate and allocate memory accordingly
	int k, errN=0,test=0;
	//TODO: Find  a way to put this in close form.
	// According to Kaplan and Glass this should be:
	//  (N-(1-dim)*stepSize)(N-(1-dim)*stepSize - 1) - timeLag
	//But we should check this properly

	for(i=N-1;i>=windowN-1;i--,test++)
		for(k=i-(windowN-1)-timeLag;k>=windowN-1;k--,errN++);

	double *err;
	if ((err = malloc(errN * sizeof(double))) == NULL) {
		fprintf(stderr,"corrint: insufficient memory for error matrix, exiting program\n");
		exit(-1);
	}

	//Calculate the error matrix
	get_err(windowN,stepSize,timeLag,err,recurFlag,*th_arr);

	//If in prediction mode, predict based or error matrix and exit from here
	if(predictSecondHalf){
		predictHalf(windowN,stepSize,timeLag,neighbors,linearStateStim,dim);
		exit(0);
	}

	//If in smooth mode, smooth based or error matrix and exit from here
	if(smoothFlag){
		smooth(windowN,stepSize,timeLag,neighbors);
		exit(0);
	}

	//Get neighborhood count
	countNeighbors(th_arr,count,countN,errN,err,normalizeFlag);

	//Display results in column format
	if(!estimateDim){
		for(i=0;i<countN;i++){
			fprintf(stdout,"%f \t %f\n",*(th_arr+i),count[i]);
		}
	}

	//For the dimension estimation case, we also print out the estimate slope value
	double slope=0, slope2=0;
	if(estimateDim){
		if(autoEstimateDim != 1){
			slope=(log(count[1]) - log(count[0]) )/( log(*(th_arr+1)) -log(*th_arr) );
			fprintf(stdout,"%f \t %f\n",*th_arr,slope);
			for(i=1;i<countN-1;i++){
				slope=(log(count[i]) - log(count[i-1]) )/( log(*(th_arr+i)) -log(*(th_arr+i-1)) );
				slope2=(log(count[i+1]) - log(count[i]) )/( log(*(th_arr+i+1)) -log(*(th_arr+i)) );
				fprintf(stdout,"%f \t %f\n",*(th_arr+i),(slope+slope2)/2.0);
			}
			slope=(log(count[countN-1]) - log(count[countN-2]) )/( log(*(th_arr+countN-1)) -log(*(th_arr+countN-2)) );
			fprintf(stdout,"%f \t %f\n",*(th_arr+countN-1),slope);
		}else{
			//For the autoEstimateDim case, the first threshold is r1=signma/4 and the second one is
			//the one that yield a count closest to 5x the first
			//Get variance of the time series
			double* r1=malloc(sizeof(double));
			double* countR1=malloc(sizeof(double));
			double var=0, dc=0;
			for(i=0;i<N;i++){
				var+= input_data[i]*input_data[i];
				dc += input_data[i];
			}
			var= var/((double) N) - dc*dc/( (double) N*N);
			*r1=sqrt(var)/4.0;
			countNeighbors(r1,countR1,1,errN,err,normalizeFlag);

			//Get value that is closest to 5x the r1
			double opt=(*countR1)/5.0, val, best=INFINITY;
			int bestInd=-1;
			for(i=0;i<countN;i++){
				val=abs(opt-count[i]);
				if(val<best){
					best=val;
					bestInd=i;
				}
			}
			if(bestInd <0){
				fprintf(stderr,"Could not find optimal scaling region for series ( std=%f, R1=%f, C(R2)=%f, log(C(R2))=%f ).\n",var,*r1,opt,log(opt));
				exit(1);
			}
			slope=(log(*countR1) - log(count[bestInd]) )/( log(*r1) -log(*(th_arr+bestInd)) );
			fprintf(stdout,"%f \t %f\n",*r1,slope);
		}

		//Print estimated lag
		fprintf(stdout,"lag=%u\n",timeLag);
	}

	//Free memory allocated by input
	free(input_data);
	return EXIT_SUCCESS;
}
