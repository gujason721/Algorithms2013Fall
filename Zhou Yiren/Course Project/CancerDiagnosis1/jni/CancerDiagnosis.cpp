#include <jni.h>
#include <android/log.h>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#include "my_point.h"
#include "my_image.h"
#include "contour-tracing.h"
#include "my_contour-tracing.h"
#include "util1.h"
#include "util2.h"

#include "my_segmentation.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "sift.h"
#include "imgfeatures.h"
#include "utils.h"

using namespace std;
using namespace cv;

#define LOG_TAG "CancerDiagnosis.cpp"
#define DPRINTF(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define IPRINTF(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define EPRINTF(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
extern "C" {

JNIEXPORT void JNICALL JNICALL Java_com_example_cancerdiagnosis1_PhotoIntentActivity_debugMessage(JNIEnv *, jobject);
JNIEXPORT void JNICALL Java_com_example_cancerdiagnosis1_PhotoIntentActivity_FindFeatures(JNIEnv*, jobject, jlong addrRgb);
JNIEXPORT jdoubleArray JNICALL Java_com_example_cancerdiagnosis1_PhotoIntentActivity_ColorFeature(JNIEnv*, jobject, jlong addrARGB, jlong addrBinary);
JNIEXPORT jintArray JNICALL Java_com_example_cancerdiagnosis1_PhotoIntentActivity_OtsuSegmentation(JNIEnv*, jobject, jlong addrGray, jlong addrRgba, jlong addrSkin, jlong addrmThre, jlong addrmask1);


JNIEXPORT void JNICALL JNICALL Java_com_example_cancerdiagnosis1_PhotoIntentActivity_debugMessage(JNIEnv *, jobject)
{
    DPRINTF( "%s\n", __FUNCTION__ );
    DPRINTF( "This is a debug message coming from Toan'C++ code!\n");
    char* s = (char*)malloc(sizeof(char)*100);
    s = (char*)realloc(s,sizeof(char)*200);
    strcpy (s, "test malloc - con ket!\n");
    DPRINTF("%s\n",s);
    free(s);
}

JNIEXPORT void JNICALL Java_com_example_cancerdiagnosis1_PhotoIntentActivity_FindFeatures(JNIEnv*, jobject, jlong addrRgb)
{
	size_t conket;
	Mat& mRgb = *(Mat*)addrRgb;
	Mat mGr;
//	RGB[A] to Gray: Y <- 0.299 * R + 0.587 * G + 0.114 * B
	cvtColor( mRgb, mGr,  CV_RGB2GRAY );
    vector<KeyPoint> v;
    FastFeatureDetector detector(30);
    detector.detect(mGr, v);
    for( unsigned int i = 0; i < v.size(); i++ )
    {
        const KeyPoint& kp = v[i];
        circle(mRgb, Point(kp.pt.x, kp.pt.y), 10, Scalar(255,0,0,255));
    }
    mGr.release();
}

//segmentation
//--------------segmentation method here--------------
JNIEXPORT jintArray JNICALL Java_com_example_cancerdiagnosis1_PhotoIntentActivity_OtsuSegmentation(JNIEnv* env, jobject, jlong addrGray, jlong addrRgba, jlong addrSkin, jlong matresSkin, jlong matresBW)
{
	Mat& mGr  = *(Mat*)addrGray;
	Mat& mRgb = *(Mat*)addrRgba;
	Mat& mCbcr = *(Mat*)addrSkin;
	Mat& mThre1 = *(Mat*)matresSkin;//color skin
	Mat& mask2 = *(Mat*)matresBW;// BW

/*
	//////--------------------------//////
	int num_sift = 0;
	int no_mole = 0;
	mRgb.copyTo(mThre1);

	//do skin detection
	Mat mThre = skin_detection(mRgb,mCbcr);
	Centeral(mThre,mThre,mThre.rows,mThre.cols);
	//count number of skin point
	int skinp = 0;
	int skinp_limit= -1;//mGr.rows * mGr.cols / 100;
	int no_skin = 0;
	for(int i=0;i<mThre.rows;i++)
		for(int j=0;j<mThre.cols;j++)
		{
			if(mThre.at<uchar>(i,j)!=0)
			{
				skinp++;
			}
		}
	if(skinp>skinp_limit)//if number of skin point > threshold
	{

//		mRgb.copyTo(mThre1);
//
//		for(int i=0;i<mThre.rows;i++)
//		{
//			for(int j=0;j<mThre.cols;j++)
//			{
//				if(mThre.at<uchar>(i,j)==0)
//				{
//					mThre1.at<Vec3b>(i,j).val[0]=0;
//					mThre1.at<Vec3b>(i,j).val[1]=0;
//					mThre1.at<Vec3b>(i,j).val[2]=0;
//				}
//			}
//		}

		//do segmentation
		Mat mask1 = toan_segmentation(mGr,num_sift);
		if (num_sift == 0)
		{
		//	no_mole = 1;
		}
		mask1.copyTo(mask2);
	}
	else
	{
		no_skin = 1;
	}
	int flag_tmp;
	if (no_skin == 1)
		flag_tmp = 2;
	else
	{
		if (no_mole == 1)
			flag_tmp = 1;
		else
		{
			flag_tmp = 0;
			draw_rectangle(mask2,mThre1);
		}
	}
*/
//	float max_size=-100;
//	Mat mask1=Mat(mRgb.rows,mRgb.cols,CV_8UC1,Scalar(0));

	int flag_tmp = OtsuSegmentation_c (mGr, mRgb, mCbcr, mThre1, mask2);
	int tlrow = -1, tlcol = -1, brrow = -1, brcol = -1;
	get_bound_rect_matrix(mask2,tlrow,tlcol,brrow,brcol);
	__android_log_print(ANDROID_LOG_INFO, "Toan:", "topleft_r_c, bottom_r_c: %d %d %d %d\n", tlrow,tlcol,brrow,brcol);
	draw_rectangle(mask2,mThre1);

/*
	//skin detection
	int skinp=0;
	Mat mThre = skin_detection(mRgb,mCbcr);
	Centeral(mThre,mThre,mThre.rows,mThre.cols);
	for(int i=0;i<mThre.rows;i++){
		for(int j=0;j<mThre.cols;j++){
			//cout<<(int)mThre.at<uchar>(i,j)<<"  ";
			if(mThre.at<uchar>(i,j)!=0){
				skinp++;
			}
		}
		//if(skinp>10) break;
	}

	if(skinp>10)
	{
		//Centeral(mThre,mThre,mThre.rows,mThre.cols);
		mRgb.copyTo(mThre1);
		for(int i=0;i<mThre.rows;i++)
		{
			for(int j=0;j<mThre.cols;j++)
			{
				if(mThre.at<uchar>(i,j)==0){
					mThre1.at<Vec3b>(i,j).val[0]=0;
					mThre1.at<Vec3b>(i,j).val[1]=0;
					mThre1.at<Vec3b>(i,j).val[2]=0;
				}
			}
		}


	//!!!!!!!!!!!!!!!!!
	//return mThre here

		//find feature on skin region
		vector<KeyPoint> v;
		int threshold = 10;
		int levels = 6;
		Ptr<FeatureDetector> detector = new FastFeatureDetector(threshold);
		PyramidAdaptedFeatureDetector a = PyramidAdaptedFeatureDetector(detector,levels);
		a.detect( mGr, v, mThre );
		//delete detector;

		int m_id;
		for( unsigned int i = 0; i < v.size(); i++ )
		{
			const KeyPoint &sss=v[i];
			if(sss.size>max_size){
				max_size=sss.size;
				m_id=i;
			}
		}
		const KeyPoint &max_k=v[m_id];

		//
		if(max_size!=-100)
		{
			//otsu segmentation
			Mat img1=Mat(mRgb.rows,mRgb.cols,CV_8UC1,Scalar(0));
			cvtColor(mRgb,img1,CV_RGBA2GRAY);
			int startx=int(max_k.pt.x-max_k.size)>0?int(max_k.pt.x-max_k.size):0;
			int starty=int(max_k.pt.y-max_k.size)>0?int(max_k.pt.y-max_k.size):0;
			int lengthx=int(startx+2*max_k.size)<img1.cols?int(2*max_k.size):(img1.cols-startx);
			int lengthy=int(starty+2*max_k.size)<img1.rows?int(2*max_k.size):(img1.rows-starty);
			Mat img=Mat(img1,Rect(startx,starty,lengthx,lengthy));
			Mat mask;
			Mat imgf;

			medianBlur(img,imgf,7);
			applyGaussian( img, imgf );

			OstuThreshholding(imgf,mask); //Or, try this: threshold(imgf, mask , 0  , 255  , cv::THRESH_OTSU | cv::THRESH_BINARY_INV);

			medianBlur(mask,mask,5);

			OstuPostprocessing(mask);

//				Mat mask1=Mat(mRgb.rows,mRgb.cols,CV_8UC1,Scalar(0));
			for(int i=starty;i<starty+lengthy;i++){
				for(int j=startx;j<startx+lengthx;j++){
					mask1.at<uchar>(i,j)=mask.at<uchar>(i-starty,j-startx);
				}
			}
			//!!!!!!!!!!!!!!!//return mask1 here
			mask1.copyTo(mask2);
			int tlrow = -1, tlcol = -1, brrow = -1, brcol = -1;
			get_bound_rect_matrix(mask2,tlrow,tlcol,brrow,brcol);
			__android_log_print(ANDROID_LOG_INFO, "Toan:", "topleft_r_c, bottom_r_c: %d %d %d %d\n", tlrow,tlcol,brrow,brcol);
			draw_rectangle(mask2,mThre1);
		}

	}
*/
	int flag[1];
	//__android_log_print(ANDROID_LOG_INFO, "Toan:", "number of skin point: %d\n", skinp);
	flag[0] = flag_tmp; //0: ok. 1: skin+nomole; 3: nomole + noskin
	__android_log_print(ANDROID_LOG_INFO, "Toan:", "flag: %d\n", flag_tmp);
	int len = 1;
	jintArray result = env->NewIntArray(len);
	env->SetIntArrayRegion(result, 0, len, flag);
	return result;
}

JNIEXPORT jdoubleArray JNICALL Java_com_example_cancerdiagnosis1_PhotoIntentActivity_ColorFeature(JNIEnv* env, jobject, jlong addrARGB, jlong addrBinary)
{
//	addrArgb: CV_8UC4
//	addrBinary: CV_8UC4

	//convert color image from ARGB to RGB (8UC3)
	/*
	Mat& ARGB = *(Mat*)addrARGB;
	Mat RGBA = convert_argb_to_rgba(ARGB);
	Mat mRgb(RGBA.rows,RGBA.cols,CV_8UC3);
	cvtColor( RGBA, mRgb,  CV_RGBA2RGB);

	//convert thresholded image from ARGB to GRAY (8UC1)
	Mat& Binary_ARGB  = *(Mat*)addrBinary;
	Mat Binary_RGBA = convert_argb_to_rgba(Binary_ARGB);
	Mat mBinary(Binary_RGBA.rows,Binary_RGBA.cols,CV_8UC1);
	cvtColor( Binary_RGBA, mBinary,  CV_RGBA2GRAY);
	*/

	Mat& mRgb = *(Mat*)addrARGB;
	Mat& mBinary = *(Mat*)addrBinary;

	Mat mBinary_thresh(mBinary.rows,mBinary.cols,CV_8UC1);
	threshold(mBinary,mBinary_thresh, 128, 255, THRESH_BINARY);
	int tlrow = -1, tlcol = -1, brrow = -1, brcol = -1;
	get_bound_rect_matrix(mBinary_thresh,tlrow,tlcol,brrow,brcol);

	int rec_H = brrow - tlrow + 1;
	int rec_W = brcol - tlcol + 1;
	Rect r(tlcol, tlrow, rec_W, rec_H);
	Mat mRgb_min = mRgb(r);
	Mat mBin_min = mBinary_thresh(r);

	//	resize boundary rectangle to 256x
	double scale = 256.0/max(rec_H,rec_W);
	int Hnew = (int)ceil(scale*rec_H);
	int Wnew = (int)ceil(scale*rec_W);
	Mat mRgb_tmp;
	resize(mRgb_min,mRgb_tmp,Size(Wnew,Hnew),0,0,INTER_CUBIC);
	my_color_image* Iori_tmp = get_rgb_image_from_rgb(mRgb_tmp);
	my_image* Igray_tmp = get_one_chanel_of_color_image(Iori_tmp,1);

	Mat mBin_tmp;
	resize(mBin_min,mBin_tmp,Size(Wnew,Hnew),0,0,INTER_CUBIC );
	cout << "size of rect_bound after resize:" << mBin_tmp.rows <<" "<<mBin_tmp.cols<<endl;
	Mat mBin_tmp_1(mBin_tmp.rows,mBin_tmp.cols,CV_8UC1);
	threshold(mBin_tmp,mBin_tmp_1, 128, 255, THRESH_BINARY);
	my_image* Ib_tmp = get_binary_image_from_binary(mBin_tmp_1);

	Mat gray(mRgb_tmp.rows, mRgb_tmp.cols, CV_8UC1);
	cvtColor(mRgb_tmp,gray,CV_RGB2GRAY);

	/////////////////test-sift////////////////
//	IplImage* img = new IplImage(gray);
//	struct feature* features;
//	int n = sift_features( img, &features);
//	__android_log_print(ANDROID_LOG_INFO, "Toan:", "number of sift point: %d\n", n);
//    free(features); //vi features duoc cap phat boi calloc la mot vung nho lien tuc (va copy tung feature tu CvSeq vao) nen chi can free(features) la duoc)
    //////////////////////////////////////////

	float color_1 = triangle_color_feature_new(gray,mBin_tmp_1,16,4,Ib_tmp);

//	float color_1 = triangle_color_feature(Ib_tmp,Igray_tmp);
	float asysmetric = asymmetric_feature(Ib_tmp, mBin_tmp_1);

	int lowThreshold = 34;
	float numedge = canny_point(mRgb_tmp,mBin_tmp,lowThreshold);
	float border_1 = border_feature(Ib_tmp);

	double dactrung[4];
	dactrung[0] = color_1;
	dactrung[1] = asysmetric;
	dactrung[2] = numedge;
	dactrung[3] = border_1;
	int len = 4;
	jdoubleArray result = env->NewDoubleArray(len);
	env->SetDoubleArrayRegion(result,0,len,dactrung);

	gray.release();
	mRgb.release();
	mBinary.release();
	mBinary_thresh.release();
	mRgb_min.release();
	mBin_min.release();
	mRgb_tmp.release();
	mBin_tmp.release();
	mBin_tmp_1.release();
	color_image_delete(Iori_tmp);
	image_delete(Igray_tmp);
	image_delete(Ib_tmp);

	return result;
}
}
