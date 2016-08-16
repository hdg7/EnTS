#include "segmentation.h"
#include "sequence.h" 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
sequence_t defineSegments(sequence_t reconstruction){
	int i;
	int numberSegments=0;
	double previous=0.0;
	sequence_t segments = newSequence();
	iniSequence(reconstruction->size,segments);
	
	for(i=0;i < reconstruction->size;i++){
		if(previous==reconstruction->seq[i])
			continue;
		if((previous*reconstruction->seq[i]!=0.0)&&(fabs(previous-reconstruction->seq[i])>0.5)){
			if(numberSegments>0){
				if(segments->seq[numberSegments-1] != i){
					segments->seq[numberSegments]=i+1;
					numberSegments++;
				}
			}
			else {
				segments->seq[numberSegments]=i;
				numberSegments++;
			}
		}
		previous = reconstruction->seq[i];
	}
	//if(numberSegments==0)
	//{
	if(numberSegments>0)
	{
		if(segments->seq[numberSegments-1]!=i){
			segments->seq[numberSegments]=i;
			numberSegments++;
		}
	}
	else{
		segments->seq[numberSegments]=i;
		numberSegments++;
	}
	resizeSequence(numberSegments, segments);
	return segments;

}

segmentation_t defineSegmentation(sequence_t subentropy,sequence_t segments){
	int i;
	int currentSegment=0;
	double total = 0.0;
	segmentation_t segmentation = newSegmentation();
	iniSegmentation(segments->size,segmentation);
	
	for(i=0;i < subentropy->size;i++){
		if(!(currentSegment >= segments->size)){					
			if(i==segments->seq[currentSegment]){
				if(currentSegment == 0)
					segmentation->sizes[currentSegment] = segments->seq[currentSegment];
				else
					segmentation->sizes[currentSegment] = segments->seq[currentSegment] - segments->seq[currentSegment-1];
				segmentation->entropy[currentSegment] = total/(double)segmentation->sizes[currentSegment]; /*Average entropy*/
				total = 0.0;
				currentSegment++;			
			}			
		}
		total += subentropy->seq[i];	
	}
	if(currentSegment == 0){
		segmentation->sizes[currentSegment] = subentropy->size;
		segmentation->entropy[currentSegment] = total/(double)segmentation->sizes[currentSegment];
	}
	else if(subentropy->size - segments->seq[currentSegment-1] > 0)
	{
		segmentation->sizes[currentSegment] = subentropy->size - segments->seq[currentSegment-1];
		segmentation->entropy[currentSegment] = total/(double)segmentation->sizes[currentSegment];
	}
	return segmentation;

}


segmentation_t newSegmentation(){
	return (segmentation_t) malloc(sizeof(struct __SEGMENT));

}

void iniSegmentation(int numSegments, segmentation_t seg){
	seg->numsegments = numSegments;
	seg->sizes = malloc(sizeof(int)*numSegments);
	seg->entropy = malloc(sizeof(double)*numSegments);
}

void printSegmentation(segmentation_t seg){
	fprintSegmentation(stdout,seg);

}

void fprintSegmentation(FILE* output,segmentation_t seg){
	int i;
	//printf("\n");
	for(i =0 ; i < seg->numsegments ; i++){
		fprintf(output,"%d;%lf",seg->sizes[i],seg->entropy[i]);
		if(i<seg->numsegments-1)
			fprintf(output,"\n");
	}

}

segmentation_t readSegmentation(char * fileName){
	int i;
	//printf("\n");
	
	
	FILE* file = NULL;
	int size=1;
	int segmentSize;
	double segmentEntropy;
	char ch;
	segmentation_t segmentation = newSegmentation();
	
	file=fopen(fileName,"r");
	if(file == NULL)
		perror("File does not exists.\n");
	//if(fseek(file, 0L, SEEK_END))
		//perror("Could not reach the end of the file");
	while(!feof(file))
	{
	  ch = fgetc(file);
	  if(ch == '\n')
	  {
		size++;
	  }
	}
	if(fseek(file, 0L, SEEK_SET))
		perror("Could not reach the beginning of the file");
	iniSegmentation(size,segmentation);
	for(i=0;i<size;i++)
	{
		//fgets(line,MAX_LINE,file);
		fscanf( file, "%d;%lf\n", &segmentSize, &segmentEntropy);
		segmentation->sizes[i] = segmentSize;
		segmentation->entropy[i] = segmentEntropy;
	}
	fclose(file);
	//entropySequence->seq[i]=-1.0;
	return(segmentation);
	
}

void cleanSegmentation(segmentation_t seg){
	free(seg->entropy);
	free(seg->sizes);
	free(seg);
}