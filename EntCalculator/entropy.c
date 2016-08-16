#include"entropy.h"
#include "sequence.h"
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>


/**
 * @brief This function calculates the local entropy of a specific byte
 * @param byte the byte 
 * @param windows the rest of the block
 * @param size the block size
 * @param ignorePositions this array is used to check how many times a specific byte is repeated in order to avoid repetitions
 * @return The local entropy of a specific byte
 */
double byteEntropy(unsigned char byte,unsigned char * windows, int size, int * ignorePositions){
	int i;
	int counter=0;
	double pbyte;
	for(i=0;i<size;i++){
		if(byte == windows[i]){
			counter++;
			ignorePositions[i] = 1;
		}
	}		
	pbyte = ((double) counter) / ((double) size);
	if(pbyte * log(pbyte)/log(2) > 8)
		printf("Ein!\n");
	return pbyte * log(pbyte)/log(2);
}

/**
 * @brief This function is used to calculate the entropy of a specific block
 * @param windows the block
 * @param size the number of bytes within the block
 * @return the entropy value
 */
double blockEntropy(unsigned char * windows, int size){
	int i;
	double totalEntropy = 0.0;
	int * ignorePositions = calloc(size,sizeof(int));

	for(i = 0; i < size; i++){
		if(ignorePositions[i]!=0)
			continue;
		totalEntropy += byteEntropy(windows[i],windows,size,ignorePositions);	
	}
	free(ignorePositions);
	return -totalEntropy;

}

/**
 * @brief This function calculates the entropy sequence for a specific file
 * @param fileName the file name
 * @param blockSize the blcokSize for the sliding windows.
 * @return A list of doubles with the values of the entropy sequence.
 */
sequence_t fileEntropy(char *fileName, int blockSize){
	
	FILE* file = NULL;
	int size;
	unsigned char * windows = malloc(sizeof(char)*(blockSize));
	sequence_t entropySequence = newSequence();
	int i;
	
	file=fopen(fileName,"r");
	if(file == NULL)
		perror("File does not exists.\n");
	if(fseek(file, 0L, SEEK_END))
		perror("Could not reach the end of the file");
	
	size = ftell(file)/blockSize;
	if(fseek(file, 0L, SEEK_SET))
		perror("Could not reach the beginning of the file");
	//entropySequence->seq = malloc(sizeof(double)*(size+1));
	iniSequence(size,entropySequence);
	for(i=0;i<size;i++)
	{
		fread(windows,sizeof(char),blockSize,file);
		entropySequence->seq[i]=blockEntropy(windows,blockSize);
	}
	//entropySequence->seq[i]=-1.0;
	free(windows);
	fclose(file);
	return entropySequence;
}