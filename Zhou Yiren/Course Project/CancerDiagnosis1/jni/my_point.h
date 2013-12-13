/*
 * my_point.h
 *
 *  Created on: Nov 19, 2013
 *      Author: thanhtoan_do
 */

#ifndef MY_POINT_H_
#define MY_POINT_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	float r;
	float c;
} my_point;
typedef struct
{
	int size;
	my_point** data;
} point_list;

 my_point* point_new();
 my_point* point_new_1(float r, float c);
 void point_delete(my_point* point);
 point_list* point_list_new();
 point_list* point_list_new_1(int size);
 void point_list_append(point_list *list, my_point *point);
 void point_list_delete(point_list *list);
 void clear_list(point_list* list);

void testmalloc();

 void testmalloc()
 {
	 char* s = (char*)malloc(sizeof(char)*100);
 }
//------------------implementation----------------------------------
my_point* point_new()
{
	my_point* point = new my_point;
	point->c = -1;
	point->r = -1;
	return point;
}
my_point* point_new_1(float r, float c)
{
	my_point* point = new my_point;
	point->r = r;
	point->c = c;
	return point;
}
void point_delete(my_point* point)
{
	delete point;
	point = NULL;
}
point_list *point_list_new(void)
{
	point_list *list=new point_list;
	list->size=0;
	list->data=NULL;
	return list;
}
point_list* point_list_new_1(int size)
{
	point_list* list = new point_list;
	list->size = size;
	list->data = new my_point*[size];
	for(int i = 0; i<size; i++)
		list->data[i] = point_new();
	return list;
}
void point_list_append(point_list *list, my_point *point)
{
//	int tmp = (sizeof(*list->data))*(list->size + 1);
//	list->data=(my_point**)realloc(list->data,tmp);
//	list->data[list->size++]=point;
}
void point_list_delete(point_list* list)
{
	for(int i = 0; i<list->size; i++)
		point_delete (list->data[i]);

	delete list->data;
	delete list;
}
void clear_list(point_list* list)
{
	for(int i = 0; i<list->size; i++)
			point_delete (list->data[i]);
	delete list->data;
	list->data = NULL;
	list->size = 0;
}

/*
typedef struct
{
	float r;
	float c;
} my_point;
typedef struct
{
	int size;
	my_point** data;
} point_list;

 my_point* point_new();
 my_point* point_new_1(float r, float c);
 void point_delete(my_point* point);
 point_list* point_list_new();
 point_list* point_list_new_1(int size);
 void point_list_append(point_list *list, my_point *point);
 void point_list_delete(point_list *list);

//------------------implementation----------------------------------
my_point* point_new()
{
	my_point* point = new my_point;
	point->c = -1;
	point->r = -1;
	return point;
}
my_point* point_new_1(float r, float c)
{
	my_point* point = new my_point;
	point->r = r;
	point->c = c;
	return point;
}
void point_delete(my_point* point)
{
	delete point;
	point = NULL;
}
point_list *point_list_new(void)
{
	point_list *list=new point_list;
	list->size=0;
	list->data=NULL;
	return list;
}
point_list* point_list_new_1(int size)
{
	point_list* list = new point_list;
	list->size = size;
	list->data = new my_point*[size];
	for(int i = 0; i<size; i++)
		list->data[i] = point_new();
	return list;
}
void point_list_append(point_list *list, my_point *point)
{
//	int tmp = (sizeof(*list->data))*(list->size + 1);
//	list->data=(my_point**)realloc((void*)(list->data),tmp);
//	list->data[list->size++]=point;
	point_list* list1 = point_list_new_1(list->size+1);
	for(int i=0; i<list->size; i++)
	{
		list1->data[i]->c = list->data[i]->c;
		list1->data[i]->r = list->data[i]->r;
	}
	list1->data[list1->size-1]->c = point->c;
	list1->data[list1->size-1]->r = point->r;
	point_list_delete(list);
	list = list1;
	point_delete(point);
}
void point_list_delete(point_list* list)
{
	for(int i = 0; i<list->size; i++)
		point_delete (list->data[i]);
	delete list->data;
	delete list;
}
*/
//---------------------end implementation---------------------------
#endif /* MY_POINT_H_ */
