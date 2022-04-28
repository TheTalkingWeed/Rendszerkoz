#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include<dirent.h>
#include <signal.h>
#include<sys/stat.h>
#include<pwd.h>


int Measurement(int **Values)
{
  time_t T1;
  int c, n, T2, asd;
  T2 = time(&T1);
  int size = T2 % 900;
	int *seged = malloc(size * (sizeof(int)));


  *Values = seged;

  if(size < 100){
  	size = 100;
  }

  (*Values)[0] = 0;
  for (int i = 1; i < size; i++) {
  double rf = (double)rand()/((unsigned)RAND_MAX+1);
  double minusz = (double)11/31 + 0.4285;

    if(rf <= 0.4285){
      seged[i] = seged[i - 1] + 1;
    }
    else if(rf <= minusz)
    {
    	seged[i] =seged[i - 1] - 1;
    }else
    {
    	seged[i] = seged[i - 1] + 0;
    }


  }

  return size;
}


int oszthato(int num)
{
	if(num % 32 == 0)
		return 0;
	else
		return ((num / 32 + 1) * 32) - num;
}

int megfelelo(int range, int num)
{
	if(range % 2 == 0)
	{
		if (num > range / 2 - 1)
			return range / 2 - 1;
		if(num < (-range / 2))
			return -range / 2;
		return num;
	}
	else
	{
		if(num > range / 2)
			return range / 2;
		if(num < -range / 2)
			return -range / 2;
		return num;
	}
}

void BMPcreator(int *values, int numValues){
	typedef struct
	{
		unsigned int fileSize;
		unsigned int unused;
		unsigned int offset;
		unsigned int DIB_headerSize;
		unsigned int width;
		unsigned int height;
		short planes;
		short bits;
		unsigned int compression;
		unsigned int imgSize;
		unsigned int horizontalPixel;
		unsigned int verticalPixel;
		unsigned int colorsPalette;
		unsigned int usedColors;
		char color0[4];
		char color1[4];
	}Header;


	Header header;
	header.fileSize = 62 + ((numValues + oszthato(numValues)) / 8) * numValues ; /////
	header.unused = 0;
	header.offset = 62;
	header.DIB_headerSize = 40;
	header.width = numValues;
	header.height = numValues;
	header.planes = 1;
	header.bits = 1;
	header.compression = 0;
	header.imgSize = 0;
	header.horizontalPixel = 3937;
	header.verticalPixel = 3937;
	header.colorsPalette = 0;
	header.usedColors = 0;
	header.color0[0] = 0;
	header.color0[1] = 255;
	header.color0[2] = 255;
	header.color0[3] = 255;
	header.color1[0] = 255;
	header.color1[1] = 0;
	header.color1[2] = 0;
	header.color1[3] = 255;

	int out = open("chart.bmp",O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRGRP | S_IROTH);

	char *cloc = calloc(header.fileSize - 62, 1);

	int seged;

	for(int i = 0; i < numValues; i++)
	{

		seged = i + (numValues + oszthato(numValues)) * ((numValues / 2)
		+ megfelelo(header.height,values[i]));


		char tmp = 1 << 7 -(seged % 8);

		cloc[seged / 8] |= tmp;
	}

	write(out, "BM", 2 * sizeof(char));
	write(out, &header, sizeof(header));
	write(out, &*cloc, header.fileSize - 62 );
	close(out);
	free(cloc);
}


char isNumber(char *text)
{
    int j;
    j = strlen(text);
    while(j--)
    {
        if(text[j] > 47 && text[j] < 58)
            continue;

        return 0;
    }
    return 1;
}


int FindPID()
{
	DIR *d, *jelen;
	FILE *f1;
  struct dirent *entry;
  char seged2[100] = {0};


  d=opendir("/proc");

  while((entry=readdir(d))!=NULL){
		if( isNumber((*entry).d_name))
		{



			char path[100] = {0};

			strcat(path, "/proc/");
			strcat(path	, (*entry).d_name);
			strcat(path, "/status");



			int  f2=open( path ,O_RDONLY);


			read(f2, &seged2, 12);
			close(f2);



			if(strcmp(seged2, "Name:\tchart\n") == 0)
			{
				return atoi((*entry).d_name);
				printf(" asd, %s\n", seged2);
			}

		}

  
    }
	return -1;
  closedir(d);
  printf("\n");
}
