/*
 * util2.h
 *
 *  Created on: Nov 30, 2013
 *      Author: thanhtoan_do
 */

#ifndef UTIL2_H_
#define UTIL2_H_

#include <iostream>

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include "sift.h"
#include "imgfeatures.h"
#include "utils.h"
using namespace cv;
using namespace std;
#define MY_VALUE 255

//H, W chieu cao va chieu rong cua hinh chu nhat can lay, co centroid la centroid cua anh
Mat get_central(Mat Ib, int H, int W)
{
	int central_r = Ib.rows/2;
	int central_c = Ib.cols/2;
	Rect r(central_c-W/2, central_r-H/2, W, H);
	Mat central_mat = Ib(r);
	return central_mat;
}
void determine_central_size(Mat Igray, int &h, int &w, int& num_sift)
{
	IplImage* img = new IplImage(Igray);
	struct feature* feat;
	int H = Igray.rows;
	int W = Igray.cols;
	int central_r = H/2;
	int central_c = W/2;
	int n = sift_features( img, &feat);
	cout<<"number of keypoint: "<<n<<endl;
//	draw_features(img, feat, n);
//	cvNamedWindow( "toan", 1 );
//	cvShowImage( "toan", img );
//	cvWaitKey( 0 );

	//lay tu tam di ra, moi lan tang len 1 it, neu moi lan tang ma lai co them K kp thi cu tang
	int tangH = 10;//moi lan tang chieu cao
	int tangW = tangH*(W*1.0/H);
	int min_increase_kp = 1;// so kp tang toi thieu trong moi lan
	int minH = 50; //kich thuoc hinh chu nhat nho nhat
	int minW = minH*(W*1.0/H);
	int tlc = central_c-minW/2;
	int tlr = central_r-minH/2;
	int brc = central_c+minW/2;
	int brr = central_r+minH/2;
	int pre_tlc = tlc;
	int pre_brr = brr;
	int pre_tlr = tlr;
	int pre_brc = brc;

	//tinh so kp trong vung minHxminW
	int count_kp = 0;
	for(int i = 0; i<n; i++)
	{
		if(feat[i].x > tlc && feat[i].x <brc && feat[i].y > tlr && feat[i].y < brr)
		{
			count_kp ++;
		}
	}
	cout<<"number of keypoint trong hinh chu nhat nho nhat: "<<count_kp<<endl;
	while(1)
	{
		tlc = pre_tlc-tangW/2;
		tlr = pre_tlr-tangH/2;
		brc = pre_brc+tangW/2;
		brr = pre_brr+tangH/2;
		if (tlc < 0 || tlr < 0 || brc >= W || brr >=H) //vuot qua kich thuoc anh -->thoat
		{
			h = pre_brr - pre_tlr - 1;
			w = pre_brc - pre_tlc - 1;
			break;
		}
		int count_kp1 = 0;
		for(int i = 0; i<n; i++)//dem so ko trong kich thuoc vua tang
		{
			if(feat[i].x > tlc && feat[i].x <brc && feat[i].y > tlr && feat[i].y < brr)
			{
				count_kp1 ++;
			}
		}
		if (count_kp1 - count_kp < min_increase_kp)//so key point ko tang len nua-->thoat
		{
			h = pre_brr - pre_tlr - 1;
			w = pre_brc - pre_tlc - 1;
			break;
		}
		else //cap nhat lai toa do va tang tiep
		{
			pre_tlc = tlc; pre_tlr = tlr; pre_brc = brc; pre_brr = brr;
			count_kp = count_kp1;
		}
	}
	cout<<"h = "<<h<<" "<<"w = "<<w<<endl;
	cout<<"number of keypoint in rectangle: "<<count_kp<<endl;
	free(feat);
	num_sift = count_kp;
}
/*
 * Mat Ib: binary image is padded
 * mask: output image
 * label: label of this connected region
 * r, c: coordinate of start point of this connected region
 * */
void bfs(Mat &Ib, Mat& mask, int label, int r, int c)
{
	mask.at<uchar>(r,c) = label;
	vector<Point2i> my_queue;
	my_queue.push_back(Point2i(c,r));
	int fy[8] = {-1,-1,-1,0,1,1, 1, 0};
	int fx[8] = {-1, 0,	1,1,1,0,-1,-1};
	for(int i = 0; i< int (my_queue.size()); i++)
	{
		int x = my_queue[i].x;
		int y = my_queue[i].y;
		for(int j = 0; j<8; j++)
		{
			int tmp = Ib.at<uchar>(y+fy[j],x+fx[j]);
			int tmp1 = mask.at<uchar>(y+fy[j],x+fx[j]);
			if (tmp == MY_VALUE && tmp1 == 0)//lan can nay mau trang va chua duoc gan nhan
			{
				my_queue.push_back(Point2i(x+fx[j],y+fy[j]));
				mask.at<uchar>(y+fy[j],x+fx[j]) = label;
				//break;
			}
		}
	}
	my_queue.clear();
}
/*
 * labeled connected region in image, gia su image da duoc pad roi
 * label duoc gan tu 1
 * */
