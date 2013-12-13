/*
 * util1.h
 *
 *  Created on: Nov 25, 2013
 *      Author: thanhtoan_do
 */

#ifndef UTIL1_H_
#define UTIL1_H_

#include <opencv2/opencv.hpp>
#include <iostream>

#include "my_image.h"
#include "my_point.h"
#include "contour-tracing.h"
#include "my_contour-tracing.h"

using namespace cv;
using namespace std;


Mat convert_argb_to_rgba(Mat Argb)
{
	Mat Rgba(Argb.rows,Argb.cols,CV_8UC4);
	for (int y=0; y<Argb.rows; y++)
		for(int x = 0; x<Argb.cols; x++)
		{
			long tmp = Argb.at<long>(y,x);
			Rgba.at<long>(y,x) = ((tmp & 0x00FFFFFF) << 8) | ((tmp & 0xFF000000) >> 24);
		}
	return Rgba;
}

my_color_image* get_rgb_image_from_rgb(Mat mRgb)
{
	int H = mRgb.rows;
	int W = mRgb.cols;
	my_color_image* img_color = color_image_new(W,H);
    for (int y = 0; y < H; y++)
    {
    	for (int x = 0; x < W; x++)
        {
    		Vec3b intensity = mRgb.at<Vec3b>(y, x);
    		uchar R = intensity.val[0];
    		uchar G = intensity.val[1];
    		uchar B = intensity.val[2];
            img_color->r[y][x] = float(R);
	        img_color->g[y][x] = float(G);
	        img_color->b[y][x] = float(B);
        }
   }
    return img_color;
}
my_image* get_binary_image_from_binary(Mat mBinary)
{
		int H = mBinary.rows;
		int W = mBinary.cols;
		my_image* Ib = image_new(W,H);
	    for (int y = 0; y < H; y++)
	    {
	    	for (int x = 0; x < W; x++)
	        {
	    		float intensity = float(mBinary.at<uchar>(y, x));
	    		float tmp = intensity;
	    		if (tmp>=128){
	    			Ib->data[y][x] = 255.0;
	    		}
	            else{
	            	Ib->data[y][x] = 0.0;
	            }
	        }
	   }
	   return Ib;
}

void get_bound_rect_matrix(Mat Ib, int &tlrow, int &tlcol, int &brrow, int &brcol)
{
	tlrow = -1; tlcol = -1; brrow = -1; brcol = -1;
	bool flag = false;
	int y, x;
	for (y=0; y<Ib.rows&&flag==false;y++)
	{
		for(x=0;x<Ib.cols;x++)
		{
			uchar tmp = Ib.at<uchar>(y,x);
			if (tmp == 255)
			{
				tlrow = y;
				flag = true;
				break;
			}
		}
	}

	flag = false;
	for(y = Ib.rows-1; y>=0&&flag==false; y--)
	{
		for(x=0; x<Ib.cols;x++)
		{
			uchar tmp = Ib.at<uchar>(y,x);
			if (tmp == 255)
			{
				brrow = y;
				flag = true;
				break;
			}
		}
	}

	flag = false;
	for(x=0; x<Ib.cols&&flag == false; x++)
	{
		for(y=0; y<Ib.rows;y++)
		{
			uchar tmp = Ib.at<uchar>(y,x);
			if(tmp == 255)
			{
				tlcol = x;
				flag = true;
				break;
			}
		}
	}

	flag = false;
	for(x=Ib.cols-1; x>=0&&flag==false; x--)
		{
			for(y=0; y<Ib.rows;y++)
			{
				uchar tmp = Ib.at<uchar>(y,x);
				if(tmp == 255)
				{
					brcol = x;
					flag = true;
					break;
				}
			}
		}
}

