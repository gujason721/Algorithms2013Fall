/*
 * my_segmentation.h
 *
 *  Created on: Nov 25, 2013
 *      Author: thanhtoan_do
 */

#ifndef MY_SEGMENTATION_H_
#define MY_SEGMENTATION_H_

#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include "sift.h"
#include "imgfeatures.h"
#include "utils.h"

using namespace std;
using namespace cv;
#define MAXSIZE 512

float check_central_value(Mat gray,float dev)
{
	int height=gray.rows;
	int width=gray.cols;
	int startx=int(dev*width);
	int endx=width-startx;
	int starty=int(dev*height);
	int endy=height-starty;
	int pixlel=0;
	int sum=0;

	for(int i=starty;i<endy;i++)
	{

		for(int j=startx;j<endx;j++)
		{
			pixlel++;
			sum=sum+gray.at<uchar>(i,j);
		}
	}
	float mean_gray=sum/pixlel;
	return mean_gray;
}

float moment(Mat im, int p, int q)
{
	float kq = 0;
	for (int x = 0; x<im.rows; x++)
		for(int y = 0; y<im.cols; y++)
		{
			float tmp = float (im.at<uchar>(x,y));
			kq = kq + tmp*pow(float(x),p)*pow(float(y),q);
		}
	return kq;
}
/*
 * goal: find centroid of mass of image im
 * output: centroid
 * */
Point2f find_centroid(Mat im)
{
	float m00 = moment(im,0,0);
	float m10 = moment(im,1,0);
	float m01 = moment(im,0,1);
	float r0 = m10/m00;
	float c0 = m01/m00;
	Point2f centroid;
	centroid.x = c0;
	centroid.y = r0;
	return centroid;
}

void find_min_boundary_keypoint(struct feature* feat, bool *index, int H, int W, int n,int radius, int& tlr, int& tlc, int& brr, int& brc)
{
	tlr = 10000; tlc = 10000; brr = -1; brc = -1;
	for(int i = 0; i<n; i++)
	{
		if(index[i])
		{
			int r = int(feat[i].y);
			int c = int(feat[i].x);
			float scl = feat[i].scl;
			int up_r = r-radius*scl;
			int bt_r = r+radius*scl;
			int left_c = c-radius*scl;
			int right_c = c+radius*scl;

			if (left_c >= 0 && left_c < tlc)
				tlc = left_c;
			if (right_c < W && right_c > brc)
				brc = right_c;
			if (up_r >=0 && up_r <tlr)
				tlr = up_r;
			if(bt_r<H && bt_r>brr)
				brr = bt_r;
		}
	}
}

bool* good_feature(struct feature* feat, int n, float th_scale1, float th_scale2, float th_dis2centroid, float r_centroid, float c_centroid)
{
	bool* index = (bool*)malloc(sizeof(bool)*n);
	for (int i = 0; i<n; i++)
		index[i] = false;

	for(int i = 0; i<n;i++)
	{
		//compute distance from keypoint to centroid
		float dis = sqrt((feat[i].x - c_centroid)*(feat[i].x - c_centroid) + (feat[i].y - r_centroid)*(feat[i].y - r_centroid));
		if (dis<th_dis2centroid && feat[i].scl > th_scale1 && feat[i].scl < th_scale2)
		{
			index[i] = true;
		}
	}
	return index;
}
float my_max(float a, float b)
{
	return a>b?a:b;
}
//convert from RGB to YCbCr
Mat rgb_2_ycbcr(Mat mRgb)
{
	int delta = 128;
	int width = mRgb.cols;
	int height = mRgb.rows;
	Mat mYcbcr = Mat(height,width,CV_8UC3,Scalar(0));
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			//rgb to ycbcr
			mYcbcr.at<Vec3b>(i,j).val[0]=int(0.2568*mRgb.at<Vec3b>(i,j).val[0]+0.5041*mRgb.at<Vec3b>(i,j).val[1]+0.0979*mRgb.at<Vec3b>(i,j).val[2]+16);
			mYcbcr.at<Vec3b>(i,j).val[1]=int(-0.1482*mRgb.at<Vec3b>(i,j).val[0]-0.291*mRgb.at<Vec3b>(i,j).val[1]+0.4392*mRgb.at<Vec3b>(i,j).val[2]+delta);
			mYcbcr.at<Vec3b>(i,j).val[2]=int(0.4392*mRgb.at<Vec3b>(i,j).val[0]-0.3678*mRgb.at<Vec3b>(i,j).val[1]-0.0714*mRgb.at<Vec3b>(i,j).val[2]+delta);
		}
	}
	return mYcbcr;
}

