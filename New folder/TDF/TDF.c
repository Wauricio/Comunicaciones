#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wavfile.c"
#define Ntap 10
#define PI 3.1415
/*FINAL*/



typedef struct ExpNum {
	double real;
	double ima;
}Exp;

Exp * NewExp(){
	 Exp *num;
	 num=(struct ExpNum*)malloc(sizeof(struct ExpNum));
	 return num;
}


short * getData(int size, FILE * fileWave ,int typeSize){
		 short* arr= (short*)malloc(sizeof(short)*size);
		fread(arr, size,sizeof(short), fileWave);
		for(int i=0;i<size;i++){
                //arr[i]=((arr[i]>>8)|(arr[i]>>8));
           // printf(" %d  %d ",arr[i],i);
		}
		return arr;
}

void CreateFileWAV(char *name  ,short * data , int sizeData, int samples_per_second,int bits_per_sample,int channels , int blockAlign ){

    FILE * f = wavfile_open(name,samples_per_second,bits_per_sample,channels);
    wavfile_write(f,data,sizeData,blockAlign);
    wavfile_close(f);

}

short * toShortArray(Exp *result,int samples , double *RMax , double *IMax){
    short *temp = (short*)malloc(sizeof(short)*samples*2);
    for(int i=0;i<samples;i++){
        temp[i*2]=(int)((result[i].real)*32767.0/ *RMax);
        temp[i*2+1]=(int)((result[i].ima)*32767.0/ *RMax);
        //temp[i*2+1]=0;
        //printf(" \n  R : %d  ,   I : %d",temp[i*2],temp[i*2+1]);
        //temp[i*2]= ((temp[i*2]<<8)|(temp[i*2]>>8));
        //temp[i*2+1]= ((temp[i*2+1]<<8)|(temp[i*2+1]>>8));
               // printf(" \n R : %x(%d)  , I : %x(%d)  ",temp[i*2],temp[i*2],temp[i*2+1],temp[i*2+1]);
        //free(result[i]);
    }
    /*printf("\n\n");
    for(int j=0;j<samples*2;j++){
    	printf(" \n  R : %x  ",temp[j]);

	}*/



    return temp;
}


Exp *TDF(short * arr , int sizeSamples ,double *MaxR , double *MaxI ){
	Exp *temp = (Exp*)malloc(sizeof(Exp)*sizeSamples);
	float x =2*PI/sizeSamples;
	int kn=0;

	for(int k=0;k<sizeSamples;k++){
		Exp num;
		num.ima=0;
		num.real=0;
		for(int n=0;n<sizeSamples;n++){
			kn=k*n;
			num.real += ((arr[n]/32767.0) * cos(kn*x));
			num.ima +=  ((arr[n]/32767.0)*-1*sin(kn*x));
		}
		if(*MaxR<num.real)
			*MaxR=num.real;
		if(*MaxI<num.ima)
			*MaxI=num.ima;
		temp[k]=num;
		printf("\n R : %f , I: %f ",temp[k].real ,temp[k].ima);

	}
    printf("\n  add \n");
    /*
	for(int k=0;k<sizeSamples;k++){
			temp[k]->real = temp[k]->real/ *MaxR;
			temp[k]->ima =temp[k]->ima / *MaxI	;
			//printf(" \nR: %f  =====  , I: %f ====  \n",temp[k]->real, temp[k]->ima);


	}*/
	return temp;
}


