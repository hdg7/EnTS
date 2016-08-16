#ifndef _WAVELET_H
#define _WAVELET_H

#include "sequence.h"

sequence_t entropy2Wavelet(sequence_t entropy, int scale);

sequence_t wavelet2Entropy(sequence_t wavelet, int scale);

sequence_t subsample4Wavelet(sequence_t entropy, int subsampleSize);

sequence_t adapSubsample4Wavelet(sequence_t entropy, int *subsampleSize);

void waveletThreshold(sequence_t wavelet,double threshold);

void waveletCleanUntilScale(sequence_t wavelet,int scale);

void waveletShowScale(sequence_t wavelet,int scale);

#endif