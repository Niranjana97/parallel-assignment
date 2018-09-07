#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
float calculateAvg(int *p,int s){
	float sum=0;
	for(int i=0;i<s;i++){
		sum+=*(p+i);
	}
	return (float)(sum/s);
}
float compute(int elem,float avg){
	float ans=abs(elem - avg);
	return ans*ans;
}
void main(int argc,char** argv){
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	float avg=0.0;
	int sd;
	int arr[size];
	if(rank==0){
		for(int i=0;i<size;i++){
			arr[i]=rand()%100;
			printf("%d ",arr[i]);
		}
		avg=calculateAvg(arr,size);
		printf("\nAverage is %f\n",avg);
	}
	MPI_Bcast(&avg,1,MPI_FLOAT,0,MPI_COMM_WORLD);
	float imm=compute(arr[rank],avg);
	float *sub_avg =NULL;
	float ans=0.0;
	if(rank==0)
		sub_avg = malloc(sizeof(float)*size);
	MPI_Gather(&imm,1,MPI_FLOAT,sub_avg,1,MPI_FLOAT,0,MPI_COMM_WORLD);
	if(rank==0){
		for(int i=0;i<size;i++) {
			printf("%f ",*(sub_avg+i));
			ans+=*(sub_avg+i);
		}
		sd=(int)ans/size;
		sd=sqrt(sd);
		printf("\n SD = %d",sd);
	}
	MPI_Finalize();

	
}
