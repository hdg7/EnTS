#ifndef _LEVENSHTEIN_H
#define _LEVENSHTEIN_H

#define SIGMAC 1.6
#define EPSILONC 0.4
#define TAU 0.3

struct __DISTMATRIX{
	double * elements;
	int columns;
	int rows;
};

typedef struct __DISTMATRIX* distmatrix_t;

distmatrix_t newDistMatrix();
void iniDistMatrix(int rows, int columns, distmatrix_t matrix);
double levenshteinDist(segmentation_t x, segmentation_t y, double *cost_max);
double similarity(double lDistance, double cost_max);
void printMatrix(distmatrix_t distMatrix);
void fprintMatrix(FILE * file, distmatrix_t distMatrix);
void cleanDistMatrix(distmatrix_t matrix);

#endif