int labeled_connnected_region(Mat &Ib, Mat &mask)
{
	int count_label = 0;
	for(int y = 0; y< Ib.rows; y++)
		for(int x= 0; x< Ib.cols; x++)
		{
			if(Ib.at<uchar>(y,x) == MY_VALUE  && mask.at<uchar>(y,x) == 0)//neu day la mau trang va chua duoc gan nhan
			{
				count_label = count_label+1;
				bfs(Ib, mask,count_label, y, x);
			}
		}
	return count_label;
}
/*
 * giu lai vung mau trang, con lai to mau den
 * Gia su anh Ib da duoc pad roi
 * */
void find_largest_connected_region(Mat &Ib)
{
//	Mat mask;
//	mask.create( Ib.size(), Ib.type() );
	Mat mask(Ib.rows,Ib.cols,CV_8UC1);
	mask = Scalar::all(0);

	int count_label = labeled_connnected_region(Ib,mask);

	cout<<"count_label = "<<count_label<<endl;
	int* tmp_arr = new int[count_label+1];
	for(int i = 1; i<=count_label; i++)
	{
		tmp_arr[i] = 0;
	}

	for(int y= 0; y<mask.rows; y++)
		for(int x=0; x<mask.cols; x++)
		{
			if(mask.at<uchar>(y,x) > 0)//chi tinh nhung pixel nao duoc gan label
				tmp_arr[mask.at<uchar>(y,x)] ++;
		}

	//find max connected region
	int max_value = tmp_arr[1];
	int max_label = 1;
	for(int i = 1; i<=count_label; i++)
	{
		if(tmp_arr[i]>max_value)
		{
			max_value = tmp_arr[i];
			max_label = i;
		}
	}
	for(int y = 0; y<Ib.rows; y++)
		for(int x = 0; x<Ib.cols; x++)
		{
			if(mask.at<uchar>(y,x)!=max_label)
			{
				Ib.at<uchar>(y,x) = 0;
			}
		}
//	imshow("Ib",Ib);
//	waitKey();
	delete tmp_arr;
}

/**
 * Used to avoid noise in the image.
 */
void applyGaussian_1(cv::Mat &input, cv::Mat &output) {
    double sigma = 1.5;
    cv::Mat gaussKernel = cv::getGaussianKernel(9,sigma,CV_32F);
    cv::GaussianBlur( input, output, cv::Size(3,3), 1.5);
}
void applyClosing_1( cv::Mat &binaryImage, int element_radius = 2 )
{
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
void padding_mat(Mat binary, Mat &Ipadded)
{
	for(int y = 0; y< Ipadded.rows; y++)
		for(int x = 0; x<Ipadded.cols; x++)
		{
			if(y==0 || y== Ipadded.rows-1 || x==0 || x== Ipadded.cols-1)
			{
				Ipadded.at<uchar>(y,x) = 0;
			}
			else
			{
				Ipadded.at<uchar>(y,x) = binary.at<uchar>(y-1,x-1);
			}
		}
}
void cvFillHoles(cv::Mat &input)
{
    //assume input is uint8 B & W (0 or 1)
    //this function imitates imfill(image,'hole')
    cv::Mat holes=input.clone();
    cv::floodFill(holes,cv::Point2i(0,0),cv::Scalar(255));
    for(int i=0;i<input.rows*input.cols;i++)
    {
        if(holes.data[i]==0)
            input.data[i]=255;
    }
}
Mat Otsu(Mat grayImage)
{
//	Mat grayImage;
//	cvtColor(inImage, grayImage, CV_RGB2GRAY);
	applyGaussian_1( grayImage, grayImage );
	Mat binaryImage;
	threshold(grayImage, binaryImage, 0, 255, cv::THRESH_OTSU | cv::THRESH_BINARY_INV);
	applyClosing_1( binaryImage, 2 );
	//imshow("Binary", binaryImage);
	//Mat outImage( inImage.rows, inImage.cols, inImage.type() );
	//inImage.copyTo( outImage, binaryImage );
	//imshow("Result", outImage);
	//waitKey(0);
	Mat Ipadded(binaryImage.rows+2, binaryImage.cols+2, CV_8UC1);
	padding_mat(binaryImage,Ipadded);
//	imshow("Binary", Ipadded);
	//waitKey(0);
	find_largest_connected_region(Ipadded);
	//imshow("Largestconnected", Ipadded);
	//waitKey(0);
	cvFillHoles(Ipadded);

	Rect r(1, 1, grayImage.cols, grayImage.rows);
	Mat Ib = Ipadded(r);
//	imshow("Largestconnected", Ib);
//	waitKey(0);
	binaryImage.release();
	Ipadded.release();
	return Ib;
}
Mat toan_segmentation(Mat gray, int& num_sift)
{
	int h, w;
	determine_central_size(gray, h, w, num_sift);

	int central_r = gray.rows/2;
	int central_c = gray.cols/2;
	int y1 = central_r-h/2;
	int x1 = central_c-w/2;
	Rect r(x1, y1, w, h);
	//cout<<h <<" "<<w<<endl;
	Mat central = gray(r);
	//cout<<central.rows<<" "<<central.cols<<endl;
	Mat central_otsu = Otsu(central);

	//imshow("otsu_image", central_otsu);

	Mat Ib(gray.rows,gray.cols,CV_8UC1);
	Ib = Scalar::all(0);

	for(int y =0; y<central_otsu.rows;y++)
		for(int x = 0; x<central_otsu.cols; x++)
		{
			Ib.at<uchar>(y+y1,x+x1) = central_otsu.at<uchar>(y,x);
		}
	central.release();
	central_otsu.release();
	return Ib;
}

#endif /* UTIL2_H_ */
