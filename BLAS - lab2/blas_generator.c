#include<stdio.h>
#include<stdlib.h>
#include<gsl/gsl_interp.h>
#include<gsl/gsl_blas.h>
#include<time.h>
#include<sys/times.h>
#include<string.h>
#include<sys/stat.h>

#define BILLION 1000000000L

void initialize_csv(char* filename){
		FILE* fp;
		fp=fopen(filename,"w+");
		fprintf(fp,"Rozmiar wektora, Czas mnożenia wektorów, Czas mnożenia wektora przez macierz");
		fclose(fp);
}

void create_csv(char* filename, double tab[][3],int x, int y){

		FILE* fp;
		int i,j;
		fp=fopen(filename,"a+");
		fseek(fp,0,SEEK_END);
		for(i=0;i<x;i++){
			fprintf(fp,"\n");
			for(j=0;j<y;j++){
				if(j!=0) fprintf(fp,",");
				fprintf(fp,"%f",tab[i][j]);
			}
		}
		fclose(fp);
}

int main(){

		char filename [13] = "time_csv.csv";
		initialize_csv(filename);
		int seed;
		time_t t;
		seed = time(&t);
		srand(seed);
		int k;

		int N;
		long size;
		long long multiplier=10;
		double tim [10][3];


	for(k=0;k<=1000;k+=20){
	if(k==0) size=1;
	else size=k;
	gsl_vector* a = gsl_vector_calloc(size);
	gsl_vector* b = gsl_vector_calloc(size);

	struct timespec matrix_start, matrix_stop, vector_start, vector_stop;

	int i;
	for(i=0;i<size;i++){
		gsl_vector_set(a,i,rand()%10000);
		gsl_vector_set(b,i,rand()%10000);
	}

	gsl_matrix* m = gsl_matrix_calloc(1000,size);
	gsl_vector* c = gsl_vector_calloc(1000);
		
	int w;
	for(i=0;i<1000;i++){
		gsl_vector_set(c,i,rand()%1000);
		for(w=0;w<size;w++)
			gsl_matrix_set(m,i,w,rand()%1000);
	}

	N=1;
	while(N<11){

		
		if(clock_gettime(CLOCK_REALTIME, &vector_start)==-1) exit(EXIT_FAILURE);

		double product;
		gsl_blas_ddot(a,b,&product);
		printf("Mnożenie dwóch wektorów : %lf \n",product);

		if(clock_gettime(CLOCK_REALTIME, &vector_stop)==-1) exit(EXIT_FAILURE);

		/*----------------------------------*/
	
		if(clock_gettime(CLOCK_REALTIME, &matrix_start)==-1) exit(EXIT_FAILURE);

		gsl_blas_dgemv(CblasNoTrans,1.0,m,a,1.0,c);
		printf("Mnożenie wektora przez macierz : [%lf, %lf]\n", gsl_vector_get(c,0), gsl_vector_get(c,1));

		if(clock_gettime(CLOCK_REALTIME, &matrix_stop)==-1) exit(EXIT_FAILURE);

		tim[N-1][0]=size;
		tim[N-1][1]=(double)((vector_stop.tv_sec-vector_start.tv_sec)*BILLION+(vector_stop.tv_nsec - vector_start.tv_nsec))/BILLION;
		tim[N-1][2]=(double)((matrix_stop.tv_sec-matrix_start.tv_sec)*BILLION+(matrix_stop.tv_nsec - matrix_start.tv_nsec))/BILLION;

		N++;
	}

		create_csv(filename, tim, 10,3);

		gsl_vector_free(a);
		gsl_vector_free(b);
		gsl_vector_free(c);
		gsl_matrix_free(m);

	}
}






