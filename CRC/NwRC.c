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

//static float x[Ntap];
//typedef  Sample;
/*

void * getData(int size, FILE * fileWave ,int typeSize){
		void * arr= (void*)malloc(typeSize*size);
		fread(arr, size,typeSize, fileWave);
		return arr;
}*/

short setGain(float max , float num){
	return num*MAXVAL/max;
}






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

}

/*

short * toShortArray(Exp *result,int samples , int *Max , int invert){
    short *temp = (short*)malloc(sizeof(short)*samples*2);


    if(invert){
    	for(int i=0;i<samples;i++){
        temp[i*2]=~(short)((result[i].real)*MAXVAL/ *Max);
        temp[i*2+1]=~(short)((result[i].ima)*MAXVAL/ *Max);
        //temp[i*2]= ~temp[i*2];
       // temp[i*2+1]= ~temp[i*2+1];
        //temp[i*2+1]=0;
        //printf(" \n  R : %d  ,   I : %d",temp[i*2],temp[i*2+1]);
        //temp[i*2]= ((temp[i*2]<<16)|(temp[i*2]>>16));
       // temp[i*2+1]= ((temp[i*2+1]<<16)|(temp[i*2+1]>>16));
               // printf(" \n R : %x(%d)  , I : %x(%d)  ",temp[i*2],temp[i*2],temp[i*2+1],temp[i*2+1]);
        //free(result[i]);
    	}
	}else{

		for(int i=0;i<samples;i++){
        temp[i*2]=(short)((result[i].real)*MAXVAL/ *Max);
        temp[i*2+1]=(short)((result[i].ima)*MAXVAL/ *Max);
        //temp[i*2]= ~temp[i*2];
       // temp[i*2+1]= ~temp[i*2+1];
        //temp[i*2+1]=0;
        //printf(" \n  R : %d  ,   I : %d",temp[i*2],temp[i*2+1]);
        //temp[i*2]= ((temp[i*2]<<16)|(temp[i*2]>>16));
       // temp[i*2+1]= ((temp[i*2+1]<<16)|(temp[i*2+1]>>16));
               // printf(" \n R : %x(%d)  , I : %x(%d)  ",temp[i*2],temp[i*2],temp[i*2+1],temp[i*2+1]);
        //free(result[i]);
    	}
	}



    return temp;
}


*/









float * Coefun(float fc , float fs , int n  ){
	float Wc= ((PI*fc)*2)/fs;
	float RC= 1.0/(PI*Wc*2);
	float *h = (float *)malloc(sizeof(float)*n);
	for(int i=0;i<n;i++)
		*(h+i)=pow(2.718281,-i*RC);
	return h;
}



float fir(float NewSample ,float *h ,float *gain ) {
	//sprintf("  %f  -",NewSample);
    static float x[Ntap];
    float y=0;
    int n;
    for(n=Ntap-1; n>0; n--){

    	 x[n] = x[n-1];
	}

    x[0] = NewSample;

    for(n=0; n<Ntap; n++)
    	y += h[n]*x[n];

	if(y>*gain)
		*gain=y;
	return y;
}

short *Pulse(int size){
    short *temp =(short*)malloc(sizeof(short)*size);
    for(int i=0;i<size/32;i++){
        temp[i*2]=0;
        temp[i*32+1]=32512;
    }
    return temp;



}





void  * Conv(short * arr , int size,int sampleRate){
	float * temp = (float *)malloc(sizeof(float)*size);
	float CDgain=0;
    float *h =Coefun(1000.00,10000,Ntap);
	for(int j=0;j<Ntap;j++){
		printf(" \n %f" ,h[j]);

	}
    for(int i=0;i<size;i++){
        temp[i]=fir(arr[i]/MAXVAL,h,&CDgain);
    }
    printf(" \n Out \n");
    for(int i=0;i<size;i++){
        arr[i]=setGain(CDgain,temp[i]);
      //  printf(" %x ",arr[i]);
    }
    free(temp);

/*

   	void *result;

	  		char * arrC=(char*)arr;
	  		int i=1;
	  		printf("temp \n");

	  		while(i<size){
	  			temp[i]=fir((float)arrC[i],h,&CDgain);
				//printf(" %f - ",temp[i]);
				i+=BlockSize;
			}
	  		i=1;
			printf("Salida \n");
			while(i<size){
    			arrC[i]=setGain(CDgain,temp[i]);
				//printf(" %d   ",arrC[i]);
    			i+=BlockSize;
			}
			result=arrC;





	//free(x);
	free(h);
	free(temp);*/

	//return result;
}








int main(int arcgs , char *args[])
{


 /******************Reading Header********************/

		FILE *fileptr;
		FILE *fileout;
		char  *buffer;
		char *endFile;
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

        short *samples;
  //      int Max=0;
//        Exp *arr;
        short * temp;
        samples=getData(siz/2,fileptr);
        Conv(samples,siz/2,sampleRate);
        CreateFileWAV(OUTFILE,samples,siz/2,sampleRate,btpSm,1,sizeof(short));
       // CreateFileWAV(OUTFILE,Pulse(750),750,2000,16,1,sizeof(short));
		/*switch(op){
            case 1:
                //FINAL TDF
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



		}*/





        	fclose(fileptr);
			//fclose(fileout);
			free(buffer);
			free(samples);
	return 0;




}
