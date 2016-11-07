#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



#define Ntap 10
#define PI 3.14159265358979323846

//static float x[Ntap];
//typedef  Sample;

void * getData(int size, FILE * fileWave ,int typeSize){
		void * arr= (void*)malloc(typeSize*size);
		fread(arr, size,typeSize, fileWave);
		return arr;
}

int setGain(float max , float num){
	return num*127/max;
}







float * Coefun(int fc , int fs , int n  ){
	float Wc= ((PI*fc)*2)/fs;
	float RC= 1/(PI*Wc*2);
	float *h = (float *)malloc(sizeof(float)*n);
	for(int i=0;i<n;i++)
		*(h+i)=pow(2.718281,-i*RC);
	return h;
}



float fir(float NewSample ,float *h ,float *gain) {
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




void  * Conv(void * arr , int size, int BlockSize){
	float * temp = (float *)malloc(sizeof(float)*size);
	//float * x = (float *)malloc(sizeof(float)*Ntap);
	float CDgain=0;
    float *h =Coefun(1000,10000,Ntap);
	for(int j=0;j<Ntap;j++){
		printf(" \n %f" ,h[j]);
	
	}
	
		
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
	free(temp);
	
	return result;
}








int main(int arcgs , char *args[])
{
	  	
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
		fileout = fopen(strcat(nwname,args[1]),"wb");
		if(fileptr==NULL | fileout==NULL)
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
		//float *h =Coefun(1000,10000,Ntap,&CDgain);
		//float *x=(float*)malloc(sizeof(float)*Ntap);
		//int size=filelen-74-44;
		int sample=45,blockAlign=*(buffer+32),numChannels=*(buffer+22),btpSm=*(buffer + 34) ;
		char unsigned a[]={*(buffer + 40),*(buffer + 41),*(buffer + 42),*(buffer + 43)};
		int siz = (a[3]<<24) | (a[2]<<16) | (a[1]<<8) | (a[0]);
		//typedef short Sample;
		printf("block : %d  numChan: %d , bitsperSample:%d ,size : %d   , %d \n",blockAlign,numChannels ,btpSm ,siz,sizeof(short));
			
		printf("block : %d  numChan: %d , bitsperSample:%d ,size : %d   , %d",blockAlign,numChannels ,btpSm ,siz,sizeof(float));
		void  *arr;
		arr=getData(siz,fileptr,blockAlign/2);
		arr=Conv(arr,siz,blockAlign);
		fwrite(buffer,44,1,fileout);
		fwrite(arr,siz,1,fileout);
	
	
	
	
	/*	if(blockAlign==2){
				arr=getData(siz,fileptr,sizeof(char));
				arr=Conv(arr,siz,2);
				fwrite(buffer,44,1,fileout);
				fwrite(arr,siz,1,fileout);
				
		}else{
			
				arr=getData(siz,fileptr,sizeof(short));
				arr=Conv(arr,siz,4);
				fwrite(buffer,44,1,fileout);
				fwrite(arr,siz,1,fileout);
		}		*/
		fread(endFile, 72,sizeof(char), fileptr);
		fwrite(endFile,72,sizeof(char),fileout);

		
		/*
		
		float max=0;
		for(int j=0;j<Ntap;j++){
			printf("%f \n",h[j]);
		}
	
		
		while(sample<size){
			printf(" %d   ",	*(buffer+sample));
			*(buffer+sample)= fir(*(buffer+sample),h,CDgain,x);
			if(*(buffer+sample)>max){
				max=*(buffer+sample);
				printf("  Maximo : %f",max);
			}
			sample+=blockAlign;
		}
		
		
		if(numChannels==2){
			sample=44;
			while(sample<size){
				*(buffer+sample)=  fir(*(buffer+sample),h,CDgain,x);
				sample+=blockAlign;
			}
		}
		sample=45;
		printf("SAlida");
		while(sample<size){
			
			*(buffer+sample)=getGain(max,*(buffer+sample));
			printf(" %d   ",	*(buffer+sample));
			sample+=blockAlign;
		}
		
		
		
		fwrite(buffer,filelen,1,fileout);
		printf("Conversión Exitosa , Nombre de archivo : %s  %f",nwname,max);
*/
        	fclose(fileptr);
			fclose(fileout);
			free(buffer);
			free(arr);
	return 0;
	
}
