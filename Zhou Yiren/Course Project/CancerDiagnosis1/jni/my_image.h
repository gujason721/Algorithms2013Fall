/*
 * my_image.h
 *
 *  Created on: Nov 19, 2013
 *      Author: thanhtoan_do
 */
#ifndef MY_IMAGE_H_
#define MY_IMAGE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int height,width;
	float **data;
} my_image;

typedef struct
{
	int height,width;
	float **r;		// Red
	float **g;		// Green
	float **b;		// Blue
} my_color_image;

typedef struct
{
	int size;		// Number of images
	my_image **data;	// List of images
} image_list;

my_image *image_new(int width, int height);
void image_delete(my_image *image);
my_color_image *color_image_new(int width, int height);
void color_image_delete(my_color_image *image);
image_list *image_list_new();
void image_list_append(image_list *list, my_image *image);
void image_list_delete(image_list *list);
my_image* get_one_chanel_of_color_image(my_color_image *image, int chanel);

//////--------------------- implementation------------------
my_image *image_new(int width, int height)
{
	my_image *im= new my_image;
	im->width=width;
	im->height=height;
	im->data = new float* [height];
	for (int i = 0; i < height; i ++)
		im->data[i] = new float[width];
	return im;
}

void image_delete(my_image *im)
{
	for (int i = 0; i < im->height; i ++)
		delete im->data[i];
	delete(im->data);
	delete(im);
}

my_color_image *color_image_new(int width, int height)
 {
	my_color_image *im= new my_color_image;
	im->width=width;
	im->height=height;
	im->r = new float*[height];
	im->g = new float*[height];
	im->b = new float*[height];
	for (int i = 0; i < height; i++)
	{
		im->r[i] = new float[width];
		im->g[i] = new float[width];
		im->b[i] = new float[width];
	}
	return im;
}

void color_image_delete(my_color_image *im)
{
	for (int i = 0; i<im->height; i++)
	{
		delete(im->r[i]);
		delete(im->g[i]);
		delete(im->b[i]);
	}
	delete im->r;
	delete im->g;
	delete im->b;
	delete(im);
}

image_list *image_list_new(void)
{
	image_list *list=new image_list;
	list->size=0;
	list->data=NULL;
	return list;
}

void image_list_append(image_list *list, my_image *image)
{
//	long tmp = (sizeof(*list->data))*(list->size + 1);
//	list->data=(my_image**)realloc(list->data,tmp);
//	list->data[list->size++]=image;
}

void image_list_delete(image_list *list)
{
	int i;
	for(i=0;i<list->size;i++)
		image_delete(list->data[i]);
	delete(list->data);
	delete(list);
}
my_image* get_one_chanel_of_color_image(my_color_image *image, int chanel)
{
	my_image* img = image_new(image->width,image->height);
	for(int y=0; y<image->height; y++)
		for(int x=0; x<image->width; x++)
		{
			switch (chanel) {
			case 1:
				img->data[y][x] = image->r[y][x];
				break;
			case 2:
				img->data[y][x] = image->g[y][x];
				break;
			case 3:
				img->data[y][x] = image->b[y][x];
				break;
			}
		}
	return img;
}
/*
using namespace std;
typedef struct
{
	int height,width;
	float **data;
} my_image;

typedef struct
{
	int height,width;
	float **r;		// Red
	float **g;		// Green
	float **b;		// Blue
} my_color_image;

typedef struct
{
	int size;		// Number of images
	my_image **data;	// List of images
} image_list;

my_image *image_new(int width, int height);
void image_delete(my_image *image);
my_color_image *color_image_new(int width, int height);
void color_image_delete(my_color_image *image);
image_list *image_list_new();
void image_list_append(image_list *list, my_image *image);
void image_list_delete(image_list *list);
my_image* get_one_chanel_of_color_image(my_color_image *image, int chanel);

//////--------------------- implementation------------------
my_image *image_new(int width, int height)
{
	my_image *im= new my_image;
	im->width=width;
	im->height=height;
	im->data = new float* [height];
	for (int i = 0; i < height; i ++)
		im->data[i] = new float[width];
	return im;
}

void image_delete(my_image *im)
{
	for (int i = 0; i < im->height; i ++)
		delete im->data[i];
	delete(im->data);
	delete(im);
}

my_color_image *color_image_new(int width, int height)
 {
	my_color_image *im= new my_color_image;
	im->width=width;
	im->height=height;
	im->r = new float*[height];
	im->g = new float*[height];
	im->b = new float*[height];
	for (int i = 0; i < height; i++)
	{
		im->r[i] = new float[width];
		im->g[i] = new float[width];
		im->b[i] = new float[width];
	}
	return im;
}

void color_image_delete(my_color_image *im)
{
	for (int i = 0; i<im->height; i++)
	{
		delete(im->r[i]);
		delete(im->g[i]);
		delete(im->b[i]);
	}
	delete im->r;
	delete im->g;
	delete im->b;
	delete(im);
}

image_list *image_list_new(void)
{
	image_list *list=new image_list;
	list->size=0;
	list->data=NULL;
	return list;
}

void image_list_append(image_list *list, my_image *image)
{
//	long tmp = (sizeof(*list->data))*(list->size + 1);
//	list->data=(my_image**)realloc(list->data,tmp);
//	list->data[list->size++]=image;
}

void image_list_delete(image_list *list)
{
	int i;
	for(i=0;i<list->size;i++)
		image_delete(list->data[i]);
	delete(list->data);
	delete(list);
}
my_image* get_one_chanel_of_color_image(my_color_image *image, int chanel)
{
	my_image* img = image_new(image->width,image->height);
	for(int y=0; y<image->height; y++)
		for(int x=0; x<image->width; x++)
		{
			switch (chanel) {
			case 1:
				img->data[y][x] = image->r[y][x];
				break;
			case 2:
				img->data[y][x] = image->g[y][x];
				break;
			case 3:
				img->data[y][x] = image->b[y][x];
				break;
			}
		}
	return img;
}*/

////------------------end implementation--------------------------
#endif /* MY_IMAGE_H_ */
