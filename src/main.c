/**
 * @file main.c
 * @brief PGM(P2) Image Processor
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @def MAX_STRING_BUFFER
 * Max length of the command that may input
 */
#define MAX_STRING_BUFFER 255

/************************************************************************************//**
 * @defgroup UI
 * @brief User Interface functions
 * @details All UI functions are strong exception safety. Which means operations can 
 * fail(i.e. user inputs the wrong data), but failed operations are guaranteed to have 
 * no side effects so all data retain original values.
 * @{
****************************************************************************************/
/**
 * @brief Option 'c' - Create PGM
 * @details Create a PGM through stdin, store in memory and write to file.
 * @param[out] image The memory area to hold the image data.
 */
void cProcess(PGM *image);

/**
 * @brief Option 'e' - Effects apply to PGM file
 * @details Sub-menu for applying Effect to image.
 * @param[in, out] image The memory area to hold the image data.
 */
void eProcess(PGM *image);

/**
 * @brief Option 'm' - ID Marking
 * @details Steganography to embeds course code and student ID into the image.
 * @param[in, out] image The memory area to hold the image data.
 */
void mProcess(PGM *image);

/**
 * @brief Option 'r' - Read PGM file
 * @details Load PGM file into memory.
 * @param[out] image The memory area to hold the image data.
 */
void rProcess(PGM *image);

/**
 * @brief Option 'v' - Character-View
 * @details View the stored image with user-specified characters that represent pixel values.
 * @param[in] image The memory area to hold the image data.
 */
void vProcess(const PGM *image);

/**
 * @brief Option 'w' - Write to PGM file
 * @details Write the PGM to file. Strong exception safety.
 * @param[in] image The memory area to hold the image data.
 */
void wProcess(const PGM *image);

/**
 * @brief Print the main menu and the option list
 */
void printMainMenu();

/** @}
****************************************************************************************/





/************************************************************************************//**
 * @defgroup Safe Stdin Operations
 * @brief Functions to handle stdin operations
 * @details Handle the tricky things of using the standard input functions.
 * @{
****************************************************************************************/
/**
 * @brief Get filename from stdin & some validation works
 * @param[out] fileName
 */
void getFileName(char* fileName);

/**
 * @brief fgets from stdin and clear the remaining char in the stream
 * @param[out] str Pointer to an array of chars where the string read is copied.
 * @param[out] buffSize Maximum number of characters to be copied into str 
 * (including the terminating null-character).
 */
char* safeGetString(char* str, int buffSize);

/**
 * @brief sending a prompt and get a valid integer
 */
int safeGetInt(char *message, int min, int max);

/**
 * @brief get a number from CLI
 * @retval -1 invalid input
 */
int CLIReadNum(char* string);

/** @}
****************************************************************************************/

int checkOverwrite(char* fileName);

int main()
{
    char control[MAX_STRING_BUFFER];
    PGM image;
	setNullPGM(&image);
    do
    {
        printMainMenu();
		safeGetString(control, MAX_STRING_BUFFER);
        if(!strcmp(control, "r"))
			rProcess(&image);
		else if(!strcmp(control, "c"))
            cProcess(&image);
		else if(!strcmp(control, "w"))
            wProcess(&image);
		else if(!strcmp(control, "v"))
			vProcess(&image);
		else if(!strcmp(control, "m"))
			mProcess(&image);
		else if(!strcmp(control, "e"))
			eProcess(&image);
		else if(!strcmp(control, "q"))
            break;
		else
            printf("\n\n>>> UNKNOWN '%s' option selected. Plerase enter your choice again:\n", control);
    } while(1);
	printf("\n>>> Option 'q'!\nBye.");
    return 0;
}

void rProcess(PGM *image)
{
    char fileName[FILENAME_MAX];
	FILE *file;
    printf("\n\nOption 'r' selected: Read P2 PGM Image, Store & Display ASCII Data...\n");
    printf("Please enter the <P2> PGM image file name: ");
    getFileName(fileName);
    file = fopen(fileName, "r");
    if(file == NULL)
    {
		printf("CANNOT open file: %s.  Do nothing!\n\n", fileName);
        printf(">> Cannot read image... Option 'r' Aborted!");
        return;
    }

    if(readFilePGM(file, image)<0)
	{
		printf("File Content Error!");
		fclose(file);
		return;
	}
    printAttPGM(stdout, image);
    printPixelPGM(stdout, image, NULL);
	fclose(file);
    printf("\n\n>>> Option 'r' Finished!");
}