// to mau red cho minimm boundary rectangle
void draw_rectangle(Mat mbinary, Mat &mskin)
{
	int tlrow = -1, tlcol = -1, brrow = -1, brcol = -1;
	get_bound_rect_matrix(mbinary,tlrow,tlcol,brrow,brcol);
	//hang tren
	for(int x = tlcol; x<brcol; x++)
	{
		mskin.at<Vec3b>(tlrow,x).val[0] = 0;
		mskin.at<Vec3b>(tlrow,x).val[1] = 0;
		mskin.at<Vec3b>(tlrow,x).val[2] = 255;
	}
	//hang duoi
	for(int x = tlcol; x<brcol; x++)
	{
		mskin.at<Vec3b>(brrow,x).val[0] = 0;
		mskin.at<Vec3b>(brrow,x).val[1] = 0;
		mskin.at<Vec3b>(brrow,x).val[2] = 255;
	}
	//cot trai
	for(int y = tlrow; y<brrow; y++)
	{
		mskin.at<Vec3b>(y,tlcol).val[0] = 0;
		mskin.at<Vec3b>(y,tlcol).val[1] = 0;
		mskin.at<Vec3b>(y,tlcol).val[2] = 255;
	}
	//cot phai
	for(int y = tlrow; y<brrow; y++)
	{
		mskin.at<Vec3b>(y,brcol).val[0] = 0;
		mskin.at<Vec3b>(y,brcol).val[1] = 0;
		mskin.at<Vec3b>(y,brcol).val[2] = 255;
	}
}
float moment(my_image* im, int p, int q)
{
	float kq = 0;
	for (int x = 0; x<im->height; x++)
		for(int y = 0; y<im->width; y++)
		{
			float tmp = im->data[x][y];
//			if (tmp == 255)
//				tmp = 1;
			kq = kq + tmp*pow(float(x),p)*pow(float(y),q);
		}
	return kq;
}
float central_moment(my_image* Ib_min, float r0, float c0, int p, int q)
{
	float kq = 0;
	int h = Ib_min->height;
	int w = Ib_min->width;
	for (int x = 0; x<h; x++)
		for(int y = 0; y< w; y++)
		{
			float tmp = Ib_min->data[x][y];
//			if (tmp == 255)
//				tmp = 1;
			kq = kq+tmp*pow(x-r0,p)*pow(y-c0,q);
		}
	return kq;
}
/*
 * goal: find centroid of mass of image im
 * output: centroid
 * */
my_point* find_centroid(my_image* im)
{
	float m00 = moment(im,0,0);
	float m10 = moment(im,1,0);
	float m01 = moment(im,0,1);
	float r0 = m10/m00;
	float c0 = m01/m00;
	my_point* centroid = point_new_1(r0,c0);
	return centroid;
}
//////////------------------Color----------------------////////
pixel* convert_2D_to_1D(my_image* Ib_min)
{
	int H = Ib_min->height;
	int W = Ib_min->width;
	//pixel * img = (pixel *)malloc(long(sizeof(pixel) * H * W));
	pixel* img = new pixel[sizeof(pixel) * H * W];
	for(int y = 0; y<H; y++)
		for(int x = 0; x<W; x++)
		{
			pixel tmp;
			tmp.g = int(Ib_min->data[y][x]);
			img[x+y*W] = tmp;
		}
	return img;
}
void find_boundary_pixels(my_image* Ib_min, vector<my_point> &list)
{
	my_mooreNeighborTracing_1(Ib_min, list);
}
void create_matrix_from_list(vector<my_point> list, Mat& tmp, float value)
{
	for(int i=0;i<list.size();i++)
	{
		int r = int(list[i].r);
		int c = int(list[i].c);
		tmp.at<uchar>(r,c) = value;
	}
}
void check_border_continous(vector<my_point> list)
{
	int count = 0;
	for(int i=0; i<list.size(); i++)
	{
		my_point point1 = list[i];
		my_point point2 = list[i+1];
		int r1 = int(point1.r); int c1 = int(point1.c);
		int r2 = int(point2.r); int c2 = int(point2.c);
		if(abs(r1-r2) > 1 || abs(c1-c2) > 1)
		{
//			printf("pixel %d va %d khong lien tuc\n",i,i+1);
			cout <<"pixel khong lien tuc " << i <<" " << i+1 <<endl;
			count++;
		}
	}
//	printf("so pixel khong lien tuc = %d\n",count);
	cout<< "so pixel khong lien tuc "<<count<<endl;
}
/*
point_list* list_shift(point_list* list, my_image* img)
{
	//tim diem bat dau
	bool flag = false;
	int r,c;
	for(int x=0; x<img->width&&flag==false;x++)//tu trai sang phai
	{
		for(int y=0; y<img->height;y++)//tu tren xuong duoi
		{
			if(img->data[y][x] == 255)
			{
				r = y;
				c = x;
				flag = true;
				break;
			}
		}
	}
	int id = 0;
	//neu duyet het tu dau den cuoi ma ko gap duoc start thi idx van = 0--->list1 = list
	for(int i=0; i<list->size;i++)
	{
		int rtmp = int(list->data[i]->r);
		int ctmp = int(list->data[i]->c);
		if(rtmp == r && ctmp == c)
		{
			id = i; break;
		}
	}
//	id = 0;
	if(id>0)
		cout<<"shift duoc list" <<endl;
	point_list* listtmp = point_list_new();
	for(int i=id; i<list->size;i++)
	{
		my_point* point = point_new_1(list->data[i]->r,list->data[i]->c);
		point_list_append(listtmp, point);
	}
	for(int i=0; i<id; i++)
	{
		my_point* point = point_new_1(list->data[i]->r,list->data[i]->c);
		point_list_append(listtmp, point);
	}
	return listtmp;
}
*/
//ham inpolygon nay chi can truyen list vao, ko can return vi minh chi dung 1 list de chay suot chuong trinh
void inpolygon(vector<Point2f> contour, my_image* Ib_min, vector<my_point> &list)
{

	for(int y=0; y<Ib_min->height;y++)
		for(int x = 0; x<Ib_min->width; x++)
		{
			int tmp = pointPolygonTest( contour, Point2f(x,y), false );
			if(tmp>=0)
			{
				my_point point;
				point.r = y;
				point.c = x;
				list.push_back(point);
			}
		}
}
float mean_gray(vector<my_point> list, my_image* Igray_min)
{
	float res = -1;
	for(int i=0; i< list.size(); i++)
	{
		int r = int(list[i].r);
		int c = int(list[i].c);
		res += Igray_min->data[r][c];
	}
	res = res/list.size();
	return res;
}
float intra_triangle_distance(float** gr, int checkempty[], int numtriangle, int k)
{
	float maxdis = -1;
	for(int i = 0; i< numtriangle-1; i++)
	{
		if(checkempty[i] == 1)
		{
			for(int j=i+1; j < numtriangle; j++)
			{
				if(checkempty[j] == 1)
				{
					float distance = 0;
					for(int t = 0; t<k; t++)
					{
						distance += (gr[i][t]-gr[j][t])*(gr[i][t]-gr[j][t]);
					}
					distance = sqrt(distance);
					if(distance > maxdis)
					{
						maxdis = distance;
					}
				}
			}
		}
	}
	return maxdis;
}