//skin detection
Mat skin_detection(Mat mRgb,Mat mCbcr)
{
	Mat mYcbcr = rgb_2_ycbcr(mRgb);
	int height=mRgb.rows;
	int width=mRgb.cols;
	Mat mSkin = Mat(height,width,CV_8UC1,Scalar(0));
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			int Cb=mYcbcr.at<Vec3b>(i,j).val[1];
			int Cr=mYcbcr.at<Vec3b>(i,j).val[2];
			mSkin.at<uchar>(i,j)=mCbcr.at<uchar>(Cb,Cr)<128?0:255;
		}
	}
	return mSkin;
}
//find large region in central
void Centeral(Mat img,Mat &mask222,int height,int width){
	int h2=height/2,w2=width/2;
	Mat bw=Mat(h2,w2,CV_8UC1,Scalar(0));
	Mat mask=Mat(h2+2,w2+2,CV_8UC1,Scalar(0));
	for(int x=width/4;x<width/4+w2;x++)
		for(int y=height/4;y<height/4+h2;y++)
			bw.at<uchar>(y-height/4,x-width/4)=img.at<uchar>(y,x);

	int max_size=-1,sx=-1,sy;
	for(int x=0;x<w2;x++)
		for(int y=0;y<h2;y++)
			if(bw.at<uchar>(y, x) !=0)
			{
				mask=Mat(h2+2,w2+2,CV_8UC1,Scalar(0));
				floodFill(bw,mask,Point(x,y),0,NULL,0,0);
				int size=countNonZero(mask)-2*h2-2*w2-4;
				if(size>max_size)
				{
					max_size=size;
					sx=x;
					sy=y;
				}
			}
	if(sx==-1)
	{
		mask222=Mat(height,width,CV_8UC1,Scalar(0));
		return;
	}
	Mat tt=Mat(height,width,CV_8UC1,Scalar(0));
	mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
	tt=img.clone();

	floodFill(tt,mask,Point(sx+width/4,sy+height/4),100,NULL,0,0);
	mask222=tt.clone();
	for(int i=0;i<width;i++)
		for(int j=0;j<height;j++)
			tt.at<uchar>(j,i)=tt.at<uchar>(j,i)==100?100:0;

	for(int x=0;x<width;x++)
	{
		if(tt.at<uchar>(0,x)!=100 && tt.at<uchar>(0,x)!=120)
		{
			mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
			floodFill(tt,mask,Point(x,0),120,NULL,0,0);
		}
		if(tt.at<uchar>(height-1,x)!=100&& tt.at<uchar>(height-1,x)!=120)
		{
			mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
			floodFill(tt,mask,Point(x,height-1),120,NULL,0,0);
		}
	}

	for(int y=0;y<height;y++)
	{
		if(tt.at<uchar>(y,0)!=100 &&tt.at<uchar>(y,0)!=120)
		{
			mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
			floodFill(tt,mask,Point(0,y),120,NULL,0,0);
		}
		if(tt.at<uchar>(y,width-1)!=100 && tt.at<uchar>(y,width-1)!=120)
		{
			mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
			floodFill(tt,mask,Point(width-1,y),120,NULL,0,0);
		}
	}

	for(int i=0;i<width;i++)
			for(int j=0;j<height;j++)
				mask222.at<uchar>(j,i)=tt.at<uchar>(j,i)==120?0:255;


}
void applyClosing( cv::Mat &binaryImage, int element_radius = 2 ) {
    int element_type   = cv::MORPH_ELLIPSE;

    // The structuring element used for dilation and erosion.
    Mat element = cv::getStructuringElement( element_type,
            Size( 2*element_radius + 1, 2*element_radius+1 ),
            Point( element_radius, element_radius ) );

    cv::dilate(binaryImage, binaryImage,
            element,
            Point(-1, -1),
            2
    );
     cv::erode(binaryImage, binaryImage,
            element,
            // Position of the anchor within the structuring element.
            // The default value -1,-1 means that the anchor is at the element center
            Point(-1, -1),
            // Iterations: the number of times this operation is applied
            2
    );
}
float OstuPostprocessing(Mat &bw)
{
	using namespace cv;
	int width=bw.cols;
	int height=bw.rows;
	Mat mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
	for(int x=0;x<width;x++)
		if(bw.at<uchar>(0,x)!=0)
			floodFill(bw,mask,Point(x,0),0,NULL,0,0);
	for(int x=0;x<width;x++)
		if(bw.at<uchar>(height-1,x)!=0)
			floodFill(bw,mask,Point(x,height-1),0,NULL,0,0);
	for(int y=0;y<height;y++)
		if(bw.at<uchar>(y,0)!=0)
			floodFill(bw,mask,Point(0,y),0,NULL,0,0);
	for(int y=0;y<height;y++)
		if(bw.at<uchar>(y,width-1)!=0)
			floodFill(bw,mask,Point(width-1,y),0,NULL,0,0);
	//applyClosing(bw,3);					//Closing to connect the region that disconnected by noise
	//medianBlur(bw,bw,5);

	Mat bw2=bw.clone();

//	imshow("aa",bw2);
//	waitKey();

	int seg_num=0;
	const int SEGSISE=300;
	int size[SEGSISE];
	int sx[SEGSISE],sy[SEGSISE];

	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++)
			if(bw.at<uchar>(y, x) !=0)
			{
				mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
				floodFill(bw,mask,Point(x,y),0,NULL,0,0);

				size[seg_num]=countNonZero(mask)-2*height-2*width-4;

				sx[seg_num]=x;
				sy[seg_num]=y;
				if(size[seg_num]>20)
				{
					//std::cout<<seg_num<<"  "<<size[seg_num]<<std::endl;
					seg_num++;
				}
			}
	int maxpixel=0,choose_lable;
	for(int i=0;i<seg_num;i++)
	{
		if(maxpixel<size[i])
		{
			choose_lable=i;
			maxpixel=size[i];
		}
	}
	if(maxpixel==0)
		return 0;
	mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
	floodFill(bw2,mask,Point(sx[choose_lable],sy[choose_lable]),100,NULL,0,0);

	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++)
			bw2.at<uchar>(y,x)=bw2.at<uchar>(y,x)==100?255:0;

	mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
	floodFill(bw2,mask,Point(0,0),100,NULL,0,0);
	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++)
			bw.at<uchar>(y,x)=bw2.at<uchar>(y,x)==100?0:255;
	return countNonZero(bw);
}
float OstuPostprocessingWithEdgeRemove(Mat &bw,struct feature* feat, bool *index,int n,int radius,int offset_x,int offset_y)
{
	imshow("BW",bw);
	waitKey();
	using namespace cv;
	int width=bw.cols;
	int height=bw.rows;
	Mat mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));

	for(int x=0;x<width;x++)
		if(bw.at<uchar>(0,x)!=0)
			floodFill(bw,mask,Point(x,0),0,NULL,0,0);
	for(int x=0;x<width;x++)
		if(bw.at<uchar>(height-1,x)!=0)
			floodFill(bw,mask,Point(x,height-1),0,NULL,0,0);
	for(int y=0;y<height;y++)
		if(bw.at<uchar>(y,0)!=0)
			floodFill(bw,mask,Point(0,y),0,NULL,0,0);
	for(int y=0;y<height;y++)
		if(bw.at<uchar>(y,width-1)!=0)
			floodFill(bw,mask,Point(width-1,y),0,NULL,0,0);

	//for(int i = 0; i<n; i++)				//for every box do the removal
	//{
	//	if(index[i])
	//	{
	//		int start_x=feat[i].x-radius*feat[i].scl - offset_x;
	//		int end_x=feat[i].x+radius*feat[i].scl - offset_x;
	//		int start_y=feat[i].y-radius*feat[i].scl - offset_y;
	//		int end_y=feat[i].y+radius*feat[i].scl - offset_y;
	//		if(start_x<0) start_x=0;
	//		start_x=max(start_x,0);
	//		end_x=min(end_x,width-1);
	//		start_y=max(start_y,0);
	//		end_y=min(end_y,height-1);

	//		for(int x=start_x;x<=end_x;x++)
	//		{
	//			if(bw.at<uchar>(start_y,x)!=0)
	//				floodFill(bw,mask,Point(x,start_y),0,NULL,0,0);
	//			if(bw.at<uchar>(end_y,x)!=0)
	//				floodFill(bw,mask,Point(x,end_y),0,NULL,0,0);
	//		}
	//		for(int y=start_y;y<end_y;y++)
	//		{
	//			if(bw.at<uchar>(y,start_x)!=0)
	//				floodFill(bw,mask,Point(start_x,y),0,NULL,0,0);
	//			if(bw.at<uchar>(y,end_x)!=0)
	//				floodFill(bw,mask,Point(end_x,y),0,NULL,0,0);
	//		}
	//		imshow("BW",bw);
	//		waitKey();
	//	}
	//}
	imshow("BW",bw);
	waitKey();
	//applyClosing(bw,3);					//Closing to connect the region that disconnected by noise
	//imshow("BW",bw);
	//waitKey();
	//medianBlur(bw,bw,5);


	//RemoveNotBlock
	Mat Temp_mask=Mat(height,width,CV_8UC1,Scalar(0));
	for(int i = 0; i<n; i++)
	{
		if(index[i])
		{
			int start_x=feat[i].x-radius*feat[i].scl - offset_x;
			int end_x=feat[i].x+radius*feat[i].scl - offset_x;
			int start_y=feat[i].y-radius*feat[i].scl - offset_y;
			int end_y=feat[i].y+radius*feat[i].scl - offset_y;
			if(start_x<0) start_x=0;
			start_x=max(start_x,0);
			end_x=min(end_x,width-1);
			start_y=max(start_y,0);
			end_y=min(end_y,height-1);

			for(int x=start_x;x<=end_x;x++)
			{
				for(int y=start_y;y<=end_y;y++)
				{
					Temp_mask.at<uchar>(y,x)=1;
				}
			}
		}
	}
	bw=bw.mul(Temp_mask*255);
	imshow("asx",Temp_mask*255);
	waitKey();


	Mat bw2=bw.clone();

