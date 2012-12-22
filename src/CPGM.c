/**
 * @file CPGM.c
 * @brief PGM(P2) Image Library Implementation
 * @author Oneonestar <oneonestar@gmail.com>
 * @version 1.0
 * @date 2012-10-27
 * @copyright 2012 Oneonestar
 *
 * @section LICENSE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "CPGM.h"

#define MAX_FILE_BUFFER 255

/*!Null PGM*/
static const PGM nullImg={"", -1, -1, -1, {0}};

static int readNum(FILE* file);

static int readNum(FILE* file)
{
	char c;
	char charNum[50];
	char *charNumPtr=charNum;
	int length=0;
	/*clear front space*/
	do
	{
		c=getc(file);
		if(isspace(c))
			continue;
		else if(isdigit(c))
			break;
		else
			return -1;	/*not digit && not space => error*/
	}while(1);

	/*read a number*/
	do
	{
		if(isspace(c)||c==EOF)
			break;
		if(length>49)
			return -1;	/*some ridiculous long number*/
		if(!isdigit(c))
			return -1;	/*not digit && not space => error*/
		*charNumPtr = c;
		charNumPtr++;
		length++;
		c=getc(file);
	}while(1);
	*charNumPtr = '\0';
	return atoi(charNum);
}

int isNullPGM(const PGM *image)
{
	return !memcmp(&nullImg, image, sizeof(PGM));
}

void setNullPGM(PGM *image)
{
	memcpy(image, &nullImg, sizeof(PGM));
}

int readFilePGM(FILE *file, PGM *image)
{
    PGM tempImg;
    int i;
	char c;
	int temp;
	/*first line*/
	c = fgetc(file);
	if(c!='P')
		return -1;	/*File Format Error*/
	c = fgetc(file);
	if(c!='2')
		return -1;	/*File Format Error*/
	
	/*clear line one space and read to next line*/
	do
	{
		c=fgetc(file);
		if(c=='\n')
			break;
		if(c==EOF||!isspace(c))		/*if some EOF or non-space char in line 1*/
			return -1;
	}while(1);

	/*if comment, ignore until LF*/
	c=fgetc(file);
	if(c==EOF)
		return -1;
	if(c=='#')
	{
		do
		{
			c=fgetc(file);
			if(c=='\n')
				break;
			if(c==EOF)		/*if EOF in the comment line*/
				return -1;
		}while(1);
	}
	else
		ungetc(c, file);	/*put back the character*/

	
	/*width and height and greyMax*/
	tempImg.width = readNum(file);
	tempImg.height = readNum(file);
	tempImg.greyMax = readNum(file);
	if(tempImg.width<0 || tempImg.height<0 || tempImg.greyMax<=0)
		return -1;
	if(tempImg.width>300 || tempImg.height>300 || tempImg.greyMax>255)
		return -1;

	/*read pixel*/
	for (i=0; i< tempImg.width * tempImg.height; i++)
	{
        temp = readNum(file);
		if(temp<0)
			return -1;
		tempImg.pixelData[i] = temp;
	}
	
	/*store the correct image into program*/
	reset(image);
    memcpy(image, &tempImg, sizeof(PGM));
	return 0;
}

int writeFilePGM(FILE *file, const PGM *image, int useGroupComment)
{
    fprintf(file, "P2\n");
	if(useGroupComment)
	    fprintf(file, "# PGM image output by 11-02, Team 1, ICP Project, 321, 2012\n");
	else
		fprintf(file, "#%s\n", image->comment);
	
    fprintf(file, "%d %d\n", image->width, image->height);
    fprintf(file, "%d\n", image->greyMax);
    printPixelPGM(file, image, NULL);
	return 0;
}


void printPixelPGM(FILE *file,const PGM *image, char* specChar)
{
    int w, h, i;
    if(specChar==NULL)	/*Print exact value*/
    {
        for(h=0; h < image->height; h++)
        {
            for(w=0; w < image->width; w++)
            {
                fprintf(file, "%d ", image->pixelData[h*image->width + w]);
            }
            fprintf(file, "\n");
        }
    }
    else	/*Character view*/
    {
        float denominator = ((float)image->greyMax) / (strlen(specChar));
        char tempChar;
        for(h=0; h < image->height; h++)
        {
            for(w=0; w < image->width; w++)
            {
				i = ((int)(image->pixelData[h*image->width + w] / denominator));
				i = (i==strlen(specChar))? i-1: i;
                tempChar = specChar[i];
                fprintf(file, "%c", tempChar);
			}
            fprintf(file, "\n");
        }
    }
}

int embedInfoPGM(PGM *image, char* info)
{
	int temp;
	unsigned int i;	/*ifn => compiler warning b/c "size_t strlen()"*/
	if(image->greyMax<9)
		return 1;
	for(i=0; i<strlen(info); i++)
	{
		temp = image->pixelData[i]/10*10+(int)info[i]-48;
		temp = temp>image->greyMax?temp-10:temp;
		image->pixelData[i] = temp;
	}
	return 0;
}


void printAttPGM(FILE *file,const PGM *image)
{
    fprintf(file, "Expected descriptor P2\n");
    fprintf(file, "Image Wdith = %d\n", image->width);
    fprintf(file, "Image Height = %d\n", image->height);
    fprintf(file, "Image Greyscale MAX = %d\n", image->greyMax);
    fprintf(file, "Image w[%d], h[%d], max[%d]\n", image->width, image->height, image->greyMax);
}

void reset(PGM *image)
{
    memset(image, 0, sizeof(PGM));
}


void negative(PGM *image)
{
	int i;
	for(i=0; i<image->width*image->height; i++)
	{
		image->pixelData[i] = image->greyMax - image->pixelData[i];
	}
}

void horizontalFlip(PGM *image)
{
	int h, w;
	PGM tempImg;
	memcpy(&tempImg, image, sizeof(PGM));
	/*flip*/
	for(h=0; h < image->height; h++)
		for(w=0; w < image->width; w++)
			tempImg.pixelData[h*image->width + w] = image->pixelData[(image->width - w) + h*image->width];
	/*copy back*/		
	reset(image);
    memcpy(image, &tempImg, sizeof(PGM));
}

void verticalFlip(PGM *image)
{
	int h, w;
	PGM tempImg;
	memcpy(&tempImg, image, sizeof(PGM));
	/*flip*/
	for(h=0; h < image->height; h++)
		for(w=0; w < image->width; w++)
			tempImg.pixelData[h*image->width + w] = image->pixelData[(image->height - h)*image->width + w];
	/*copyback*/
	reset(image);
    memcpy(image, &tempImg, sizeof(PGM));
}

void rotate90C(PGM *image)
{
	int h, w;
	PGM tempImg;
	memcpy(&tempImg, image, sizeof(PGM));
	tempImg.height = image->width;
	tempImg.width = image->height;
	/*rotate*/
	for(h=0; h < image->height; h++)
		for(w=0; w < image->width; w++)
			tempImg.pixelData[w*tempImg.width + tempImg.width-h-1] = image->pixelData[h*image->width + w];
	/*copyback*/
	reset(image);
    memcpy(image, &tempImg, sizeof(PGM));
}

