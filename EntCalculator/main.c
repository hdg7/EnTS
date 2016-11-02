#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "entropy.h"
#include "tools.h"
#include "wavelet.h"
#include "sequence.h"
#include "readConfig.h"
#include "segmentation.h"
#include "levenshtein.h"
#include<math.h>

#define LINE_L 256

/*Tow options:
 * - -f Create the segmentation from a specific config file.
 * - -d Calculates the similarity for all file *.out in a folder. These files are segmentations.
 *  - Nothing uses a config.ini file in the current folder.
 */
 
 int commandFile(char * filein){
	char line[LINE_L];
	int scaleOr=4;
	int scaleMod=1;
	float threshold = 0.5;
	int subsampleSize=256;
	int windowsSize = 256;
	
	int flag_scaleShow=0;
	int flag_scaleUntil=0;
	int flag_threshold=0;
	
	sequence_t list = NULL;
	sequence_t sublist = NULL;
	//double sublist[8] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0};
	sequence_t wavelet = NULL;
	sequence_t reconstruction = NULL;
	sequence_t segments = NULL;
	
	segmentation_t segmentation = NULL;

	setinifile(filein);
	 
	readConfigurationS("Files","original",line,LINE_L);
	FILE* originalSegment = fopen(line,"w");
	readConfigurationS("Files","subseq",line,LINE_L);
	FILE* subsegment = fopen(line,"w");
	readConfigurationS("Files","wavelet",line,LINE_L);
	FILE* waveletFile = fopen(line,"w");
	readConfigurationS("Files","reconstruction",line,LINE_L);
	FILE* reconFile = fopen(line,"w");
	readConfigurationS("Files","segments",line,LINE_L);
	FILE* segmentFile = fopen(line,"w");
	readConfigurationS("Files","segmentation",line,LINE_L);
	FILE* segmentationFile = fopen(line,"w");
	
	/*Reading Values*/
	readConfigurationI("Values","scaleOr",&scaleOr);
	readConfigurationI("Values","scaleMod",&scaleMod);
	readConfigurationI("Values","subsampleSize",&subsampleSize);
	readConfigurationI("Values","windowsSize",&windowsSize);
	readConfigurationF("Values","threshold",&threshold);

	readConfigurationI("Operations","threshold",&flag_threshold);
	readConfigurationI("Operations","scaleUntil",&flag_scaleUntil);
	readConfigurationI("Operations","scaleShow",&flag_scaleShow);
	
	readConfigurationS("Files","input",line,LINE_L);
	list = fileEntropy(line, windowsSize);
	sublist = subsample4Wavelet(list,subsampleSize);
	
	//sublist = adapSubsample4Wavelet(list,&subsampleSize,sequenceLenght(list));
	//printf("Scale = %d",((int) (log(subsampleSize)/log(2))));
	
	if(scaleOr == -1)
		scaleOr = (int) floor(log(subsampleSize)/log(2));
	wavelet = entropy2Wavelet(sublist,scaleOr);
	
	
	/*Operations*/
	if(flag_threshold)
		waveletThreshold(wavelet,threshold);	
	if(flag_scaleUntil)
		waveletCleanUntilScale(wavelet,scaleMod);
	if(flag_scaleShow)
		waveletShowScale(wavelet,scaleMod);
	
	/*Reconstruction*/
	reconstruction = wavelet2Entropy(wavelet,scaleOr);

	/*Segmentation*/
	segments = defineSegments(reconstruction);
	segmentation = defineSegmentation(sublist,segments);
	
	/*Printing*/
	fprintSequence(originalSegment,list);	
	fprintSequence(subsegment,sublist);
	fprintSequence(reconFile,reconstruction);
	fprintSequence(waveletFile,wavelet);
	fprintSequence(segmentFile,segments);
	fprintSegmentation(segmentationFile,segmentation);
	
	/*Cleaning*/
	cleanSequence(list);
	cleanSequence(sublist);
	cleanSequence(wavelet);
	cleanSequence(reconstruction);
	cleanSequence(segments);
	cleanSegmentation(segmentation);
	
	
	fclose(originalSegment);
	fclose(subsegment);
	fclose(waveletFile);
	fclose(reconFile);
	fclose(segmentFile);
	return 0;
	 
 }
 
 int commandInput(char * filein){
	char line[LINE_L];
	int scaleOr=4;
	int scaleMod=1;
	float threshold = 0.5;
	int subsampleSize=256;
	int windowsSize = 256;
	
	int flag_scaleShow=0;
	int flag_scaleUntil=0;
	int flag_threshold=0;
	
	sequence_t list = NULL;
	sequence_t sublist = NULL;
	//double sublist[8] = {0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0};
	sequence_t wavelet = NULL;
	sequence_t reconstruction = NULL;
	sequence_t segments = NULL;
	
	segmentation_t segmentation = NULL;

	setinifile("config.ini");
	 
	if (readConfigurationS("Files","original",line,LINE_L)!=CONFIG_OK)
	{
                perror("Configuration File Error");
		return -1;
	}
	FILE* originalSegment = fopen(strcat(line,filein),"w");
	if(originalSegment == NULL)
	{
		perror("Can't read original segment");
		return -1;
	}
	readConfigurationS("Files","subseq",line,LINE_L);
	FILE* subsegment = fopen(strcat(line,filein),"w");
	if(subsegment == NULL)
		perror("Can't read original segment");
	readConfigurationS("Files","wavelet",line,LINE_L);
	FILE* waveletFile = fopen(strcat(line,filein),"w");
	if(waveletFile == NULL)
		perror("Can't read original segment");
	readConfigurationS("Files","reconstruction",line,LINE_L);
	FILE* reconFile = fopen(strcat(line,filein),"w");
	if(reconFile == NULL)
		perror("Can't read original segment");
	readConfigurationS("Files","segments",line,LINE_L);
	FILE* segmentFile = fopen(strcat(line,filein),"w");
	if(segmentFile == NULL)
		perror("Can't read original segment");
	readConfigurationS("Files","segmentation",line,LINE_L);
	FILE* segmentationFile = fopen(strcat(line,filein),"w");
	if(segmentationFile == NULL)
		perror("Can't read original segment");
	
	/*Reading Values*/
	readConfigurationI("Values","scaleOr",&scaleOr);
	readConfigurationI("Values","scaleMod",&scaleMod);
	readConfigurationI("Values","subsampleSize",&subsampleSize);
	readConfigurationI("Values","windowsSize",&windowsSize);
	readConfigurationF("Values","threshold",&threshold);

	readConfigurationI("Operations","threshold",&flag_threshold);
	readConfigurationI("Operations","scaleUntil",&flag_scaleUntil);
	readConfigurationI("Operations","scaleShow",&flag_scaleShow);
	
	//readConfigurationS("Files","input",line,LINE_L);
	list = fileEntropy(filein, windowsSize);
	sublist = subsample4Wavelet(list,subsampleSize);
	
	//sublist = adapSubsample4Wavelet(list,&subsampleSize,sequenceLenght(list));
	//printf("Scale = %d",((int) (log(subsampleSize)/log(2))));
	
	if(scaleOr == -1)
		scaleOr = (int) floor(log(subsampleSize)/log(2));
	wavelet = entropy2Wavelet(sublist,scaleOr);
	
	
	/*Operations*/
	if(flag_threshold)
		waveletThreshold(wavelet,threshold);	
	if(flag_scaleUntil)
		waveletCleanUntilScale(wavelet,scaleMod);
	if(flag_scaleShow)
		waveletShowScale(wavelet,scaleMod);
	
	/*Reconstruction*/
	reconstruction = wavelet2Entropy(wavelet,scaleOr);

	/*Segmentation*/
	segments = defineSegments(reconstruction);
	segmentation = defineSegmentation(sublist,segments);
	
	/*Printing*/
	fprintSequence(originalSegment,list);	
	fprintSequence(subsegment,sublist);
	fprintSequence(reconFile,reconstruction);
	fprintSequence(waveletFile,wavelet);
	fprintSequence(segmentFile,segments);
	fprintSegmentation(segmentationFile,segmentation);
	
	/*Cleaning*/
	cleanSequence(list);
	cleanSequence(sublist);
	cleanSequence(wavelet);
	cleanSequence(reconstruction);
	cleanSequence(segments);
	cleanSegmentation(segmentation);
	
	
	fclose(originalSegment);
	fclose(subsegment);
	fclose(waveletFile);
	fclose(reconFile);
	fclose(segmentFile);
	fclose(segmentationFile);
	
	return 0;
	 
 }
 
 
 