//	imshow("aa",bw2);
//	waitKey();

	int seg_num=0;
	const int SEGSISE=300;
	int size[SEGSISE];
	int sx[SEGSISE],sy[SEGSISE];

	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++)
			if(bw.at<uchar>(y, x) !=0)
			{
				mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
				floodFill(bw,mask,Point(x,y),0,NULL,0,0);

				size[seg_num]=countNonZero(mask)-2*height-2*width-4;

				sx[seg_num]=x;
				sy[seg_num]=y;
				if(size[seg_num]>20)
				{
					//std::cout<<seg_num<<"  "<<size[seg_num]<<std::endl;
					seg_num++;
				}
			}
	int maxpixel=0,choose_lable;
	for(int i=0;i<seg_num;i++)
	{
		if(maxpixel<size[i])
		{
			choose_lable=i;
			maxpixel=size[i];
		}
	}
	if(maxpixel<100)
		return -1;
	mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
	floodFill(bw2,mask,Point(sx[choose_lable],sy[choose_lable]),100,NULL,0,0);

	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++)
			bw2.at<uchar>(y,x)=bw2.at<uchar>(y,x)==100?255:0;

	mask=Mat(height+2,width+2,CV_8UC1,Scalar(0));
	floodFill(bw2,mask,Point(0,0),100,NULL,0,0);
	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++)
			bw.at<uchar>(y,x)=bw2.at<uchar>(y,x)==100?0:255;
	return 0;
}
void applyGaussian(cv::Mat &input, cv::Mat &output) {
    cv::GaussianBlur( input, output, cv::Size(3,3), 9);
}