float triangle_color_feature(my_image* Ib_min, my_image* Igray_min)
{
	float res = 0.0;
//	int H = Ib_min->height;
//	int W = Ib_min->width;
	my_point* centroid = find_centroid(Ib_min);
//	cout<< "centroid:" << centroid->r << " " <<centroid->c <<endl;
//	centroid matlab: r 105, c 130. centroid here: r 103, c 129 -->OK

	vector<my_point> list1;
	find_boundary_pixels(Ib_min,list1);
//	point_list* list1 = list_shift(list,Ib_min);
//	cout <<"number border pixels:" << list1.size() <<endl;
	if (list1.size() > 0)
	{
//		cout <<"pixel 0 hang cot:"<<" "<<list1->data[0]->r <<" "<<list1->data[0]->c<<endl;
//		cout <<"pixel 1 hang cot:"<<" "<<list1->data[1]->r <<" "<<list1->data[1]->c<<endl;
//		cout <<"pixel 2 hang cot:"<<" "<<list1->data[2]->r <<" "<<list1->data[2]->c<<endl;
//		cout <<"pixel 3 hang cot:"<<" "<<list1->data[3]->r <<" "<<list1->data[3]->c<<endl;
	}

//	check_border_continous(list1);
//	Mat border(Ib_min->height,Ib_min->width,CV_8UC1);
//	border = Scalar::all(0);
//	create_matrix_from_list(list1,border,255);
//	namedWindow("border", CV_WINDOW_AUTOSIZE);
//	imshow("border", border);
//	waitKey(0);
//	border.release();

	int numtriangle = 16;
	int tt = list1.size();
	int step = tt/numtriangle;
	if (step<1)
	{
		step = 1;
	}
//	% chia moi hinh tam giac thanh k phan
	int k = 4;
//	float gr[16][4]; //tuong ung 16 part, moi part 4 triangle
	float **gr;
	gr = new float*[numtriangle];
	for(int i = 0; i < numtriangle; i++)
		gr[i] = new float[k];
	for(int i = 0; i< numtriangle; i++)
		for(int j=0; j<k; j++)
			gr[i][j] = -1;

	int* checkempty = new int[numtriangle];

	for(int i=0;i<numtriangle;i++)
	{
		checkempty[i] = 1; //luc dau gan tat ca cac triangle deu OK
	}
	float xM1, yM1, xN1, yN1, xM, yM, xN, yN;
	vector<my_point> list_inpoly;
	vector<Point2f> contour;
	int count = -1;
	float mean_value;

	Mat tri(Ib_min->height,Ib_min->width,CV_8UC1);//matrix de visual
	tri = Scalar::all(0);
	for (int i = 0; i<= tt-step; i = i+step)
	{
//		 xv = Q(i:i+step,2); %column
//		 yv = Q(i:i+step,1); %row
		count ++;
//		cout<<"count = "<<count<<" "<<"i = "<<i<<endl;
//		cout<<list1->data[i]->c<<" "<<centroid->c<<endl;
//		cout<<list1[i].r<<" "<<centroid->r<<endl;
		xM1 = (1.0/k)*(my_point(list1[i]).c - centroid->c) + centroid->c;
		yM1 = (1.0/k)*(my_point(list1[i]).r - centroid->r) + centroid->r;
		xN1 = (1.0/k)*(my_point(list1[i+step-1]).c - centroid->c) + centroid->c;
		yN1 = (1.0/k)*(my_point(list1[i+step-1]).r - centroid->r) + centroid->r;

		list_inpoly.clear();
		contour.clear();
		contour.push_back(Point2f(centroid->c,centroid->r));
		contour.push_back(Point2f(xM1,yM1));
		contour.push_back(Point2f(xN1,yN1));
		inpolygon(contour, Ib_min, list_inpoly);

		if (list_inpoly.size() > 0)// neu khac rong thi tinh gray value va moi lam tiep, ko thi dung, chuyen sang tam giac khac
		{
			mean_value = mean_gray(list_inpoly,Igray_min);
			gr[count][0] = mean_value;

			create_matrix_from_list(list_inpoly,tri,(count+1)*15);

			bool flag = true;
			for(int j = 2; j<=k && flag; j++)
			{

				list_inpoly.clear();
				contour.clear();

				//toa do diem truoc
				my_point pointtruoc = list1[i];
				my_point pointsau = list1[i+step-1];
				xM1 = 1.0*(j-1)/k*(pointtruoc.c - centroid->c) + centroid->c;
				yM1 = 1.0*(j-1)/k*(pointtruoc.r - centroid->r) + centroid->r;
				xN1 = 1.0*(j-1)/k*(pointsau.c - centroid->c) + centroid->c;
				yN1 = 1.0*(j-1)/k*(pointsau.r - centroid->r) + centroid->r;

				//toa do cua diem:
				xM = 1.0*j/k*(pointtruoc.c - centroid->c) + centroid->c;
				yM = 1.0*j/k*(pointtruoc.r - centroid->r) + centroid->r;
				xN = 1.0*j/k*(pointsau.c - centroid->c) + centroid->c;
				yN = 1.0*j/k*(pointsau.r - centroid->r) + centroid->r;
				if(j<k)//if(j<k-1)
				{
					contour.push_back(Point2f(xM1,yM1));
					contour.push_back(Point2f(xM,yM));
					contour.push_back(Point2f(xN,yN));
					contour.push_back(Point2f(xN1,yN1));
					inpolygon(contour, Ib_min, list_inpoly);
//					if (count == 0)
					{
						create_matrix_from_list(list_inpoly,tri,(count+1)*15);
					}
				}
				else
				{
					//phan cuoi cung bao gom toa do cua diem k-1 va cac diem tren bien
					contour.push_back(Point2f(xM1,yM1));
					for(int xx = i; xx<i+step; xx++)
					{
						my_point pointtmp = list1[xx];
						float xtmp = (pointtmp.c);
						float ytmp = (pointtmp.r);
						contour.push_back(Point2f(xtmp,ytmp));
					}
					contour.push_back(Point2f(xN1,yN1));
					inpolygon(contour, Ib_min, list_inpoly);
//					if (count == 0)
					{
						create_matrix_from_list(list_inpoly,tri,(count+1)*15);
					}

				}
				if (list_inpoly.size() > 0)
				{
					mean_value = mean_gray(list_inpoly,Igray_min);
					gr[count][j-1] = mean_value;
				}
				else//chi can co bat ki mot subpart nao empty --> danh dau triangle = -1;
				{
					flag = false;
					checkempty[count] = -1;
				}
			}
		}
		else
		{
			checkempty[count] = -1;
		}
	}

//	namedWindow("tri", CV_WINDOW_AUTOSIZE);
//	imshow("tri", tri);
//	waitKey(0);
//	tri.release();

	res = intra_triangle_distance(gr, checkempty,  numtriangle,  k);

	list_inpoly.clear();
	contour.clear();
	list1.clear();
//	for(int i = 0; i< numtriangle; i++)
//	{
//		for(int j=0; j<k; j++)
//		{
//			cout<< gr[i][j] << " ";
//		}
//		cout<<endl;
//	}
	for(int i=0; i< numtriangle; i++)
		delete gr[i];
	delete gr;
	delete checkempty;

	return res;
}
float intra_triangle_distance_new(float** gr, int numtriangle, int k)
{
	float maxdis = -1;
	for(int i = 0; i< numtriangle-1; i++)
	{
			for(int j=i+1; j < numtriangle; j++)
			{
					float distance = 0;
					for(int t = 0; t<k; t++)
					{
						distance += (gr[i][t]-gr[j][t])*(gr[i][t]-gr[j][t]);
					}
					distance = sqrt(distance);
					if(distance > maxdis)
					{
						maxdis = distance;
					}
		}
	}
	return maxdis;
}
float **feature(Mat gray,Mat mask,int num_tri , int num_sub,int cx,int cy){
	int height=gray.rows,width=gray.cols;
	Mat mask2=mask/255;
	Mat lable1=Mat(height,width,CV_8UC1,Scalar(0));
	Mat lable2=Mat(height,width,CV_8UC1,Scalar(0));

	for(int i=0;i<num_sub;i++)
	{
		Mat rotateMat;
		Mat temp;
		rotateMat = cv::getRotationMatrix2D(Point(cx,cy), 0, 1-1/float(num_sub)*i);
		warpAffine(mask2, temp,rotateMat , mask2.size());
		add(lable1,temp,lable1);
	}
	float angle=2*CV_PI/float(num_tri);
	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++)
		{
				lable2.at<uchar>(y,x)=std::floor( (std::atan2((float)(y-cy),(float)(x-cx))+CV_PI)/angle );
		}
