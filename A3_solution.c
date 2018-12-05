
/* FILE: A3_solutions.c is where you will code your answers for Assignment 3.
 * 
 * Each of the functions below can be considered a start for you. They have 
 * the correct specification and are set up correctly with the header file to
 * be run by the tester programs.  
 *
 * You should leave all of the code as is, especially making sure not to change
 * any return types, function name, or argument lists, as this will break
 * the automated testing. 
 *
 * Your code should only go within the sections surrounded by
 * comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A3_solutions.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "A3_provided_functions.h"

unsigned char*
bmp_open( char* bmp_filename,        unsigned int *width, 
          unsigned int *height,      unsigned int *bits_per_pixel, 
          unsigned int *padding,     unsigned int *data_size, 
          unsigned int *data_offset                                  )
{
  unsigned char *img_data=NULL;
  // REPLACE EVERYTHING FROM HERE
	FILE *bmpfile = fopen(bmp_filename, "rb");

	char b, m;
	fread(&b, 1, 1, bmpfile);
	fread(&m, 1, 1, bmpfile);

	fread(data_size, 1, sizeof(unsigned int), bmpfile);

	rewind(bmpfile);

	img_data = (unsigned char *)malloc(sizeof(unsigned char) * (*data_size));

	fread(img_data, 1, *data_size, bmpfile);

	*width = *(unsigned int*)(img_data+18);

	*height = *(unsigned int*)(img_data+22);

	*bits_per_pixel = *(unsigned short int*)(img_data+28);

	*data_offset = *(unsigned int*)(img_data+10);

	int numOfColors = *(bits_per_pixel)/8;

	*padding = ((4-((*width * numOfColors)%4))%4);


 // TO HERE!
  return img_data;
}

void 
bmp_close( unsigned char **img_data )
{
  // REPLACE EVERYTHING FROM HERE

	free(*img_data);
	*img_data = NULL;

  // TO HERE!  
}

unsigned char***  
bmp_scale( unsigned char*** pixel_array, unsigned char* header_data, unsigned int header_size,
           unsigned int* width, unsigned int* height, unsigned int num_colors,
           float scale )
{
 unsigned char*** new_pixel_array = NULL; 
  // REPLACE EVERYTHING FROM HERE

	unsigned int newheight = (unsigned int)((*height)*scale);
	*height = newheight;
	unsigned int newwidth = (unsigned int)((*width)*scale);
	*width = newwidth;
	unsigned int pixel_size = (unsigned int)((newwidth)*(newheight)*(num_colors));
	unsigned int padding = ((4-((newwidth*num_colors)%4))%4);
	*(((unsigned int*)(header_data+2))) = (header_size + pixel_size + (newheight*padding));
	*(((unsigned int*)(header_data+18))) = newwidth;
	*(((unsigned int*)(header_data+22))) = newheight;

	new_pixel_array = (unsigned char***)malloc(sizeof (unsigned char**) * (newheight));

	if(pixel_array == NULL) {
		return NULL;
	}

	for(int row=0; row<newheight; row++) {
	new_pixel_array[row] = (unsigned char**)malloc(sizeof(unsigned char*) * (newwidth) );
	for(int col=0; col<newwidth; col++) {
	new_pixel_array[row][col] = (unsigned char*)malloc(sizeof(unsigned char) * (num_colors));
		}
	}

	for(int row=0; row<newheight; row++) {
		for(int col=0; col<newwidth; col++) {
			for(int color=0; color<num_colors; color++) {
	new_pixel_array[row][col][color] = pixel_array[(int)(row/scale)][(int)(col/scale)][color];
			}
		}
	}

  // TO HERE! 
  return new_pixel_array;

}

int 
bmp_collage( char* background_image_filename,     char* foreground_image_filename, 
             char* output_collage_image_filename, int row_offset,                  
             int col_offset,                      float scale )
{
 // REPLACE EVERYTHING FROM HERE

	unsigned char* bgHeaderData;
	unsigned int bgHeaderSize;
	unsigned int bgHeight;
	unsigned int bgWidth;
	unsigned int bgNumCol;

	unsigned char* fgHeaderData;
	unsigned int fgHeaderSize;
	unsigned int fgHeight;
	unsigned int fgWidth;
	unsigned int fgNumCol;


	unsigned char *** bg_pixel_array = NULL;
	bg_pixel_array = bmp_to_3D_array (background_image_filename, 
	&bgHeaderData, &bgHeaderSize, &bgWidth, &bgHeight, &bgNumCol);


	unsigned char *** fg_pixel_array = NULL;
	fg_pixel_array = bmp_to_3D_array (foreground_image_filename, 
	&fgHeaderData, &fgHeaderSize, &fgWidth, &fgHeight, &fgNumCol);

	unsigned char *** fg_scaled_array = bmp_scale (fg_pixel_array, fgHeaderData, fgHeaderSize,
	&fgWidth, &fgHeight, fgNumCol, scale);

	if(bg_pixel_array == NULL || fg_pixel_array == NULL) {
		printf("Could not open one of the files.\n");
		return -1;
	}

	if(bgNumCol != 4 || fgNumCol !=4) {
		printf("Both files must be 32 bpp. Try with files that meet requirement.\n");
		return -1;
	}

	if(fgHeight + row_offset > bgHeight) {
		printf("Foreground image is too tall or background image is too small.\n");
		return -1;
	}

	if(fgWidth + col_offset > bgWidth) {
		printf("Foreground image too wide or background is too small.\n");
		return -1;
	}

	for(int row=0; row<bgHeight; row++) {
		for(int col=0; col<bgWidth; col++) {
			for(int color=0; color<bgNumCol; color++) {
				if(row>=row_offset && row<row_offset+fgHeight && 
				col>=col_offset && col<col_offset+fgWidth && 
				fg_scaled_array[row-row_offset][col-col_offset][0] != 0) {
				bg_pixel_array[row][col][color] = fg_scaled_array[row-row_offset]
				[col-col_offset][color];
				}
			}
		}
	}

	bmp_from_3D_array(output_collage_image_filename, bgHeaderData, bgHeaderSize, 
	bg_pixel_array,	bgWidth, bgHeight, bgNumCol);

 // TO HERE!
  return 0;
}              

