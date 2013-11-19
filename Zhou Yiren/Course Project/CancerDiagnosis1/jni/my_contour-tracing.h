/*
 * my_contour-tracing.h
 *
 *  Created on: Nov 20, 2013
 *      Author: thanhtoan_do
 */

#ifndef MY_CONTOUR_TRACING_H_
#define MY_CONTOUR_TRACING_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "my_point.h"
#include "my_image.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
#define WHITE 0.0
#define BLACK 255.0

void create_matrix_from_list1(point_list* list, Mat& tmp, float value)
{
	for(int i=0;i<list->size;i++)
	{
		int r = int(list->data[i]->r);
		int c = int(list->data[i]->c);
		tmp.at<uchar>(r,c) = value;
	}
}
void my_mooreNeighborTracing(my_image* img, point_list* list);
void my_mooreNeighborTracing_1(my_image* img, vector<my_point> &list);

my_image* padding(my_image* img)
{
	int H = img->height;
	int W = img->width;
	int Hnew = H + 2;
	int Wnew = W + 2;
	my_image* padded_img = image_new(Wnew,Hnew);

	for(int y  = 0; y < Hnew; y++)
		for(int x = 0; x < Wnew; x++)
		{
			if(y==0 || y== Hnew-1 || x==0 || x== Wnew-1)
			{
				padded_img->data[y][x] = WHITE;
			}
			else
			{
				padded_img->data[y][x] = img->data[y-1][x-1];
			}
		}
	return padded_img;
}
void find_start(my_image* img, int &r, int &c)
{
	bool flag = false;
	for(int x=0; x<img->width&&flag==false;x++)//tu trai sang phai
	{
		for(int y=0; y<img->height;y++)//tu tren xuong duoi, co the tu duoi len tren neu can
		{
			if(img->data[y][x] == BLACK)
			{
				r = y;
				c = x;
				flag = true;
				break;
			}
		}
	}
}
void addlist(point_list* list, float r, float c)
{
	my_point* point = point_new_1(float(r),float(c));
	point_list_append(list,point);
}