float OstuThreshholding(Mat img,Mat &mask)
{
	using namespace cv;
	int width=img.cols;
	int height=img.rows;
	int hist[256];
	for(int i=0;i<256;i++)
		hist[i]=0;
	for(int i=0;i<width;i++)
		for(int j=0;j<height;j++)
			hist[img.at<uchar>(j,i)]++;
	float sum=0;
	double p[256];
	double Sp[256];
	double Sxp[256];
	for(int i=0;i<256;i++)
	{
		p[i]=hist[i];
		sum+=p[i];
	}
	for(int i=0;i<256;i++)
		p[i]/=sum;
	Sp[0]=p[0];
	Sxp[0]=0;
	for(int i=1;i<256;i++)
	{
		Sp[i]=Sp[i-1]+p[i];
		Sxp[i]=Sxp[i-1]+(i-1)*p[i];
	}
	float maxg=-1;
	int max_t;
	for(int i=1;i<256;i++)
	{
		double m1=Sxp[i];
		double m2=(Sxp[255]-Sxp[i]);
		double g= Sp[i] * (1-Sp[i]) * (m1-m2) * (m1-m2);
		if( g > maxg )
		{
			maxg=g;
			max_t=i;
		}
	}

	mask=Mat(img.rows,img.cols, CV_8UC1,Scalar(0) );
	for(int x=0;x<img.cols;x++)
		for(int y=0;y<img.rows;y++)
			mask.at<uchar>(y,x)=(img.at<uchar>(y,x)<max_t)?255:0;
	return maxg;
}

