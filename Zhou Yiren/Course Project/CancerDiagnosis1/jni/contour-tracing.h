/**
 * This algorithm is called Moore Neighbor Tracing
 * http://www.thebigblob.com/moore-neighbor-contour-tracing-algorithm-in-c/
 * http://blog.daum.net/chowood/8039477
 * An explanation of the algorithm can be found here:
 * http://www.imageprocessingplace.com/downloads_V3/root_downloads/tutorials/contour_tracing_Abeer_George_Ghuneim/moore.html
 *
 * @author Erik Smistad <smistad@idi.ntnu.no>
 */

/*
 * contour-tracing.h
 */

#ifndef CONTOUR_TRACING_H_
#define CONTOUR_TRACING_H_

#include <stdio.h>
#include <stdlib.h>
#include "my_point.h"

struct pixel
{
	unsigned char g;
	bool operator==(const pixel &other);
};
const pixel WHITE = {0};
const pixel BLACK = {255};

pixel * padImage(pixel * image, int width, int height, pixel paddingColor);
pixel * mooreNeighborTracing(pixel * image, int width, int height, point_list* list);

//--------------------implementation-------------------------
/**
 * Check to see if to pixels are equal
 */
bool pixel::operator==(const pixel &other)
{
	return g == other.g;
}
/**
 * Pads an image represented by a 1D pixel array with 1 pixel with a color
 * specified by paddingColor
 */
pixel * padImage(pixel * image, int width, int height, pixel paddingColor)
{
	//pixel * paddedImage = (pixel *)malloc(long(sizeof(pixel) * (height+2) * (width+2)));
	pixel* paddedImage = new pixel[sizeof(pixel) * (height+2) * (width+2)];
	for(int x = 0; x < width+2; x ++)
	{
		for(int y = 0; y < height+2; y ++)
		{
			if(x == 0 || y == 0 || x == width+1 || y == height+1)
			{
				paddedImage[x + y*(width+2)] = paddingColor;
			}
			else
			{
				paddedImage[x+y*(width+2)] = image[x-1 + (y-1)*width];
			}
		}
	}
	return paddedImage;
}
void convert_pos_to_coordinate(int pos, int width, int &r, int &c)
{
	r = int(pos/width);
	c = pos - r*width;
}
//----------------------------------------------------------
pixel * mooreNeighborTracing(pixel * image, int width, int height, point_list* list)
{
	bool inside = false;
	int pos = 0;

	// Need to start by padding the image by 1 pixel
	pixel * paddedImage = padImage(image, width, height, WHITE);

	// Allocate new image as a 1D array
//	pixel * borderImage = (pixel *)malloc(long(sizeof(pixel) * (height+2) * (width+2)));
	pixel * borderImage = new pixel[sizeof(pixel) * (height+2) * (width+2)];
	// Set entire image to WHITE
	for(int y = 0; y < (height+2); y ++)
	{
		for(int x = 0; x < (width+2); x ++)
		{
			borderImage[x + y*(width+2)] = WHITE;
		}
	}

	for(int y = 0; y < (height+2); y ++)//for(int x = 0; x < (width+2); x ++)//
	{
		for(int x = 0; x < (width+2); x ++)//for(int y = 0; y < (height+2); y ++)//
		{
			pos = x + y*(width+2);

			// Scan for BLACK pixel
			if(borderImage[pos] == BLACK && inside == false)		// Entering an already discovered border
			{
				inside = true;
			}
			else if(paddedImage[pos] == BLACK && inside == true)	// Already discovered border point
			{
				continue;
			}
			else if(paddedImage[pos] == WHITE && inside == true)	// Leaving a border
			{
				inside = false;
			}
			else if(paddedImage[pos] == BLACK && inside == false)	// Undiscovered border point
			{
				borderImage[pos] = BLACK; 	// Mark the start pixel
				// convert pos to row and colum
				int r, c;
				convert_pos_to_coordinate(pos,width+2,r,c);
				my_point* point = point_new_1(float(r-1),float(c-1));
				point_list_append(list,point); //Add border point to list
				int checkLocationNr = 1;	// The neighbor number of the location we want to check for a new border point
				int checkPosition;			// The corresponding absolute array address of checkLocationNr
				int newCheckLocationNr; 	// Variable that holds the neighborhood position we want to check if we find a new border at checkLocationNr
				int startPos = pos;			// Set start position

				int counter = 0; 			// Counter is used for the jacobi stop criterion
				int counter2 = 0; 			// Counter2 is used to determine if the point we have discovered is one single point

				// Defines the neighborhood offset position from current position and the neighborhood
				// position we want to check next if we find a new border at checkLocationNr
				int neighborhood[8][2] = {
						{-1,7},
						{-3-width,7},
						{-width-2,1},
						{-1-width,1},
						{1,3},
						{3+width,3},
						{width+2,5},
						{1+width,5}
					};
				// Trace around the neighborhood
				while(true)
				{
					checkPosition = pos + neighborhood[checkLocationNr-1][0];
					newCheckLocationNr = neighborhood[checkLocationNr-1][1];

					if(paddedImage[checkPosition] == BLACK) // Next border point found
					{
						if(checkPosition == startPos)
						{
							counter ++;

							// Stopping criterion (jacob)
							if(newCheckLocationNr == 1 || counter >= 3)
							//if(newCheckLocationNr == 1)
							{
								// Close loop
								inside = true; // Since we are starting the search at were we first started we must set inside to true
								break;
							}
						}

						checkLocationNr = newCheckLocationNr; // Update which neighborhood position we should check next
						pos = checkPosition;
						counter2 = 0; 						// Reset the counter that keeps track of how many neighbors we have visited
						borderImage[checkPosition] = BLACK; // Set the border pixel
						int r1, c1;
						convert_pos_to_coordinate(checkPosition,width+2,r1,c1);
						my_point* point = point_new_1(float(r1-1),float(c1-1));
						point_list_append(list,point); //Add border point to list
					}
					else
					{
						// Rotate clockwise in the neighborhood
						checkLocationNr = 1 + (checkLocationNr % 8);
						if(counter2 > 8)
						{
							// If counter2 is above 8 we have traced around the neighborhood and
							// therefor the border is a single black pixel and we can exit
							counter2 = 0;
							break;
						}
						else
						{
							counter2 ++;
						}
					}
				}
			}
		}
	}

	// Remove white padding and return it
	//pixel * clippedBorderImage = (pixel *)malloc(long(sizeof(pixel) * (height) * (width)));
	pixel * clippedBorderImage = new pixel[sizeof(pixel) * (height) * (width)];
	for(int x = 0; x < width; x ++)
	{
		for(int y = 0; y < height; y ++)
		{
			clippedBorderImage[x+y*width] = borderImage[x+1+(y+1)*(width+2)];
		}
	}
	// free memory of paddedImage and borderImage
//	free(paddedImage);
//	free(borderImage);
	delete paddedImage;
	delete borderImage;
	return clippedBorderImage;
}
//--------------------end implementation---------------------

#endif /* CONTOUR_TRACING_H_ */