void my_mooreNeighborTracing(my_image* img, point_list* list)
{
//	int Hnew = img->height+2;
//	int Wnew = img->width+2;
	my_image* padded_img = padding(img);
	//define neighbor circle
    // p0 p1 p2
	// p7 p  p3
	// p6 p5 p4
	int fy[8] = {-1,-1,-1,0,1,1, 1, 0};
	int fx[8] = {-1, 0,	1,1,1,0,-1,-1};
	int rstart;
	int cstart; //start coordinate
	find_start(padded_img, rstart, cstart);
	//add start point to list
	addlist(list, float(rstart-1), float(cstart-1));
//	my_point* point = point_new_1(float(rstart-1),float(cstart-1));
//	point_list_append(list,point); //Add border point to list
	int pr = rstart, pc = cstart; //toa do diem p
	int pre_pr = pr-1, pre_pc = pc; //khi chuyen cach duyet thi cung chuyen pre
	//danh dau diem tien vao cua start, vi duyet theo cot tu tren xuong nen cot nhu nhau
	//hang tu tren xuong nen hang-1
	int tienvao_start = 1;
	bool jacob = false; //chua gap dieu kien Jacob
	bool flag = true; //neu co lan can nao = BLACK thi true; nguoc lai la false
	int count = 0; //so lan gap lai start
	while(jacob == false && flag == true && count<1)
	{
//		cout<<"size: "<<list->size<<endl;
		if(list->size == 1000)
		{
//				Mat border(img->height,img->width,CV_8UC1);
//				border = Scalar::all(0);
//				create_matrix_from_list1(list,border,255);
//				namedWindow("border", CV_WINDOW_AUTOSIZE);
//				imshow("border", border);
//				waitKey(0);
//				border.release();
//				break;
		}
		// kiem tra pre la lan can thu may cua p de biet duong xoay theo kim dong ho
		int idx = -1;
		for(int i=0; i<8; i++)
		{
			if (pre_pr == fy[i] + pr && pre_pc == fx[i] + pc)
			{
				idx = i;
				break;
			}
		}
		//bat dau tu lan can idx, duyet vong quanh p theo chieu kim dong ho
		flag = false;

		for(int i=idx+1; i<8&&flag==false;i++)
		{
			if(padded_img->data[pr+fy[i]][pc+fx[i]] == BLACK)
			{
				if(pr+fy[i] == rstart && pc+fx[i] == cstart)
					count++;
				flag = true; //tim duoc diem BLACK
				if(pr+fy[i] == rstart && pc+fx[i] == cstart && i==tienvao_start)
				{	//thoa dieu kien jacob
					jacob = true;
				}
				else
				{
					//giu lai gia tri p
					float pr_tmp = pr;
					float pc_tmp = pc;
					pr = pr+fy[i];
					pc = pc+fx[i];
					addlist(list,float(pr-1),float(pc-1));
					//danh dau lai pre
					if(i>0)
					{
						pre_pr = pr_tmp+fy[i-1];
						pre_pc = pc_tmp+fx[i-1];
					}
					else
					{
						pre_pr = pr_tmp+fy[7];
						pre_pc = pc_tmp+fx[7];
					}
				}
			}
		}
		if (flag == false)//neu chua tim duoc diem BLACK, duyet tiep tuc tu 0 den idx
		{
			for(int i = 0; i<idx&&flag==false; i++)
			{
				if(padded_img->data[pr+fy[i]][pc+fx[i]] == BLACK)
				{
					if(pr+fy[i] == rstart && pc+fx[i] == cstart)
						count++;
					flag = true; //tim duoc diem BLACK
					if(pr+fy[i] == rstart && pc+fx[i] == cstart && i==tienvao_start)
					{	////thoa dieu kien jacob
						jacob = true;
					}
					else
					{
						float pr_tmp = pr;
						float pc_tmp = pc;
						pr = pr+fy[i];
						pc = pc+fx[i];
						addlist(list,float(pr-1),float(pc-1));
						//danh dau lai pre
						if(i>0)
						{
							pre_pr = pr_tmp+fy[i-1];
							pre_pc = pc_tmp+fx[i-1];
						}
						else
						{
							pre_pr = pr_tmp+fy[7];
							pre_pc = pc_tmp+fx[7];
						}
					}
				}
			}
		}
	}

	{
//		Mat border1(img->height,img->width,CV_8UC1);
//		border1 = Scalar::all(0);
//		create_matrix_from_list1(list,border1,255);
//		namedWindow("border", CV_WINDOW_AUTOSIZE);
//		imshow("border", border1);
//		waitKey(0);
//		border1.release();
	}
	image_delete(padded_img);
}