float OstuThreshholdingWithMask(Mat img,Mat &mask,Mat maskofSkin)
{
	using namespace cv;
	int width=img.cols;
	int height=img.rows;
	int hist[256];
	for(int i=0;i<256;i++)
		hist[i]=0;
	for(int i=0;i<width;i++)
		for(int j=0;j<height;j++)
			if(maskofSkin.at<uchar>(j,i)!=0)
				hist[img.at<uchar>(j,i)]++;

	float sum=0;
	double p[256];
	double Sp[256];
	double Sxp[256];
	for(int i=0;i<256;i++)
	{
		p[i]=hist[i];
		sum+=p[i];
	}
	for(int i=0;i<256;i++)
		p[i]/=sum;
	Sp[0]=p[0];
	Sxp[0]=0;
	for(int i=1;i<256;i++)
	{
		Sp[i]=Sp[i-1]+p[i];
		Sxp[i]=Sxp[i-1]+(i-1)*p[i];
	}
	float maxg=-1;
	int max_t;
	for(int i=1;i<256;i++)
	{
		double m1=Sxp[i];
		double m2=(Sxp[255]-Sxp[i]);
		double g= Sp[i] * (1-Sp[i]) * (m1-m2) * (m1-m2);
		if( g > maxg )
		{
			maxg=g;
			max_t=i;
		}
	}

	mask=Mat(img.rows,img.cols, CV_8UC1,Scalar(0) );
	for(int x=0;x<img.cols;x++)
		for(int y=0;y<img.rows;y++)
			if(maskofSkin.at<uchar>(y,x)!=0)
				mask.at<uchar>(y,x)=(img.at<uchar>(y,x)<max_t)?255:0;
			else
				mask.at<uchar>(y,x)=0;
	return maxg;
}

void PartialOstuThreshholding(Mat img,Mat &mask, int beginx1, int beginy1, int window1, int beginx2, int beginy2, int window2)
{
	using namespace cv;
	int width=img.cols;
	int height=img.rows;
	int hist[256];
	for(int i=0;i<256;i++)
		hist[i]=0;
	int endx1=(beginx1+window1)<width?(beginx1+window1):(width);
	int endy1=(beginy1+window1)<height?(beginy1+window1):(height);
	for(int i=beginx1;i<endx1;i++)
		for(int j=beginy1;j<endy1;j++)
			hist[img.at<uchar>(j,i)]++;
	float sum=0;
	double p[256];
	double Sp[256];
	double Sxp[256];
	for(int i=0;i<256;i++)
	{
		p[i]=hist[i];
		sum+=p[i];
	}
	for(int i=0;i<256;i++)
		p[i]/=sum;
	Sp[0]=p[0];
	Sxp[0]=0;
	for(int i=1;i<256;i++)
	{
		Sp[i]=Sp[i-1]+p[i];
		Sxp[i]=Sxp[i-1]+(i-1)*p[i];
	}
	float maxg=-1;
	int max_t;
	for(int i=1;i<256;i++)
	{
		double m1=Sxp[i];
		double m2=(Sxp[255]-Sxp[i]);
		double g= Sp[i] * (1-Sp[i]) * (m1-m2) * (m1-m2);
		if( g > maxg )
		{
			maxg=g;
			max_t=i;
		}
	}


	int endx2=(beginx2+window2)<width?(beginx2+window2):(width);
	int endy2=(beginy2+window2)<height?(beginy2+window2):(height);

	mask=Mat(endy2-beginy2,endx2-beginx2, CV_8UC1,Scalar(0) );

	for(int x=0;x<endx2-beginx2;x++)
		for(int y=0;y<endy2-beginy2;y++)
			mask.at<uchar>(y,x)=(img.at<uchar>(y+beginy2,x+beginx2)<max_t)?255:0;
}