Exp *TDFI(short * arr , int sizeSamples ,double *MaxR , double *MaxI ){
	Exp *temp = (Exp*)malloc(sizeof(Exp)*sizeSamples);
	float x =2*PI/sizeSamples;
	int kn=0;

	for(int k=0;k<sizeSamples;k++){
		Exp num;
		num.ima=0;
		num.real=0;
		for(int n=0;n<sizeSamples;n++){
			kn=k*n;
			num.real += ((int)arr[n]) * cos(kn*x)/sizeSamples;
			num.ima +=  ((int)arr[n])*sin(kn*x)/sizeSamples;
		}
		if(*MaxR<num.real)
			*MaxR=num.real;
		if(*MaxI<num.ima)
			*MaxI=num.ima;
		temp[k]=num;
		printf("\n R : %f , I: %f ",temp[k].real,temp[k].ima);

	}
    printf("\n  add \n");
    /*
	for(int k=0;k<sizeSamples;k++){
			temp[k]->real = temp[k]->real/ *MaxR;
			temp[k]->ima =temp[k]->ima / *MaxI	;
			//printf(" \nR: %f  =====  , I: %f ====  \n",temp[k]->real, temp[k]->ima);


	}*/
	return temp;
}




int main(int arcgs , char *args[])
{

        /******************Reading Header********************/

		FILE *fileptr;
		FILE *fileout;
		char  *buffer;
		char *endFile;
		char nwname[]="Nuevo-";
		long filelen;
	    int i=44;
	    if(arcgs<2)
		{
	    	printf("Ingresa el Nombre del Archivo");
	    	exit(0);
		}
		fileptr = fopen(args[1], "rb");
		//fileout = fopen(strcat(nwname,args[1]),"wb");
		if(fileptr==NULL)
		{
			printf("Ocurrio un Problema para Abrir Archivo");
			exit(0);
		}
		fseek(fileptr, 0, SEEK_END);
		filelen = ftell(fileptr);
		rewind(fileptr);
		buffer = (char*)malloc((filelen+1)*sizeof(char));
		fread(buffer, 44,sizeof(char), fileptr);
		endFile=(char*)malloc(sizeof(char)*72);

		/*********************************************************************************************/


		int blockAlign=*(buffer+32),numChannels=*(buffer+22),btpSm=*(buffer + 34) ;
		char unsigned a[]={*(buffer + 40),*(buffer + 41),*(buffer + 42),*(buffer + 43)};
		char unsigned b[]={*(buffer + 24),*(buffer + 25),*(buffer + 26),*(buffer + 27)};
		int siz = (a[3]<<24) | (a[2]<<16) | (a[1]<<8) | (a[0]);
		int sampleRate= (b[3]<<24) | (b[2]<<16) | (b[1]<<8) | (b[0]);
		//typedef short Sample;
		printf("block : %d  numChan: %d , bitsperSample:%d ,size : %d   , %d \n , %x \n",blockAlign,numChannels ,btpSm ,siz,sizeof(short),-50);

		printf("block : %d  numChan: %d , bitsperSample:%d ,size : %d   , %d   sample Rate %d \n",blockAlign,numChannels ,btpSm ,siz,sizeof(float),sampleRate);
		/*

		void  *arr;
		arr=getData(siz,fileptr,blockAlign/2);
		arr=Conv(arr,siz,blockAlign);
		fwrite(buffer,44,1,fileout);
		fwrite(arr,siz,1,fileout);

		fread(endFile, 72,sizeof(char), fileptr);
		fwrite(endFile,72,sizeof(char),fileout);

*/


		printf("\n");
		short *samples=getData(siz/blockAlign,fileptr,blockAlign/2);
		double MaxR=0,MaxI=0;
		Exp *arr = TDF(samples,siz/2,&MaxR,&MaxI);
		short * temp = toShortArray(arr,siz/2,&MaxR,&MaxI);
		printf("\n% f , %f \n",MaxR,MaxI);
		/*for(int i=0;i<50;i++){

			printf("Real %d     ima : %d \n",temp[i*2],temp[i*2 +1 ]);
		}
		int x=10;
		printf("%x , %x \n",x,((x<<8)|(x>>8)));*/


		CreateFileWAV("nw.wav",temp,siz,sampleRate,btpSm,2,sizeof(short));



        	fclose(fileptr);
			//fclose(fileout);
			free(buffer);
			//free(arr);
	return 0;

}