void my_mooreNeighborTracing_1(my_image* img, vector<my_point> &list)
{
//	int Hnew = img->height+2;
//	int Wnew = img->width+2;
	my_image* padded_img = padding(img);
	//define neighbor circle
    // p0 p1 p2
	// p7 p  p3
	// p6 p5 p4
	int fy[8] = {-1,-1,-1,0,1,1, 1, 0};
	int fx[8] = {-1, 0,	1,1,1,0,-1,-1};
	int rstart;
	int cstart; //start coordinate
	find_start(padded_img, rstart, cstart);
	//add start point to list
	my_point point;
	point.r = float(rstart-1);
	point.c = float(cstart-1);
	list.push_back(point);
//	addlist(list, float(rstart-1), float(cstart-1));
//	my_point* point = point_new_1(float(rstart-1),float(cstart-1));
//	point_list_append(list,point); //Add border point to list
	int pr = rstart, pc = cstart; //toa do diem p
	int pre_pr = pr-1, pre_pc = pc; //khi chuyen cach duyet thi cung chuyen pre
	//danh dau diem tien vao cua start, vi duyet theo cot tu tren xuong nen cot nhu nhau
	//hang tu tren xuong nen hang-1
	int tienvao_start = 1;
	bool jacob = false; //chua gap dieu kien Jacob
	bool flag = true; //neu co lan can nao = BLACK thi true; nguoc lai la false
	int count = 0; //so lan gap lai start
	while(jacob == false && flag == true && count<1)
	{
//		cout<<"size: "<<list->size<<endl;
		if(list.size() == 1000)
		{
//				Mat border(img->height,img->width,CV_8UC1);
//				border = Scalar::all(0);
//				create_matrix_from_list1(list,border,255);
//				namedWindow("border", CV_WINDOW_AUTOSIZE);
//				imshow("border", border);
//				waitKey(0);
//				border.release();
//				break;
		}
		// kiem tra pre la lan can thu may cua p de biet duong xoay theo kim dong ho
		int idx = -1;
		for(int i=0; i<8; i++)
		{
			if (pre_pr == fy[i] + pr && pre_pc == fx[i] + pc)
			{
				idx = i;
				break;
			}
		}
		//bat dau tu lan can idx, duyet vong quanh p theo chieu kim dong ho
		flag = false;

		for(int i=idx+1; i<8&&flag==false;i++)
		{
			if(padded_img->data[pr+fy[i]][pc+fx[i]] == BLACK)
			{
				if(pr+fy[i] == rstart && pc+fx[i] == cstart)
					count++;
				flag = true; //tim duoc diem BLACK
				if(pr+fy[i] == rstart && pc+fx[i] == cstart && i==tienvao_start)
				{	//thoa dieu kien jacob
					jacob = true;
				}
				else
				{
					//giu lai gia tri p
					float pr_tmp = pr;
					float pc_tmp = pc;
					pr = pr+fy[i];
					pc = pc+fx[i];
					my_point point1;
					point1.r = float(pr-1);
					point1.c = float(pc-1);
					list.push_back(point1);
					//danh dau lai pre
					if(i>0)
					{
						pre_pr = pr_tmp+fy[i-1];
						pre_pc = pc_tmp+fx[i-1];
					}
					else
					{
						pre_pr = pr_tmp+fy[7];
						pre_pc = pc_tmp+fx[7];
					}
				}
			}
		}
		if (flag == false)//neu chua tim duoc diem BLACK, duyet tiep tuc tu 0 den idx
		{
			for(int i = 0; i<idx&&flag==false; i++)
			{
				if(padded_img->data[pr+fy[i]][pc+fx[i]] == BLACK)
				{
					if(pr+fy[i] == rstart && pc+fx[i] == cstart)
						count++;
					flag = true; //tim duoc diem BLACK
					if(pr+fy[i] == rstart && pc+fx[i] == cstart && i==tienvao_start)
					{	////thoa dieu kien jacob
						jacob = true;
					}
					else
					{
						float pr_tmp = pr;
						float pc_tmp = pc;
						pr = pr+fy[i];
						pc = pc+fx[i];
						my_point point2;
						point2.r = float(pr-1);
						point2.c = float(pc-1);
						list.push_back(point2);
						//danh dau lai pre
						if(i>0)
						{
							pre_pr = pr_tmp+fy[i-1];
							pre_pc = pc_tmp+fx[i-1];
						}
						else
						{
							pre_pr = pr_tmp+fy[7];
							pre_pc = pc_tmp+fx[7];
						}
					}
				}
			}
		}
	}

	{
//		Mat border1(img->height,img->width,CV_8UC1);
//		border1 = Scalar::all(0);
//		create_matrix_from_list1(list,border1,255);
//		namedWindow("border", CV_WINDOW_AUTOSIZE);
//		imshow("border", border1);
//		waitKey(0);
//		border1.release();
	}
	image_delete(padded_img);
}
#endif /* MY_CONTOUR_TRACING_H_ */
