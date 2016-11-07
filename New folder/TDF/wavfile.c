
#include <string.h>
#include "wavfile.h"


struct wavfile_header {
	char	riff_tag[4];
	int	riff_length;
	char	wave_tag[4];
	char	fmt_tag[4];
	int	fmt_length;
	short	audio_format;
	short	num_channels;
	int	sample_rate;
	int	byte_rate;
	short	block_align;
	short	bits_per_sample;
	char	data_tag[4];
	int	data_length;
};

FILE * wavfile_open( char *filename ,int samples_per_second , int bits_per_sample , int channels)
{
	struct wavfile_header header;
/*
	int samples_per_second = WAVFILE_SAMPLES_PER_SECOND;
	int bits_per_sample = 16;*/

	strncpy(header.riff_tag,"RIFF",4);
	strncpy(header.wave_tag,"WAVE",4);
	strncpy(header.fmt_tag,"fmt ",4);
	strncpy(header.data_tag,"data",4);

	header.riff_length = 0;
	header.fmt_length = 16;
	header.audio_format = 1;
	header.num_channels = channels;
	header.sample_rate = samples_per_second;
	header.byte_rate = samples_per_second*(bits_per_sample/8);
	header.block_align = channels*bits_per_sample/8;
	header.bits_per_sample = bits_per_sample;
	header.data_length = 0;

	FILE * file = fopen(filename,"wb");
	if(file==NULL){
        printf("No puede Crearse el Archivo de Salida \n");
        exit(1);
	}
	fwrite(&header,sizeof(header),1,file);
	fflush(file);
	return file;

}

void wavfile_write( FILE *file, short data[], int length ,int blockAlign )
{
	fwrite(data,blockAlign,length,file);
}

void wavfile_close( FILE *file )
{
    char * endHeader= "LISTB...INFOISFT5...Goldwave (c) chris S. Craig, http://www.goldwave.com..";
	int file_length = ftell(file);

	int data_length = file_length - sizeof(struct wavfile_header);
	fseek(file,sizeof(struct wavfile_header) - sizeof(int),SEEK_SET);
	fwrite(&data_length,sizeof(data_length),1,file);

	int riff_length = file_length - 8;
	fseek(file,4,SEEK_SET);
	fwrite(&riff_length,sizeof(riff_length),1,file);
	fflush(file);
	fseek(file,0,SEEK_END);
	fwrite(endHeader,sizeof(char),74,file);


	fclose(file);
}

