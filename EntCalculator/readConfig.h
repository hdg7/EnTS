#ifndef _READCONFIG
#define _READCONFIG

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "minIni.h"

#define CONFIG_ERROR -1
#define CONFIG_OK 0

int readConfigurationS(char* field,char *instance,char *buffer,int buffersize);
int readConfigurationI(char *field,char *instance,int *buffer);
int readConfigurationF(char *field,char *instance,float *buffer);

void setinifile(char * nombre);
#endif /*_READCONFIG*/
