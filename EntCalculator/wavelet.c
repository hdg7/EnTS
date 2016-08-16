#include "wavelet.h"
#include "math.h"
#include<stdio.h>
#include<stdlib.h>
 
/**
 * @brief This function calculates the Discrete Wavelet Transformation of the entropy following a HAAR transformation 
 * @param entropy the signal we want to transform
 * @param scale the scale paramete usually set as log_2 (subsampleSize)
 * @param subsampleSize size of the entropy sequence
 * @return the wavelet sequence
 */
sequence_t entropy2Wavelet(sequence_t entropy, int scale){
	
	int i =0,j=0,power;
	sequence_t wavelet = newSequence();
	double * aux = malloc(sizeof(double)*entropy->size);
	iniSequence(entropy->size,wavelet);
	for(i=0;i<entropy->size;i++)
		aux[i] = entropy->seq[i];
	//scale=1;
	for(j=0;j <scale; j++){
		power=((int)pow(2,j+1));
		for(i=0;i < entropy->size/(power);i++){
			wavelet->seq[i] =  ((double) aux[2*i]+aux[2*i+1])/sqrt(2.0);
			wavelet->seq[entropy->size/(power) + i] = ((double) aux[2*i]-aux[2*i+1])/sqrt(2.0);
		}
		for(i=0;i<entropy->size;i++)
		aux[i] = wavelet->seq[i];
	
	}
	free(aux);
	return wavelet;
	
}

sequence_t wavelet2Entropy(sequence_t wavelet, int scale){
	
	int i =0,j=0,power;
	sequence_t  entropy = newSequence();
	double * aux = malloc(sizeof(double)*wavelet->size);
	
	iniSequence(wavelet->size,entropy);
	for(i=0;i<wavelet->size;i++)
		aux[i] = wavelet->seq[i];
	//scale=1;
	for(j=scale;j > 0; j--){
		power=wavelet->size/((int)pow(2,j));
		for(i=0;i < power;i++){
			entropy->seq[2*i] =  ((double) aux[i]+aux[power+i])/sqrt(2.0);
			entropy->seq[2*i+1] = ((double) aux[i]-aux[power+i])/sqrt(2.0);
		}
		for(i=0;i<2*power;i++)
			aux[i] = entropy->seq[i];
	
	}
	free(aux);
	return entropy;
	
}

void waveletThreshold(sequence_t wavelet,double threshold){
	int i;
	for(i=0;i < wavelet->size;i++){
		if(abs(wavelet->seq[i])< threshold)
			wavelet->seq[i] =  0.0;
	}
	
}


void waveletShowScale(sequence_t wavelet,int scale){
	int i;
	int power1=wavelet->size/((int)pow(2,scale));
	int power2=wavelet->size/((int)pow(2,scale+1));
	for(i=power1;i < wavelet->size;i++){
		wavelet->seq[i] =  0.0;
	}
	for(i=power2-1;i >= 0;i--){
		wavelet->seq[i] =  0.0;
	}
}

void waveletCleanUntilScale(sequence_t wavelet,int scale){
	int i;
	int power=wavelet->size/((int)pow(2,scale));
	for(i=power;i < wavelet->size;i++){
		wavelet->seq[i] =  0.0;
	}
	
}
/**
 * @brief This function generates a subsample of the original entropy, this is used to have a wavelet which is a power of 2
 * @param entropy the original sequence
 * @param subsampleSize new size for the sequence
 * @param originalSize size of the original sequence
 * @return a reduced version of the original sequence
 */
sequence_t subsample4Wavelet(sequence_t entropy, int subsampleSize){
	double jump;
	int i;
	sequence_t solution = newSequence();
	
	//jump = (double)entropy->size/ (double)subsampleSize;
	jump = (double)entropy->size/ ((double)subsampleSize-1);
	//printf("jump %lf\n",jump);
	iniSequence(subsampleSize,solution);
	
	for(i = 0; i < subsampleSize; i++){
		solution->seq[i] = entropy->seq[(int) (((double) i)*jump) ];
	}
	return solution;	

}

/**
 * @brief This function generates a subsample of the original entropy, this is used to have a wavelet which is a power of 2
 * @param entropy the original sequence
 * @param subsampleSize new size for the sequence
 * @param originalSize size of the original sequence
 * @return a reduced version of the original sequence
 */
sequence_t adapSubsample4Wavelet(sequence_t entropy, int *subsampleSize){
	double jump;
	int i;
	sequence_t solution = newSequence();
	
	*subsampleSize=(int) pow(2,floor(log(entropy->size)/log(2)));
	jump = (double)entropy->size/ (double)*subsampleSize;
	//printf("jump %lf\n",jump);
	iniSequence(*subsampleSize,solution);
	
	for(i = 0; i < *subsampleSize; i++){
		solution->seq[i] = entropy->seq[(int) (((double) i)*jump) ];
	}
	return solution;	

}