void AdaptiveOstuThreshholding(Mat img,Mat &mask,int windowsize)
{
	using namespace cv;
	int width=img.cols;
	int height=img.rows;
	float nx=width/windowsize;
	float ny=height/windowsize;
	mask=Mat(img.rows,img.cols, CV_8UC1,Scalar(0) );
	for(int i=0;i<nx;i++)
	{
		for(int j=0;j<ny;j++)
		{
			int beginx1;
			int beginy1;
			int window1=3*windowsize;
			int beginx2;
			int beginy2;
			int window2=windowsize;
			if(i==0 && j==0)
			{
				beginx1=0;
				beginx2=0;
				beginy1=0;
				beginy2=0;
				window1=2*windowsize;
			}
			else if(i==0)
			{
				beginx1=0;
				beginx2=0;
				beginy1=windowsize*(j-1);
				beginy2=windowsize*j;
				window1=2*windowsize;
			}
			else if(j==0)
			{
				beginx1=windowsize*(i-1);
				beginx2=windowsize*i;
				beginy1=0;
				beginy2=0;
				window1=2*windowsize;
			}
			else
			{
				beginx1=windowsize*(i-1);
				beginx2=windowsize*i;
				beginy1=windowsize*(j-1);
				beginy2=windowsize*j;
			}
			Mat mask1;
			PartialOstuThreshholding(img,mask1,beginx1,beginy1,window1,beginx2,beginy2,window2);
			for(int x=beginx2;x<beginx2+mask1.cols;x++)
				for(int y=beginy2;y<beginy2+mask1.rows;y++)
					mask.at<uchar>(y,x)=(mask1.at<uchar>(y-beginy2,x-beginx2)<128)?0:255;
		}
	}
}