void cProcess(PGM *image)
{
    char fileName[255];
	char temp[255];
    PGM tempImg;
	FILE *file;
	int w, h;
    /*File*/
    printf("Option 'c' selected: Create a New P2 Image, Store, with User Input Data...\n");
    printf("Please enter the NEW <P2> PGM image file name: ");
	getFileName(fileName);
	/*if user don't want to overwrite => exit*/
	if(checkOverwrite(fileName))
		return;
    /*Comment*/
    printf("Please add a comment line, ended with an <Enter>;\n");
    printf("(Just press <Enter> if no comment: ");
    safeGetString(tempImg.comment, MAX_COMMENT_LENGTH);
    printf("Your comment line is [#%s]\n", tempImg.comment);
    /*w, h, max*/
    tempImg.width = safeGetInt("Please input the width (Integer expected, less than 100): ", 1, 100);
    tempImg.height = safeGetInt("Please input the height (Integer expected, less than 100): ", 1, 100);
    tempImg.greyMax = safeGetInt("Please input the MAX greyscale level (Integer expected, less than 256): ", 1, 255);
    printf("Image Width[%d], Height[%d], GreyMax[%d]\n", tempImg.width, tempImg.height, tempImg.greyMax);
    /*data*/
    printf("Please input data (total number of pixel = [%d])\n", tempImg.width*tempImg.height);
    for(h=0; h<tempImg.height; h++)
    {
        for(w=0; w<tempImg.width; w++)
        {
            sprintf(temp, "...Data for w[%d]h[%d]: ", w, h);
            tempImg.pixelData[h*tempImg.width+w] = safeGetInt(temp, 0, tempImg.greyMax);
        }
		printf("\n");
    }
    reset(image);
    memcpy(image, &tempImg, sizeof(PGM));

	/*save to file*/
	file = fopen(fileName, "w");
    if(file==NULL)
    {
        printf("CANNOT open file: %s.  Do nothing!\n\n", fileName);
        printf(">> Cannot write image... Option 'c' Aborted!");
        return;
    }
	printf("\nFile [%s] Created and Output OK.\n\n", fileName);
	writeFilePGM(file, image, 0);
	fclose(file);
}

void wProcess(const PGM *image)
{
	char fileName[FILENAME_MAX];
	char control[MAX_STRING_BUFFER];
	FILE *file;
	printf("Option 'w' selected: Write the stored image, if any...\n");
	if(isNullPGM(image))
	{
		printf("\n>> No Input Image Stored... Option 'w' Aborted!");
		return;
	}
	printf("Please enter the NEW <P2> PGM image file name: ");
	safeGetString(fileName, FILENAME_MAX);
	/*if user don't want to overwrite => exit*/
	if(checkOverwrite(fileName))
		return;

	/*write*/
    file = fopen(fileName, "w");
    if(file==NULL)
    {
        printf("CANNOT open file: %s.  Do nothing!\n\n", fileName);
        printf(">> Cannot write image... Option 'w' Aborted!");

        return;
    }
	writeFilePGM(file, image, 1);
	fclose(file);
	printf("\n>>>Option 'w' Finished!\n");
}

void vProcess(const PGM *image)
{
	char sequence[255];
	printf("Option 'v' selected: Character-View: Display image with specified characters...\n");
	if(isNullPGM(image))
	{
		printf("\n>> No Input Image Stored... Option 'v' Aborted!");
		return;
	}
	do
	{
		printf("Please enter a sequence of characters, representing ZERO to MAX grey -levels:\n");
		safeGetString(sequence, FILENAME_MAX);
	}while(strlen(sequence)==0);
	printPixelPGM(stdout, image, sequence);
	printf("\n>>>Option 'v' Finished!\n");
}

void mProcess(PGM *image)
{
	int i, status;
	printf("Option 'm' selected: ID Marking...");
	if(isNullPGM(image))
	{
		printf("\n>> No Input Image Stored... Option 'm' Aborted!");
		return;
	}
	printf("\nPlease input the numbers: ");
	i = safeGetInt("Please input the team member number:\n1. Star (10552020)\
\n2. CK (10549919)\
\n3. Harvey (10548814)\
\n4. John (10548799)\nSelect: ", 1, 4);
	switch(i)
	{
		case 1:
			status = embedInfoPGM(image, "32110552020");
			break;
		case 2:
			status = embedInfoPGM(image, "32110549919");
			break;
		case 3:
			status = embedInfoPGM(image, "32110548814");
			break;
		case 4:
			status = embedInfoPGM(image, "32110548799");
			break;
	}
	if(status)
	{
		printf("\nImage Grey Max < 10, Option 'm' Terminated!\n");
		return;
	}
    printPixelPGM(stdout, image, NULL);
	printf("\n>>>Option 'm' Finished!\n");
}

