/*
 ============================================================================
 Name        : queueMe.c
 Author      : Ikaro Silva (ikaro@mit.edu)
 Version     : 1.0
 Copyright   : GPL
 Modified	 : NA
 Compilation :
 	 	 	 	 To compile with GCC run:
 	 	 	 	 	 	 gcc -Wall QueueMe.c -o QueueMe

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char *help_strings[] = {
		"usage: queueMe -N n [OPTIONS ...]\n",
		"where arguments are:",
		"-N n             number of cards in the deck ( integer: 0< n < 2147483647 )",
		"-v n			  (optional) verbose level (integer): 0 (quiet), 1 ( verbose ), 2 (very verbose)",
		"-L n			  (optional) set maximum limit on number of rounds for the simulation (integer >0)",
		"-h               print this usage summary",
		NULL
};

static void help()
{
	int i;
	for (i = 0; help_strings[i] != NULL; i++)
		fprintf(stderr, "%s\n", help_strings[i]);
	exit(-1);
}

typedef struct node {
	long val;
	struct node *restp;
} node;

typedef struct queue{
	node *frontp, *rearp;
	long size;
} queue;

void push_back(queue *qp,long new_val){
	//push only adds longs to the end of the list
	if (qp->size ==0){
		//Add to empty queue
		qp->rearp = (node *) malloc(sizeof (node));
		qp->frontp = qp->rearp;
	}else{
		qp->rearp->restp= (node *) malloc(sizeof(node));
		qp->rearp= qp->rearp->restp;
	}
	qp->rearp->val=new_val;
	qp->rearp->restp = NULL;
	++(qp->size);
}

void push_front(queue *qp,long new_val){
	//Only push to the front of the queue
	if (qp->size ==0){
		//Add to empty queue
		qp->rearp = (node *) malloc(sizeof (node));
		qp->frontp = qp->rearp;
		qp->rearp->val=new_val;
		qp->rearp->restp = NULL;
	}else{
		node* new_frontp=(node *) malloc(sizeof(node));
		new_frontp->restp=qp->frontp;
		new_frontp->val=new_val;
		qp->frontp=new_frontp;
	}
	++(qp->size);
}


long pop_back(queue *qp){
	//pop only removes elements from the start of the list
	long popval;
	if (qp->size > 0){
		node *to_freep;
		popval=qp->frontp->val;

		to_freep= qp->frontp;
		qp->frontp= to_freep->restp;
		free(to_freep);
		--(qp->size);

		if(qp->size ==0)
			qp->rearp=NULL; //Queue is empty
	}else{
		//Attempting to pop empty queue
		popval=0;
	}
	return popval;
}

void print_queue(queue *qp){
	long n;
	if(qp->size>0){
		node* this_nodep= qp->frontp;
		fprintf(stdout, " ( Top ) ");
		for(n=0;n< (qp->size)-1;n++){
			fprintf(stdout, "[%lu]->", this_nodep->val);
			this_nodep=this_nodep->restp;
		}
		fprintf(stdout, "[%lu] ( Bottom )\n", this_nodep->val);
	}
}

int main(int argc,char* argv[]) {

	long N=0;
	long MAXN=2147483647; //Based on program using long as the datatype for the deque

	//Array to check if deque is sorted
	//First value is the most recent past value added to deque.
	//If the array is sorted each sequential difference is of exactly +1. We assume the array is sorted at first, but as soon as any incremental difference != 1 is detected,
	//we set the array to not sorted ( is_sorted[1]=0).
	long is_sorted[2]={0L,1L};
	int verbose=0;
	long i, round=0, to_add_val=0;
	long limit =1000000; //Chosen arbitrarily....
	char ch;
	while ((ch = getopt(argc,argv,"hN:v:L:"))!=EOF)
		switch(ch){
		case 'N':
			N=atoi(optarg);
			break;
		case 'L':
			limit=atoi(optarg);
			break;
		case 'v':
			verbose=atoi(optarg);
			break;
		case 'h':
			help();
			break;
		default:
			fprintf(stderr,"Unknown option for QueueMe: '%s'\n",optarg);
			help();
		}

	argc-= optind;
	argv += optind;

	if(N<1 || N>MAXN){
		fprintf(stderr,"Number of cards (N) has to be an integer: 0 < N < %lu (see help: QueueMe -h)\n",MAXN);
		return EXIT_FAILURE;
	}

	if(verbose>0)
		fprintf(stdout,"Running simulation for deck of %lu cards ...\n",N);

	//Use to Queues, alternating between them for hand and table
	queue Q1={NULL,NULL,0};
	queue Q2={NULL,NULL,0};
	queue tmpQ;

	//Initialize Q1, so that hand has with a full ordered deque
	for(i=1;i<=N;i++)
		push_back(&Q1,i);

	if(verbose){
		fprintf(stdout,"--Round %lu, Hand Deque: ",round);
		print_queue(&Q1);
	}

	while(1){

		is_sorted[0]=0L;
		is_sorted[1]=1L; //Assume deque is sorted until we find incremental difference > 1 between elements

		//Begin A shuffling round
		while(Q1.size>0){
			N=Q1.size;

			//Shuffle according to problem statement
			for(i=1;i<=N;i++){
				if((i % 2)==0){
					push_back(&Q1,pop_back(&Q1));
				}else{
					to_add_val=pop_back(&Q1);
					if( ( (is_sorted[0]-to_add_val) != 1 ) && Q2.size>0)
						is_sorted[1]=0L;
					push_front(&Q2,to_add_val);
					is_sorted[0]=to_add_val;
				}
			}

			//For odd deque numbers, we need to do one additional pop on the hand deque
			if(((N % 2) != 0) && Q1.size >0 )
				push_back(&Q1,pop_back(&Q1));

			if(verbose>1){
				fprintf(stdout,"\tShuffling: \n");
				fprintf(stdout,"\t\t Table:\t");
				print_queue(&Q2);
				fprintf(stdout,"\t\t Hand:\t");
				print_queue(&Q1);
			}
		} //End of one shuffling round

		round++;
		if(verbose){
			fprintf(stdout,"\n--Round %lu, Hand Deque: ",round);
			print_queue(&Q2);
		}

		//Check if deque is sorted and is_sorted[0] !=0 (ie, we have passed at least once through the current deque, which should have positive integers only)
		if( ( is_sorted[1]==1 ) && ( is_sorted[0] !=0 ))
			break;

		if(round>limit){
			fprintf(stderr,"\n***Maximum Round limit of %lu reached (see help on how to change limit: QueueMe -h). Exiting Simulation !!\n",limit);
			return EXIT_FAILURE;
		}

		//Swap Queues (put table deque on hand) and start over
		tmpQ=Q1;
		Q1=Q2;
		Q2=tmpQ;
	} // End of infinite loop that shuffles and checks deque order


	fprintf(stdout,"***Simulation Complete. Total number of rounds required: %lu\n",round);
	return EXIT_SUCCESS;
}
