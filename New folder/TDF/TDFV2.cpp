#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "wavfile.c"
#define Ntap 10
#define PI 3.1415
#define MAXVAL 32767.0
#define OUTFILE "OUT.wav"



typedef struct ExpNum {
	int real;
	int ima;
}Exp;

Exp * NewExp(){
	 Exp *num;
	 num=(struct ExpNum*)malloc(sizeof(struct ExpNum));
	 return num;
}


short * getData(int size, FILE * fileWave ){
		 short* arr= (short*)malloc(sizeof(short)*size);
		fread(arr, size,sizeof(short), fileWave);
		return arr;
}


void CreateFileWAV(char *name  ,short * data , int sizeData, int samples_per_second,int bits_per_sample,int channels , int blockAlign ){

    FILE * f = wavfile_open(name,samples_per_second,bits_per_sample,channels);
    wavfile_write(f,data,sizeData,blockAlign);
    wavfile_close(f);

}

short * toShortArray(Exp *result,int samples , int *Max ,int invert){
    short *temp = (short*)malloc(sizeof(short)*samples*2);

    
    
    
    if(invert){
    	for(int i=0;i<samples;i++){
        temp[i*2]=~(short)((result[i].real)*MAXVAL/ *Max);
        temp[i*2+1]=~(short)((result[i].ima)*MAXVAL/ *Max);
	  	}
	}else{
		
		for(int i=0;i<samples;i++){
        temp[i*2]=(short)((result[i].real)*MAXVAL/ *Max);
        temp[i*2+1]=(short)((result[i].ima)*MAXVAL/ *Max);
        //temp[i*2]= ~temp[i*2];
       // temp[i*2+1]= ~temp[i*2+1];
       
    	}
	}
    


    return temp;
}


Exp *TDF(short * arr , int sizeSamples ,int *Max ){
	Exp *temp = (Exp*)malloc(sizeof(Exp)*sizeSamples);
	float x =2*PI/sizeSamples;
	int kn=0;
	//printf(" X: %f \n",x);

	for(int k=0;k<sizeSamples;k++){
		Exp num;
		num.ima=0;
		num.real=0;
		for(int n=0;n<sizeSamples;n++){
			kn=k*n;
			num.real += ((arr[n]) * cos(kn*x));
			num.ima -= ((arr[n])*sin(kn*x));
		}
		num.real=num.real/MAXVAL;
		num.ima=num.ima/MAXVAL;

		if(*Max<abs(num.real))
			*Max=abs(num.real);
		else if(*Max<abs(num.ima))
			*Max=abs(num.ima);
		temp[k]=num;
		//printf("\n R : %d , I: %d ",temp[k].real,temp[k].ima);

	}
    //printf("\n  add \n");
  
	return temp;
}



Exp *TDFI(short * arr , int sizeSamples ,int *Max ){
    sizeSamples=sizeSamples/2;
	Exp *temp = (Exp*)malloc(sizeof(Exp)*sizeSamples);
	float x =2*PI/sizeSamples;
	int kn=0;

	for(int k=0;k<sizeSamples;k++){
            Exp num;
            num.ima=0;
            num.real=0;
            for(int n=0;n<sizeSamples;n++){
                kn=k*n;
                num.real +=arr[n*2+1]*sin(x*kn)-arr[n*2]*cos(x*kn);
                num.ima += arr[n*2]*sin(x*kn)+arr[n*2+1]*cos(x*kn);
            }
            num.real=(num.real/sizeSamples);
            num.ima=(num.ima/sizeSamples);
            	  
            if(*Max<abs(num.real))
                *Max=abs(num.real);
            if(*Max<abs(num.ima))
                *Max=abs(num.ima);
            temp[k]=num;
      // printf("\n R : %x , I: %x ",temp[k].real,temp[k].ima);



	}

	return temp;
}




int main(int arcgs , char *args[])
{

        /******************Reading Header********************/

		FILE *fileptr;
		FILE *fileout;
		char  *buffer;
		char *endFile;
		//char nwname[]="Nuevo-";
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
        int op;
		printf("block : %d  numChan: %d , bitsperSample:%d ,size : %d   , %d   sample Rate %d \n",blockAlign,numChannels ,btpSm ,siz,sizeof(float),sampleRate);

		printf("Seleccionar una Opcion: \n");
		printf("TDF...................(1)\nTDFI...................(2)\n");
		scanf("%d",&op);
        short *samples;
        int Max=0;
        Exp *arr;
        short * temp;
        samples=getData(siz/2,fileptr);
		switch(op){
            case 1:
                //FINAL TDF
               /* int mod;	
					printf("Seleccionar una Opcion: \n");
					printf("Calcular Modulo\n SI ...................(1)\n NO...................(0)\n");
					scanf("%d",&mod);*/
				//	printf("Mod : %d",mod);
                arr = TDF(samples,siz/2,&Max);
                temp = toShortArray(arr,siz/2,&Max,0);
                CreateFileWAV(OUTFILE ,temp,siz,sampleRate,btpSm,2,sizeof(short));
                printf("\n%d \n",Max);

                break;

            case 2:

                arr = TDFI(samples,siz/2,&Max);
                temp = toShortArray(arr,siz/4,&Max,1);
                CreateFileWAV(OUTFILE ,temp,siz/2,sampleRate,btpSm,2,sizeof(short));
                printf("\n%d \n",Max);
                break;



		}





        	fclose(fileptr);
			//fclose(fileout);
			free(buffer);
			free(arr);
	return 0;

}
