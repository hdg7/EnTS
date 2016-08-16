#include "sequence.h"
#include<stdlib.h>
#include<stdio.h>
sequence_t newSequence(){
	return (sequence_t) malloc(sizeof(struct __SEQ));

}

void iniSequence(int size, sequence_t seq){
	seq->size = size;
	seq->seq = malloc(sizeof(double)*size);
}

void resizeSequence(int newSize, sequence_t seq){
	seq->size = newSize;
	seq->seq = realloc(seq->seq,sizeof(double)*newSize);
}

void cleanSequence(sequence_t seq){
	free(seq->seq);
	free(seq);
}

void printSequence(sequence_t seq){
	fprintSequence(stdout,seq);

}

void fprintSequence(FILE* output,sequence_t seq){
	int i;
	//printf("\n");
	for(i =0 ; i < seq->size ; i++){
		fprintf(output,"%lf",seq->seq[i]);
		if(i<seq->size-1)
			fprintf(output,";");
	}

}