//	imshow("l1",lable1*40);
//	imshow("l2",lable2*15);
//	waitKey();

	float ** pixel=new float*[num_tri];
	for(int i=0;i<num_tri;i++)
		pixel[i]=new float[num_sub];

	float ** ave=new float*[num_tri];
	for(int i=0;i<num_tri;i++)
		ave[i]=new float[num_sub];
	for(int i=0;i<num_tri;i++)
		for(int j=0;j<num_sub;j++)
		{
			pixel[i][j]=0;
			ave[i][j]=0;
		}

	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++)
			if(lable1.at<uchar>(y,x)!=0 && lable2.at<uchar>(y,x)>=0 && lable2.at<uchar>(y,x)<num_tri)
			{
				pixel[lable2.at<uchar>(y,x)][lable1.at<uchar>(y,x)-1]++;
				ave[lable2.at<uchar>(y,x)][lable1.at<uchar>(y,x)-1]+=gray.at<uchar>(y,x);
			}
	for(int i=0;i<num_tri;i++)
		for(int j=0;j<num_sub;j++)
			ave[i][j]=ave[i][j]/pixel[i][j];
	//for(int i=0;i<num_tri;i++)
	//{
	//	for(int j=0;j<num_sub;j++)
	//		cout<<ave[i][j]<<"   ";
	//	cout<<endl;
	//}
	for (int i = 0; i< num_tri; i++)
		delete pixel[i];
	delete pixel;
	return ave;
}
float triangle_color_feature_new(Mat gray, Mat mask, int numtri, int num_sub, my_image* Ib_min)
{
	float res = 0;
	my_point* centroid = find_centroid(Ib_min);
	float** arr = feature(gray, mask, numtri , num_sub, int(centroid->c),int (centroid->r));
	res = intra_triangle_distance_new(arr,numtri, num_sub);

	for (int i = 0; i< numtri; i++)
		delete arr[i];
	delete arr;
	return res;
}
//////////------------------End Color----------------------////////
//////////------------------Asymmetric----------------------////////

