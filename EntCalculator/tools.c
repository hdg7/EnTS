#include"tools.h"

int sequenceLenght(double * sequence){
	int i=0;
	
	while(sequence[i]!=-1)
		i++;
	return i;

}