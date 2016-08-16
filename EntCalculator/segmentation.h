#ifndef _WAVELETSEGMENTATION_H
#define _WAVELETSEGMENTATION_H

#include "sequence.h"

struct __SEGMENT {
	int numsegments;
	int * sizes;
	double * entropy;
};

typedef struct __SEGMENT *segmentation_t;

segmentation_t newSegmentation();
void fprintSegmentation(FILE* output,segmentation_t seg);
void printSegmentation(segmentation_t seg);
void iniSegmentation(int numSegments, segmentation_t seg);
void cleanSegmentation(segmentation_t seg);
segmentation_t readSegmentation(char * fileName);



sequence_t defineSegments(sequence_t reconstruction);
segmentation_t defineSegmentation(sequence_t subentropy,sequence_t segments);


#endif