#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wavfile.h"
#include "wavfile.c"


#define MAXVAL 32767.0
//32512
#define OUTFILE "OUT.wav"
#define Ntap 10
#define PI 3.14159265358979323846


typedef struct {
    int size;
    int blockAlign;
    int channels;
    int btps;
    int sampleRate;


}FileInfo;



short * getData(int size, FILE * fileWave ){
		 short* arr= (short*)malloc(sizeof(short)*size);
		fread(arr, size,sizeof(short), fileWave);
		/*for(int i=0;i<size;i++){
                //arr[i]=((arr[i]>>8)|(arr[i]>>8));
            printf(" %x  %d ",arr[i],i);
		}*/
		return arr;
}


void CreateFileWAV(char *name  ,short * data , int sizeData, int samples_per_second,int bits_per_sample,int channels , int blockAlign ){

    FILE * f = wavfile_open(name,samples_per_second,bits_per_sample,channels);
    wavfile_write(f,data,sizeData,blockAlign);
    wavfile_close(f);
    //free(data);

}



FileInfo readHeader(char * buffer ){
        FileInfo info;
        int blockAlign=*(buffer+32),numChannels=*(buffer+22),btpSm=*(buffer + 34) ;
		char unsigned a[]={*(buffer + 40),*(buffer + 41),*(buffer + 42),*(buffer + 43)};
		char unsigned b[]={*(buffer + 24),*(buffer + 25),*(buffer + 26),*(buffer + 27)};
		int siz = (a[3]<<24) | (a[2]<<16) | (a[1]<<8) | (a[0]);
		int sampleRate= (b[3]<<24) | (b[2]<<16) | (b[1]<<8) | (b[0]);
        printf(" block : %d  numChan: %d , bitsperSample:%d ,size : %d  sample Rate %d \n",blockAlign,numChannels ,btpSm ,siz,   sampleRate);
        info.size=siz;
        info.blockAlign=blockAlign;
        info.channels=numChannels;
        info.btps=btpSm;
        info.sampleRate=sampleRate;

    return info;

}






short *Pulse(int size){
    short *temp =(short*)malloc(sizeof(short)*size);
    for(int i=0;i<size;i++)
        temp[i]=0;
    for(int i=0;i<size/8;i++){
        //temp[i*16]=0;
        temp[i*8]=32512;
    }
    return temp;



}


int main(int arcgs , char *args[])
{

 /******************Reading Header********************/

		FILE *fileptr1;
		FILE *fileptr2;
		char  *buffer;
		char *buffer2;
		//char *endFile;
		long filelen1;
		long filelen2;
		short *samples1,*samples2;
	    if(arcgs<3)
		{
	    	printf("Ingresa el Nombre del Archivo");
	    	exit(0);
		}
		fileptr1 = fopen(args[1], "rb");
		fileptr2 = fopen(args[2],"rb");
		if(fileptr1==NULL || fileptr2==NULL )
		{
			printf("Ocurrio un Problema para Abrir Archivos");
			exit(0);
		}

        /***********************Reading Files***********************/

            /*****Reading File No 1********************/
                fseek(fileptr1, 0, SEEK_END);
                filelen1 = ftell(fileptr1);
                rewind(fileptr1);
                buffer = (char*)malloc(44*sizeof(char));
                fread(buffer, 44,sizeof(char), fileptr1);
                //=(char*)malloc(sizeof(char)*72);
                printf(" \n Header Archivo 1 \n");
                FileInfo info1=readHeader(buffer);
                samples1 = getData(info1.size/2,fileptr1);
            /*****Reading File No 2**********************/
                fseek(fileptr2, 0, SEEK_END);
                filelen2 = ftell(fileptr2);
                rewind(fileptr2);
                buffer2 = (char*)malloc(44*sizeof(char));
                fread(buffer2, 44,sizeof(char), fileptr2);
                printf(" \n Header Archivo 1 \n");
                FileInfo info2=readHeader(buffer2);
                samples2=getData(info2.size/2,fileptr2);


		/***************************************Multi******************************************************/

            FileInfo min=(info1.size<info2.size) ?info1:info2;
            //short *smpTemp=(short*)malloc(sizeof(short)*min.size);
            double *smpTemp=(double*)malloc(sizeof(double)*min.size/2);
            double max=0.0;
            for(int i=0;i<min.size/2;i++){
                smpTemp[i]=(samples1[i]*samples2[i])/MAXVAL;
                if(smpTemp[i]>max)
                    max=smpTemp[i];
            }
            for(int i=0;i<min.size/2;i++){
                samples1[i]=smpTemp[i]*MAXVAL/max;
            }


            CreateFileWAV(OUTFILE,samples1,min.size/2,min.sampleRate,min.btps,min.channels,min.blockAlign);
            //CreateFileWAV(OUTFILE,Pulse(705),705,44100,16,1,sizeof(short));




		/**************************************************************************************************/

        	fclose(fileptr1);
        	fclose(fileptr2);
			//fclose(fileout);
			free(samples1);
            free(samples2);
            free(smpTemp);
			free(buffer);
			free(buffer2);
	return 0;




}
