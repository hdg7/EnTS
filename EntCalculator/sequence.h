#ifndef _SEQUENCE_H
#define _SEQUENCE_H

#include<stdio.h>

struct __SEQ {
	double * seq;
	int size;
};
typedef struct __SEQ* sequence_t;

sequence_t newSequence();
void iniSequence(int size, sequence_t seq);
void cleanSequence(sequence_t seq);
void fprintSequence(FILE* output,sequence_t seq);
void resizeSequence(int newSize, sequence_t seq);

#endif