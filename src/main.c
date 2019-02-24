/*
 *	/systat/src/main.c
 *
 *	Copyrigth (c) Thomas Young 2019
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int cpusage(void){
	
	//Declares arrays for the 8 task counts to be read
	int procA[8], procB[8];
	int allTime, usedTime;

	//Opens the file and finds the initial tasks performed since boot 
	FILE *fpA = fopen("/proc/stat", "r");
	fscanf(fpA, "cpu %d %d %d %d %d %d %d %d", &procA[0], &procA[1], &procA[2], &procA[3], &procA[4], &procA[5], &procA[6], &procA[7]);
	fclose(fpA);
	sleep(1);

	//Opens the file a second time to find the new tasks performed sice boot
	FILE *fpB = fopen("/proc/stat", "r");
	fscanf(fpB, "cpu %d %d %d %d %d %d %d %d", &procB[0], &procB[1], &procB[2], &procB[3], &procB[4], &procB[5], &procB[6], &procB[7]);
	fclose(fpB);
	
	//Calculates the number of tasks over 1 second
	allTime  = (procB[0] + procB[1] + procB[2] + procB[3] + procB[4] + procB[5] + procB[6] + procB[7])
			- (procA[0] + procA[1] + procA[2] + procA[3] + procA[4] + procA[5] + procA[6] + procA[7]);

	//Calculates the number of taks actually performed in the 1 second
	usedTime = (procB[0] + procB[1] + procB[2]) - (procA[0] + procA[1] + procA[2]);
	
	//Puts the percetage used (change / all * 100) into the stdio stream 
	return (usedTime * 100) / allTime;
}

int memusage(void){
	int memAvailable, memTotal;
	char buffer[255];
	
	FILE *fp = fopen("/proc/meminfo", "r");

	fscanf(fp, "MemTotal: %d kB\n", &memTotal);
	fgets(buffer, sizeof(buffer), fp);
	fscanf(fp, "MemAvailable: %d kB\n", &memAvailable);

	return ((memTotal - memAvailable) * 100) / memAvailable;
}

void showcpusage(void){
	printf("%d%%\n", cpusage());
	return;
}

void loopcpusage(void){
	while(1){
		showcpusage();
	}
	return;
}



void systat(void){
	printf("CPU: %d%% \tRAM: %d%%\n", cpusage(), memusage());
	return;
}

void loopsystat(void){
	while(1){
		systat();
	}
	return;
}

int main(int argc, char *argv[]){
	if(argc == 1){
		systat();
	}

	for(int i = 0; argc > i; i++){
		if(strcmp(argv[i], "loop") == 0){
			loopsystat();
		}
	}

	memusage();
	return 0;
}