/*
input:
mGr: gray scale image
mRgb: color image
mCbcr: skin mask trained by Gaussian model
output:
 mThre1: skin image
 mThre2: bw image
 flag: 0: ok. 1 skin + no mole. 3: no skin+no mole
*/
int OtsuSegmentation_c(Mat mGr,Mat mRgb,Mat mCbcr ,Mat& mThre1,Mat& mask2)
{

	int width=mGr.cols;
	int height=mGr.rows;

	int siftp_num=0;
	float err_info_Ostu=0;
	//skin detection
	int skinp=0,skinp_limit= mGr.rows * mGr.cols /100;
	mask2=Mat(mRgb.rows,mRgb.cols,CV_8UC1,Scalar(0));
	Mat mThre = skin_detection(mRgb,mCbcr);

	//imshow("skin",mThre);
	//waitKey();

	Centeral(mThre,mThre,mThre.rows,mThre.cols);

	//imshow("center",mThre);
	//waitKey();

	for(int i=0;i<mThre.rows;i++){
		for(int j=0;j<mThre.cols;j++){
			//cout<<(int)mThre.at<uchar>(i,j)<<"  ";
			if(mThre.at<uchar>(i,j)!=0){
				skinp++;
			}
		}
		//if(skinp>10) break;
	}
	float central_gray_value=check_central_value(mGr,0.35);
	if(central_gray_value<80)
	{
		mRgb.copyTo(mThre1);
		for(int i=0;i<mThre.rows;i++){
			for(int j=0;j<mThre.cols;j++){
				if(mThre.at<uchar>(i,j)==0){
					mThre1.at<Vec3b>(i,j).val[0]=0;
					mThre1.at<Vec3b>(i,j).val[1]=0;
					mThre1.at<Vec3b>(i,j).val[2]=0;
				}
			}
		}
		Mat tmp_i;
		tmp_i=mGr.clone();
		medianBlur(tmp_i,tmp_i,9);
		OstuThreshholdingWithMask(tmp_i,mask2,mThre); //Or, try this: threshold(imgf, mask , 0  , 255  , cv::THRESH_OTSU | cv::THRESH_BINARY_INV);

		//imshow("sub_rect",tmp_i);
		//waitKey();
		//imshow("sub_rect",mask2);
		//waitKey();
		int pixel=OstuPostprocessing(mask2);
		//imshow("sub_rect",mask2);
		//waitKey();

		int flat[1];
		flat[0]=   ( pixel<100 ? 1:0) + (skinp<=skinp_limit?2:0);
		return flat[0];
	}
	else
	{
/////////The only thing changed is above

		if(skinp>skinp_limit)
		{
			mRgb.copyTo(mThre1);
			for(int i=0;i<mThre.rows;i++){
				for(int j=0;j<mThre.cols;j++){
					if(mThre.at<uchar>(i,j)==0){
						mThre1.at<Vec3b>(i,j).val[0]=0;
						mThre1.at<Vec3b>(i,j).val[1]=0;
						mThre1.at<Vec3b>(i,j).val[2]=0;
					}
				}
			}

			//another version based on SIFT
			struct feature* features;
			int n = 0;
			IplImage* img_sift=new IplImage(mGr);

			int intvls = SIFT_INTVLS;
			double sigma = SIFT_SIGMA;
			double contr_thr = 0.04;//SIFT_CONTR_THR;
			int curv_thr = 12;//SIFT_CURV_THR;
			int img_dbl = SIFT_IMG_DBL;
			int descr_width = SIFT_DESCR_WIDTH;
			int descr_hist_bins = SIFT_DESCR_HIST_BINS;
			n=_sift_features( img_sift, &features, intvls, sigma, contr_thr, curv_thr,img_dbl, descr_width, descr_hist_bins );

			//Mat temp;								//for drawing
			//cvtColor(mRgb,temp,CV_RGB2BGR);
			//Mat temp_gray;
			//cvtColor(mRgb,temp_gray,CV_RGBA2GRAY);

			bool *inskin=new bool[n];
			int inskin_num=0;
			float *score=new float[n];
			bool *selected=new bool[n];
			for(int i = 0; i <n ; i++ )
				selected[i]=false;

			float parame=4;
			float parame1=3;

			//check sift is in skin?
			for(int i = 0; i <n ; i++ )
			{
				int radio=int(features[i].scl*parame);
	//			int radio1=int(features[i].scl*parame1);
				inskin[i]=true;
				int key_startx=(features[i].x-radio)>0?(features[i].x-radio):0;
				int key_endx=(features[i].x+radio)<mThre.cols?(features[i].x+radio):(mThre.cols-1);
				int key_starty=(features[i].y-radio)>0?(features[i].y-radio):0;
				int key_endy=(features[i].y+radio)<mThre.rows?(features[i].y+radio):(mThre.rows-1);
				for(int j=key_startx;j<key_endx;j++)
				{
					for(int k=key_starty;k<key_endy;k++)
					{
						if(mThre.at<uchar>(k,j)==0)
						{
							inskin[i]=false;
							break;
						}
					}
					if(!inskin[i]) break;
				}
				if(inskin[i]) inskin_num++;
			}
			//calc score
			for(int i = 0; i <n ; i++ )
			{
				if(inskin[i])
				{
					float cendis_x= min(features[i].x/mGr.cols,1-features[i].x/mGr.cols);
					float cendis_y= min(features[i].y/mGr.rows,1-features[i].y/mGr.rows);
					float cendis=min(cendis_x,cendis_y);
					score[i]=features[i].scl*cendis;
					if(cendis<0.25)
						score[i]=-1;				//find only inside the half screen	and set outside point illegle
					if(features[i].scl<1.5)//2.5
						score[i]=-1;
					if(features[i].scl>20)//25
						score[i]=-1;

					//cout<<score[i]<<"   "<<endl;
				}
			}
			//select the  best  number_p   sift point
			int number_p=3;
			//int number_p=inskin_num/3;
			//if(number_p=0) number_p=2;

			for(int j = 0; j < number_p; j++ )
			{
				int max_p=-1;
				float max_score=-100;
				for(int i = 0; i <n ; i++ )
				{
					if(inskin[i] && !selected[i])
						if(score[i]>max_score)
						{
							max_p=i;
							max_score=score[i];
						}
				}
				cout<<"max"<<max_score<<endl;
				if (max_p!=-1  && max_score!=-1)
				{
					selected[max_p]=true;
					//cout<<"select "<<max_p<<endl;
					siftp_num++;
				}
			}

			//draw
	//		for(int i = 0; i < n; i++ )
	//		{
	//			if(inskin[i])
	//				circle(temp,Point(features[i].x,features[i].y),(features[i].scl)+ rand()/4000,selected[i]?Scalar(255,0,0):Scalar(0,255,0));
	//		}

			//imshow("sift",temp);
			//waitKey();

			if(siftp_num!=0)
			{
				int box_radius=4;
				//Method 1: OstuThreshholding with mask
				/*
				int tlr, tlc,  brr,  brc;
				find_min_boundary_keypoint(features,selected, mRgb.rows,mRgb.cols, n,box_radius, tlr, tlc, brr, brc);
				int rec_H = brr - tlr + 1;
				int rec_W = brc - tlc + 1;
				Rect r(tlc, tlr, rec_W, rec_H);
				Mat skin_mask_min = mThre(r); //gray_min: region to do Otsu
				Mat img_min = mGr(r);

				Mat mask;
				Mat imgf;

				applyGaussian( img_min, imgf );
				imshow("skin_mask",skin_mask_min);
				waitKey();

				OstuThreshholdingWithMask(imgf,mask,skin_mask_min);
				imshow("OstuThreshholdingWithMask",mask);
				waitKey();
				err_info_Ostu=OstuPostprocessingWithEdgeRemove(mask,features,selected,n,box_radius,tlc, tlr);	//if mole size=0 or <100 return error -1
				*/
				//Method 2: Ostu for every block and find the max region
				int max_pixel=-100,max_pixel_lable;
				Mat temp_best;
				for(int i = 0; i<n; i++)				//for every box do the removal
				{
					if(selected[i])
					{
						int start_x=features[i].x-box_radius*features[i].scl;
						int end_x=features[i].x+box_radius*features[i].scl;
						int start_y=features[i].y-box_radius*features[i].scl ;
						int end_y=features[i].y+box_radius*features[i].scl;
						if(start_x<0) start_x=0;
						start_x=max(start_x,0);
						end_x=min(end_x,width-1);
						start_y=max(start_y,0);
						end_y=min(end_y,height-1);

						Mat tmp_i,tmp_mask;
						tmp_i=mGr(Rect(start_x,start_y,end_x-start_x+1,end_y-start_y+1));
						medianBlur(tmp_i,tmp_i,9);
						OstuThreshholding(tmp_i,tmp_mask); //Or, try this: threshold(imgf, mask , 0  , 255  , cv::THRESH_OTSU | cv::THRESH_BINARY_INV);
						//imshow("sub_rect",tmp_i);
						//waitKey();
						//imshow("sub_rect",tmp_mask);
						//waitKey();
						int pixel=OstuPostprocessing(tmp_mask);
						//imshow("sub_rect",tmp_mask);
						//waitKey();
						if(pixel>max_pixel)
						{
							max_pixel_lable=i;
							tmp_mask.copyTo(temp_best);
							max_pixel=pixel;
						}
					}
				}
				if(max_pixel<100) err_info_Ostu=-1;

				//End Method 2
				if(err_info_Ostu!=-1)
				{
					/* Method 1
					for(int i=tlr;i<tlr+rec_H;i++){
						for(int j=tlc;j<tlc+rec_W;j++){
							mask2.at<uchar>(i,j)=mask.at<uchar>(i-tlr,j-tlc);
						}
					}
					*/
					int start_x=features[max_pixel_lable].x-box_radius*features[max_pixel_lable].scl;
					int end_x=features[max_pixel_lable].x+box_radius*features[max_pixel_lable].scl;
					int start_y=features[max_pixel_lable].y-box_radius*features[max_pixel_lable].scl ;
					int end_y=features[max_pixel_lable].y+box_radius*features[max_pixel_lable].scl;
					for(int i=start_x;i<end_x;i++){
						for(int j=start_y;j<end_y;j++){
							mask2.at<uchar>(j,i)=temp_best.at<uchar>(j-start_y,i-start_x);
						}
					}
				}
			}
			for(int i = 0; i < n; i++ )
				free(features[i].feature_data );
			free(features);
			delete inskin;
			delete score;
			delete selected;
		}
	}
	int flat[1];
	flat[0]=   ( (siftp_num==0  || err_info_Ostu==-1 )?1:0) + (skinp<=skinp_limit?2:0);
	return flat[0];
}

#endif /* MY_SEGMENTATION_H_ */
