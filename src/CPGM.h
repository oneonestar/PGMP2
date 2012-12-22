/**
 * @file CPGM.h
 * @brief PGM(P2) Image Library
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

#ifndef _CPGM_
#define _CPGM_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define DEF_MAX_PIXEL_W 300
#define DEF_MAX_PIXEL_H 300

/**
 * @def MAX_COMMENT_LENGTH
 * Max comment length in the PGM file
 */
#define MAX_COMMENT_LENGTH 255


/**A structure to represent a PGM (P2) file*/
typedef struct 
{
	char comment[MAX_COMMENT_LENGTH];	/*!< Comments show in the second line of the file*/
	int width;
	int height;
	int greyMax;
	unsigned char pixelData[DEF_MAX_PIXEL_W*DEF_MAX_PIXEL_H];	/*!< Pixel data range from 0-255.*/
}PGM;


/**
 * @brief Determine the input image is null or not.
 * @param image the input image
 * @retval 1 is equal to null
 * @retval 0 is not equal to null
 */
int isNullPGM(const PGM *image);

/**
 * @brief Set the input image to null
 * @pre image pointer is allocated
 * @param[out] null image
 */
void setNullPGM(PGM *image);

/**
 * @brief Read PGM file into memory
 * @param[in] file opened file pointer
 * @param[out] memory location of the image
 */
int readFilePGM(FILE *file, PGM *image);
int writeFilePGM(FILE *file, const PGM *image, int useGroupComment);
int embedInfoPGM(PGM *image, char* info);


/************************************************************************************//**
 * @defgroup UI
 * @brief User Interface functions
 * @details All UI functions are strong exception safety. Which means operations can 
 * fail(i.e. user inputs the wrong data), but failed operations are guaranteed to have 
 * no side effects so all data retain original values.
 * @{
****************************************************************************************/
/**
 * @brief Negative Effect
 */
void negative(PGM *image);
/**
 * @brief Horizontal Flip Effect
 */
void horizontalFlip(PGM *image);
/**
 * @brief Vertical Effect
 */
void verticalFlip(PGM *image);
/**
 * @brief Rotate90C Effect
 */
void rotate90C(PGM *image);


/** @}
****************************************************************************************/

void reset(PGM *image);
void printPixelPGM(FILE *file,const PGM *image, char* specChar);
void printAttPGM(FILE *file,const PGM *image);

#endif