void eProcess(PGM *image)
{
	int i;
	printf("Option 'e' selected: Image Effect...\n");
	if(isNullPGM(image))
	{
		printf("\n>> No Input Image Stored... Option 'e' Aborted!");
		return;
	}
	printf("Available options below:\n\
'1': Negative\n\
'2': Horizontal Flip\n\
'3': Vertical Flip\n\
'4': Rotate 90 Clockwise\n\
'5': Rotate 90 Anti-Clockwise\n\
'6': Rotate 180 Clockwise\n\n");
	i = safeGetInt("Please select effect: ", 1, 6);
	switch(i)
	{
		case 1:
			negative(image);
			break;
		case 2:
			horizontalFlip(image);
			break;
		case 3:
			verticalFlip(image);
			break;
		case 4:
			rotate90C(image);
			break;
		case 5:
			rotate90C(image);
			rotate90C(image);
			rotate90C(image);
			break;
		case 6:
			rotate90C(image);
			rotate90C(image);
			break;
	}
    printf("\n\n>>> Option 'e' Finished!");
}

void printMainMenu()
{ 
    printf("\n\n\
===================< PGM Image Processor >=====================\n\
===============< ICP Class 11-02, TEAM 01, 2012 >==============\n\
===========< TEAM Members: Star, CK, Harvey, John >============\n\n");
    printf("\n\n\
===============================MAIN MENU=================================\n\
Available options below:\n\
'r': READ, STORE and DISP:   Read P2 PGM Image, Store, & Dispaly Data\n\
'c': CREATE, STORE:          Create a New Image, & store (Manual Input)\n\
'w': WRITE IMAGE FILE:       Write (Output) the Stored Image, if any\n\
'v': CHAR-VIEW IMAGE:        Character-View the Stored Image\n");
	printf("\
'm': IDMARK IMAGE:           Create ID Marking to Image\n\
'e': IMAGE EFFECT ADDED:     Create and ADD Effect to Image\n\
'q': QUIT:                   Quit Porgram\n\
=========================================================================\n\n\
Please enter your option character, followed by an <Enter> key:");
}


void getFileName(char* fileName)
{
    safeGetString(fileName , FILENAME_MAX);
	/*! @todo filename validation*/
}


char* safeGetString(char* str, int buffSize)
{
	char *pos;
	if (!fgets(str, buffSize, stdin))
		return NULL;
	if ((pos = strchr(str, '\n')))
		*pos = '\0';	/*'\n' found, the current line is completely read*/
	else
	{
		/*'\n' is not found, there are still chars remain in the stdin*/
		char temp;
		while( (temp = getchar()) != '\n' && temp != EOF );
	}
	return str;
}


int safeGetInt(char *message, int min, int max)
{
	char str[MAX_STRING_BUFFER];
	int temp;
	do
	{
		printf(message);
		safeGetString(str, MAX_STRING_BUFFER);
		temp = CLIReadNum(str);
		if(temp<min || temp>max)
			printf("Invalid Input! Number:(%d - %d)\n", min, max);
		else
			return temp;
	}while(1);
}


int CLIReadNum(char* str)
{
	char c;
	char charNum[50];
	char *charNumPtr=charNum;
	int length=0;
	/*clear front space*/
	do
	{
		c=*str++;
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
		if(isspace(c)||c==EOF||c=='\0')
			break;
		if(length>49)
			return -1;	/*some ridiculous long number*/
		if(!isdigit(c))
			return -1;	/*not digit && not space => error*/
		*charNumPtr = c;
		charNumPtr++;
		length++;
		c=*str++;
	}while(1);
	*charNumPtr = '\0';
	return atoi(charNum);
}

int checkOverwrite(char* fileName)
{
	char temp[MAX_STRING_BUFFER];
	FILE *file = fopen(fileName, "r");
	if(file)
	{
		fclose(file);
		do
		{
			printf("File already exist, overwrite(Y/N): ");
			safeGetString(temp, MAX_STRING_BUFFER);
			if(!strcmp(temp, "Y")||!strcmp(temp, "y"))
				return 0;
			else if((!strcmp(temp, "N")||!strcmp(temp, "n")))
				return 1;
		}while(1);
	}
	return 0;
}