int commandDirectory(char * directory){
	char lsCommand[LINE_L];
	char line[LINE_L];
	FILE * listFiles;
	char ch;
	char pid[LINE_L];
	char filename[LINE_L];
	int sizeFile=0;
	segmentation_t* segmentationList;
//	distmatrix_t distMatrix;
	double lDistance;
	double costMax;
	int i,j;
	strcpy(lsCommand,"ls ");
	strcat(lsCommand,directory);
	strcat(lsCommand,"segmentation*.bin > file.");
	sprintf(pid,"%d",getpid());
	strcat(lsCommand,pid);
	strcat(lsCommand,".ini");
	system(lsCommand);
	

	strcpy(filename,"file.");
	strcat(filename,pid);
	strcat(filename,".ini");
	listFiles = fopen(filename,"r");
	
	while(!feof(listFiles))
	{
	  ch = fgetc(listFiles);
	  if(ch == '\n')
	  {
		sizeFile++;
	  }
	}
	if(fseek(listFiles, 0L, SEEK_SET))
		perror("Could not reach the beginning of the file");
	segmentationList = malloc(sizeof(segmentation_t)*sizeFile);
	for(i=0;i<sizeFile;i++){
		fgets(line,LINE_L,listFiles);
		line[strlen(line)-1]='\0';
		segmentationList[i] = readSegmentation(line);
		//printSegmentation(segmentation);
	}
	
	//distMatrix = newDistMatrix();
	//iniDistMatrix(sizeFile,sizeFile,distMatrix);
	
	for(i=0;i<sizeFile;i++){
		for(j=0;j < sizeFile; j++){
			if(i == j){
				fprintf(stdout,"1.000000");
			}
			else {
				lDistance = levenshteinDist(segmentationList[i],segmentationList[j],&costMax);
				fprintf(stdout,"%lf",similarity(lDistance,costMax));
			}
			if(j < sizeFile - 1){
				fprintf(stdout,";");
			}
		}
		fprintf(stdout,"\n");
	}
	//printMatrix(distMatrix);
	return 0;
}
int main(int argc, char **argv)
{
	
//	segmentation_t segmentation = NULL;
//	segmentation_t segmentationAux = NULL;

	if(argc < 2)
	{
		setinifile("config.ini");
	}
	else if(!strncmp(argv[1],"-f",2)){
		commandFile(argv[2]);
	}
	else if(!strncmp(argv[1],"-i",2)){
		commandInput(argv[2]);
	}
	else if(!strncmp(argv[1],"-d",2)){
		commandDirectory(argv[2]);
	}
//	segmentation = defineSegmentation(sublist,segments);
	
	
	
	//segmentationAux = defineSegmentation(sublist,segments);
	//segmentationAux->sizes[1]=120;
	//segmentationAux->entropy[1]=3.0;
	//lDistance = levenshteinDist(segmentation,segmentationAux,&costMax);
//	printf("Distancia Levenshtein: %lf\n",lDistance);
	//printf("%lf\n",similarity(lDistance,costMax));
	return 0;
}