Mat rotate_image(const Mat& source, Point2f src_center, double angle)
{
//	http://stackoverflow.com/questions/2289690/opencv-how-to-rotate-iplimage
//	In OpenCV a positive angle is counter-clockwise
    Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
    Mat dst;
    warpAffine(source, dst, rot_mat, source.size(),INTER_LINEAR);
    return dst;
}
float orientation(my_image* Ib_min)
{
	float theta = 0.0;
	float m00 = moment(Ib_min,0,0);
	float m10 = moment(Ib_min,1,0);
	float m01 = moment(Ib_min,0,1);
	float r0 = m10/m00;
	float c0 = m01/m00;
//	cout <<"center: cot, hang (toa do anh):" << c0 <<" " <<r0 <<endl;
	float mu11 = central_moment(Ib_min,r0,c0,1,1);
	float mu20 = central_moment(Ib_min,r0,c0,2,0);
	float mu02 = central_moment(Ib_min,r0,c0,0,2);
	//major axis orientation
	theta = 0.5*atan(2*mu11/(mu20-mu02));
//	cout<<"major axis orientation radian, degree: " << theta << " " << theta*180/CV_PI <<endl;
	return theta;
}
void compute_score(my_image* Ib_min, float& Ax, float& Ay)
{
	Ax = 0.0;
	Ay = 0.0;
	int H = Ib_min->height;
	int W = Ib_min->width;
	float m00 = moment(Ib_min,0,0);
	float m10 = moment(Ib_min,1,0);
	float m01 = moment(Ib_min,0,1);
	int r0 = round(m10/m00);//centroid
	int c0 = round(m01/m00);
	//tinh doi xug qua truc ngang x
	int tren = r0;
	int duoi = H-r0-1;
	int count;

	if(tren<duoi)//chay tu giua (r0) nguoc len trenva lay doi xung
	{
		count = -1;
		for(int y=r0-1; y>=0; y--)
		{
			count ++;
			for(int x=0;x<W;x++)
				if(Ib_min->data[y][x] != Ib_min->data[r0+1+count][x])
					Ax++;
		}
		for(int y=r0+1+count+1; y<H;y++)//chay phan con lai ben duoi
			for(int x=0; x<W; x++)
				if(Ib_min->data[y][x] == 255)
					Ax++;
	}
	else//chay tu giua xuong duoi va lay doi xung
	{
		count = r0;
		for(int y=r0+1; y<H;y++)
		{
			count --;
			for(int x=0;x<W;x++)
				if(Ib_min->data[y][x] != Ib_min->data[count][x])
					Ax++;
		}
		for(int y = count-1; y>=0; y--)//chay phan con lai ben tren
			for(int x = 0; x<W; x++)
				if(Ib_min->data[y][x] == 255)
					Ax++;
	}
	//tinh doi xung qua truc thang dung y
	int trai = c0;
	int phai = W-c0-1;
	if(phai < trai) // chay tu c0 sang phai va lay doi xung
	{
		count = c0;
		for(int x=c0+1; x<W;x++)
		{
			count --;
			for(int y = 0; y<H;y++)
				if(Ib_min->data[y][x] != Ib_min->data[y][count])
					Ay++;
		}
		for(int x=count-1; x>=0; x--)
			for(int y=0; y<H; y++)
				if(Ib_min->data[y][x] == 255)
					Ay++;
	}
	else// chay tu c0 sang trai va lay doi xung
	{
		count = c0;
		for(int x=c0-1;x>=0;x--)
		{
			count++;
			for(int y = 0; y<H; y++)
				if(Ib_min->data[y][x] != Ib_min->data[y][count])
					Ay++;
		}
		for(int x=count+1; x<W; x++)
			for(int y = 0; y<H; y++)
				if(Ib_min->data[y][x] == 255)
					Ay++;
	}
}
//compute area of object
float area_img(my_image* Ib)
{
	float A = 0.0;
	for(int y=0; y< Ib->height;y++)
		for(int x = 0; x<Ib->width; x++)
			if(Ib->data[y][x] == 255)
				A++;
	return A;
}
float area_mat(Mat Ib)
{
	float A = 0.0;
	for(int y=0; y< Ib.rows;y++)
		for(int x = 0; x<Ib.cols; x++)
		{
			int tmp = Ib.at<uchar>(y,x);
			if(tmp == 255)
				A++;
		}
	return A;
}
//compute asymmetric score
float asymmetric_feature(my_image* Ib_min, Mat Ib_min_mat)
{
	float res = 0;
//	int H = Ib_min->height;
//	int W = Ib_min->width;

	float m00 = moment(Ib_min,0,0);
	float m10 = moment(Ib_min,1,0);
	float m01 = moment(Ib_min,0,1);
	float r0 = m10/m00;//centroid
	float c0 = m01/m00;
	cout <<"center: cot, hang (toa do anh):" << c0 <<" " <<r0 <<endl;
	float theta = orientation(Ib_min);
	cout<<"major axis orientation radian, degree: " << theta << " " << theta*180/CV_PI <<endl;
	Mat Ib_min_rotate = rotate_image(Ib_min_mat,Point2f(c0,r0),-theta*180/CV_PI);
//	Point2f src_center(Ib_min_mat.cols/2.0F, Ib_min_mat.rows/2.0F);
//	Mat Ib_min_rotate = rotate_image(Ib_min_mat,src_center,-theta*180/CV_PI);
	Mat Ib_min_rotate_thresh(Ib_min_rotate.rows,Ib_min_rotate.cols,CV_8UC1);
	threshold(Ib_min_rotate,Ib_min_rotate_thresh, 128, 255, THRESH_BINARY);

//	namedWindow("Ib_min_rotate", CV_WINDOW_AUTOSIZE);
//	imshow("Ib_min_rotate", Ib_min_rotate);
//	waitKey(0);

	int tlrow1, tlcol1,brrow1,brcol1;
	get_bound_rect_matrix(Ib_min_rotate_thresh, tlrow1, tlcol1, brrow1, brcol1);
	int rec_H1 = brrow1 - tlrow1 + 1;
	int rec_W1 = brcol1 - tlcol1 + 1;
	Rect r1(tlcol1, tlrow1, rec_W1, rec_H1);
	Mat Ib_min_rotate_thresh_bound = Ib_min_rotate_thresh(r1);

	double scale1 = 256.0/max(rec_H1,rec_W1);
	int Hnew1 = (int)ceil(scale1*rec_H1);
	int Wnew1 = (int)ceil(scale1*rec_W1);
	cout<<"sau khi resize hang cot: "<<Hnew1 <<" " <<Wnew1 <<endl;
	Mat Ib_min_rotate_thresh_bound_resize;
	resize(Ib_min_rotate_thresh_bound,Ib_min_rotate_thresh_bound_resize,Size(Wnew1,Hnew1),0,0,INTER_CUBIC);
	Mat I_b_final(Ib_min_rotate_thresh_bound_resize.rows,Ib_min_rotate_thresh_bound_resize.cols,CV_8UC1);
	threshold(Ib_min_rotate_thresh_bound_resize,I_b_final, 128, 255, THRESH_BINARY);
	my_image* I_b_final_img = get_binary_image_from_binary(I_b_final);

//	namedWindow("I_final", CV_WINDOW_AUTOSIZE);
//	imshow("I_final", I_final);
//	waitKey(0);

	float Ax, Ay;
//	float A = area_img(I_b_final_img);//nhu area_mat
	float A = area_mat(I_b_final);
	cout<<"Area: "<<A<<endl;
	compute_score(I_b_final_img,Ax,Ay);
	cout<<"Ax Ay: "<<Ax<<" "<<Ay<< endl;
	res = min(Ax,Ay)/A;
	cout<<"asymmetric score: "<<res<<endl;

	Ib_min_rotate.release();
	Ib_min_rotate_thresh.release();
	Ib_min_rotate_thresh_bound.release();
	Ib_min_rotate_thresh_bound_resize.release();
	I_b_final.release();
	image_delete(I_b_final_img);
	return res;
}
//////////------------------End asymmetric----------------------////////
//////////------------------Texture----------------------////////
void get_one_chanel_of_matrix(Mat rgb, Mat& dst, int chanel)
{
	for (int y = 0; y < rgb.rows; y++)
	{
		for (int x = 0; x < rgb.cols; x++)
		{
			Vec3b intensity = rgb.at<Vec3b>(y, x);
			uchar R = intensity.val[0];
			uchar G = intensity.val[1];
			uchar B = intensity.val[2];
			switch (chanel)
			{
				case 1:
					dst.at<uchar>(y,x) = R; break;
				case 2:
					dst.at<uchar>(y,x) = G; break;
				case 3:
					dst.at<uchar>(y,x) = B; break;
			}
	   }
	}
}
void print_matrix_1D(Mat im, char* filename)
{
	FILE*f;
	f = fopen(filename,"wt");
	fprintf(f,"hang, cot: %d %d\n",im.rows,im.cols);
	for(int y = 0; y<im.rows; y++)
	{
		for(int x = 0; x<im.cols; x++)
		{
			int tmp = im.at<uchar>(y,x);
			fprintf(f,"%d ",tmp);
		}
		fprintf(f,"\n");
	}
}
float count_edge_point(Mat edge, Mat Ib)
{
	float res = 0;
	for(int y=0; y<Ib.rows; y++)
		for(int x = 0; x<Ib.cols; x++)
		{
			int tmp = Ib.at<uchar>(y,x);
			int tmp_edge = edge.at<uchar>(y,x);
			if (tmp == 255 && tmp_edge==255)//pixel thuoc object va thuoc edge
				res++;
		}
	return res;
}
float canny_point(Mat src, Mat Ib, int lowThreshold)
{
//	http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html
	float res = 0.0;
	float ratio = 3.0;
	int kernel_size = 3;
	bool L2gradient = true;
//	int lowThreshold = 50;
//	Mat dst;
//	dst.create( src.size(), src.type() );
//	Mat src_gray(src.rows,src.cols,CV_8UC1);
	Mat src_gray;
	src_gray.create(src.size(),CV_8UC1);//phai dinh nghia gray la CV_*UC neu ko se bi loi
	get_one_chanel_of_matrix(src,src_gray,1);//get red chanel
//	cvtColor( src, src_gray, CV_BGR2GRAY );

//	Mat detected_edges(src_gray.rows,src_gray.cols,CV_8UC1);
	Mat detected_edges;
//	detected_edges.create(src_gray.size(),src_gray.type());
	blur( src_gray, detected_edges, Size(3,3) );
	Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size,L2gradient );
