#include <math.h>
#include "segmentation.h"
#include "levenshtein.h"
#include<stdlib.h>
#include <stdio.h>

double sigmaCost(segmentation_t x, segmentation_t y,int i, int j){
	double aux =(fabs(x->sizes[i] - y->sizes[j]))/((double) x->sizes[i] + y->sizes[j]);
	return aux;
}


double epsilonCost(segmentation_t x, segmentation_t y,int i, int j){
	double aux = 1/(1+pow(M_E,-4.0*fabs(x->entropy[i] - y->entropy[j]) + 6.5)) - 0.001501;
	return aux;
}

double cost(segmentation_t x, segmentation_t y,int i, int j){
	double aux = SIGMAC*sigmaCost(x,y,i,j) + EPSILONC*epsilonCost(x,y,i,j);
	return aux;
}

double deltaXY(segmentation_t x, segmentation_t y,int i, int j){
	double aux = cost(x,y,i-1,j-1)*(log((x->sizes[i-1]+y->sizes[j-1])/2)/log(10));
	return aux;
}

double deltaXYMax(segmentation_t x, segmentation_t y,int i, int j){
	double aux = 2*TAU*(log(x->sizes[i-1])/log(10)+log(y->sizes[j-1])/log(10));
	return aux;
}

double deltaY(segmentation_t y,int j){
	double aux = TAU*(log(y->sizes[j-1])/log(10));
	return aux;
}

double deltaX(segmentation_t x,int i){
	double aux = TAU*(log(x->sizes[i-1])/log(10));
	return aux;
}

double differentXY(distmatrix_t distMatrix, segmentation_t x, segmentation_t y,int i, int j){
	double posibilities[3];
	double min;
	posibilities[0] = distMatrix->elements[distMatrix->columns*i+j-1] + deltaY(y,j);
	posibilities[1] = distMatrix->elements[distMatrix->columns*(i-1)+j] + deltaX(x,i);
	posibilities[2] = distMatrix->elements[distMatrix->columns*(i-1)+j-1] + deltaXY(x,y,i,j);
	min = posibilities[0];
	for(i=1;i<3;i++){
		if(min > posibilities[i])
			min = posibilities[i];
	}
	return min;	
}

double levenshteinDist(segmentation_t x, segmentation_t y, double *cost_max){
	int i,j;
	double sol;
	distmatrix_t distMatrix = newDistMatrix();
	iniDistMatrix(x->numsegments+1,y->numsegments+1,distMatrix);
	
	for(i=0;i < x->numsegments+1;i++){
		for(j=0;j < y->numsegments+1; j++){
			if((i==j)&&(i==0))
				distMatrix->elements[distMatrix->columns*(i) + j] = 0;
			else if(i==0)
				distMatrix->elements[distMatrix->columns*(i) + j] = distMatrix->elements[distMatrix->columns*(i) + j-1] + deltaY(y,j);
			else if(j==0)
				distMatrix->elements[distMatrix->columns*(i) + j] = distMatrix->elements[distMatrix->columns*(i-1) + j] + deltaX(x,i);
			else{
				if((x->sizes[i-1]==y->sizes[j-1]) &&(x->entropy[i-1]==y->entropy[j-1]))
					distMatrix->elements[distMatrix->columns*(i) + j] = distMatrix->elements[distMatrix->columns*(i-1) + j-1];
				else
					distMatrix->elements[distMatrix->columns*(i) + j] = differentXY(distMatrix,x,y,i,j);
			}
		}
	}
	
	*cost_max = distMatrix->elements[y->numsegments] + distMatrix->elements[distMatrix->columns*(x->numsegments)];
	sol = distMatrix->elements[distMatrix->columns*distMatrix->rows - 1];
	cleanDistMatrix(distMatrix);
	return sol;
}

distmatrix_t newDistMatrix(){
	return (distmatrix_t) malloc(sizeof(struct __DISTMATRIX));

}

void iniDistMatrix(int rows, int columns, distmatrix_t matrix){
	matrix->rows = rows;
	matrix->columns = columns;
	matrix->elements = malloc(sizeof(double)*rows*columns);
}

void cleanDistMatrix(distmatrix_t matrix){
	free(matrix->elements);
	free(matrix);
}
double similarity(double lDistance, double cost_max){
	return 1- lDistance/cost_max;

}

void printMatrix(distmatrix_t distMatrix){
	fprintMatrix(stdout,distMatrix);
}

	
void fprintMatrix(FILE * file, distmatrix_t distMatrix){
	int i,j;
	for(i=0;i < distMatrix->rows;i++){
		for(j=0;j < distMatrix->columns; j++){
				fprintf(file,"%.4f",distMatrix->elements[distMatrix->columns*(i) + j]);
				if(j!=distMatrix->columns - 1)
					fprintf(file,";");
		}
		printf("\n");
	}
}