//	dst = Scalar::all(0);
//	src.copyTo( dst, detected_edges);

//	namedWindow("edge", CV_WINDOW_AUTOSIZE);
//	imshow("edge", detected_edges);
//	waitKey(0);

//	print_matrix_1D(detected_edges,"matrix.txt");
	float numpoint = count_edge_point(detected_edges,Ib);
	float A = area_mat(Ib);
//	dst.release();
	src_gray.release();
	detected_edges.release();
	res = numpoint/A;
	return res;
}
//////////------------------End Texture----------------------////////
//////////------------------Border----------------------////////
void polyfit(float *x, float *t, int n, float& a, float&b)
{
//	t = ax+b
	a = 0.0;
	b = 0.0;
	float sum_t = 0;
	float sum_x = 0;
	float sum_x_2 = 0;
	float sum_t_x = 0;
	for(int i=0; i<n;i++)
	{
		sum_t+=t[i];
		sum_x+= x[i];
		sum_x_2 += x[i]*x[i];
		sum_t_x += t[i]*x[i];
	}
	b = (sum_t*sum_x_2-sum_t_x*sum_x)/(n*sum_x_2-sum_x*sum_x);
	a = (sum_t_x - b*sum_x)/sum_x_2;
}
void polyval(float*x, float*y, int n, float a, float b)
{
	for(int i = 0; i<n; i++)
		y[i] = a*x[i] + b;
}
float find_min(float *arr, int n)
{
	float min_value = arr[0];
	for(int i = 1; i<n; i++)
		if(arr[i] < min_value)
			min_value = arr[i];
	return min_value;
}
float find_max(float *arr, int n)
{
	float max_value = arr[0];
	for(int i = 1; i<n; i++)
		if(arr[i] > max_value)
			max_value = arr[i];
	return max_value;
}
float compute_angle(float u1, float u2, float v1, float v2)
{
	float anpha = 0;
	float product = u1*v1 + u2*v2;
	float norm_u = sqrt(u1*u1+u2*u2);
	float norm_v = sqrt(v1*v1+v2*v2);
	float tmp = product/(norm_u*norm_v);
	anpha = acos(tmp);
	return anpha;
}

float border_feature(my_image* Ib_min)
{
	float res = 0;

	vector<my_point> list1;
	find_boundary_pixels(Ib_min,list1);
	//	o day 648 point trong khi matlab la 658 point
//	point_list* list1 = list_shift(list,Ib_min);
//	cout<< "size list: "<<list1.size()<< endl;
	int numline = 16;
	int tt = list1.size();
	int step = tt/numline;
	if (step<1)
	{
		step = 1;
	}

	vector<my_point> L;
	// map toa do hang trong anh den toa do truc descarte (*)
//	for(int i=0; i<list1.size(); i++)
//	{
//		list1[i].r = float(Ib_min->height) - my_point(list1[i]).r -1;
//	}
	for (int i = 0; i<= tt-step; i = i+step)
	{
		float*x = new float[step];
		float*y = new float[step];
		for(int j=i;j<i+step;j++)
		{
			x[j-i] = my_point(list1[j]).c;
//			y[j-i] = my_point(list1[j]).r;
			//vi ko gan duoc o cau lenh (*) nen phai gan o day
			y[j-i] = float(Ib_min->height) - my_point(list1[j]).r - 1; //map toa do hang ve descarte
		}
		float min_value = find_min(x,step);
		float max_value = find_max(x,step);
		if(min_value == max_value)//duong thang x = const
		{
			for(int k = 0; k<step-1; k++)
			{
				if(y[k]!=y[k+1])//tim 2 diem khac nhau tren duong thang x = const;
				{
					my_point point;
					point.r = y[k+1]-y[k];
					point.c = 0;
					L.push_back(point);
					break;
				}
			}
		}
		else
		{
			float a,b;
			polyfit(x,y,step,a,b);
			float* y2 = new float[step];
			polyval(x,y2,step,a,b);
//			for(int k=0; k<step;k++)
//			{
//				cout<<y2[k] <<" ";
//			}
//			cout <<endl;
			//tim 2 diem khac nhau tren duong thang va tinh VTCP
			for(int k = 0; k<step-1; k++)
			{
				if(x[k]!= x[k+1] || y2[k]!=y2[k+1])//tim 2 diem khac nhau tren duong thang
				{
					my_point point;
					point.r = y2[k+1] - y2[k];
					point.c = x[k+1] - x[k];
					L.push_back(point);
					break;
				}
			}
			delete y2;
		}
		delete x;
		delete y;
	}
	//compute angle anpha between two adjacent lines
	float* angle_arr = new float[L.size()];
	float anpha;
	for(int i=0; i<L.size()-1; i++)
	{
		float u1 = my_point(L[i]).c;
		float u2 = my_point(L[i]).r;
		float v1 = my_point(L[i+1]).c;
		float v2 = my_point(L[i+1]).r;
		anpha = compute_angle(u1,u2,v1,v2);
		angle_arr[i] = CV_PI - anpha;
	}
	int kthuoc = L.size()-1;
	anpha = compute_angle(my_point(L[L.size()-1]).c,my_point(L[L.size()-1]).r,my_point(L[0]).c,my_point(L[0]).r);
	angle_arr[L.size()-1] = CV_PI - anpha;
	float ave_angle = 0.0;
	for(int i=0; i<L.size(); i++)
	{
		ave_angle += angle_arr[i];
	}
	ave_angle = ave_angle/L.size();
	res = ave_angle;
	cout<<"cac VTCP:"<<endl;
//	for(int i=0; i<L->size; i++)
//	{
//		cout<<L->data[i]->c<<" "<<L->data[i]->r<<endl;
//	}

//	point_list_delete(list);
	list1.clear();
	L.clear();
	delete angle_arr;
	return res;
}

#endif /* UTIL1_H_ */

