package com.example.cancerdiagnosis1;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;

//import android.R;
import android.app.Activity;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.util.TypedValue;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.VideoView;

//import org.opencv.android.BaseLoaderCallback;
//import org.opencv.android.LoaderCallbackInterface;
//import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
//import org.opencv.android.LoaderCallbackInterface;
//import org.opencv.android.OpenCVLoader;
//import org.opencv.android.Utils;
//import org.opencv.core.CvType;
//import org.opencv.core.Mat;
//import org.opencv.android.CameraBridgeViewBase;
//import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
//import org.opencv.imgproc.Imgproc;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;

import org.opencv.imgproc.Imgproc;
import org.opencv.core.*;
import org.opencv.highgui.*;
import org.opencv.ml.*;
//import com.example.cancerdiagnosis2.R;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.WindowManager;

public class PhotoIntentActivity extends Activity {

	private static final String    TAG = "OCVSample::Activity";
	private static final int ACTION_TAKE_PHOTO_B = 1;
	private static final int ACTION_TAKE_PHOTO_S = 2;
//	private static final int ACTION_TAKE_VIDEO = 3;
	private static final int ACTION_LOAD_IMAGE = 4;
	private static final int ACTION_DIAGNOSIS = 5;
	private static final String BITMAP_STORAGE_KEY = "viewbitmap";
	private static final String IMAGEVIEW_VISIBILITY_STORAGE_KEY = "imageviewvisibility";
	private ImageView mImageView;
	private TextView mTextView;
	
	private int targetW_ori;
	private int targetH_ori;
	
	private Bitmap mImageBitmap;
	private Bitmap mImageBitmap_scaled;
	private Bitmap mImageBitmap_load;
	private int MAXSIZE = 512;
	
//	Mat m;
	Mat matIori;
	Mat matIb;
	Mat matIori_tmp;
	Mat matIb_tmp;
	Mat m;
	Mat matSkin;
	Mat matGray;
	Mat matresSkin;
	Mat matresBW;
	Mat matskinmode;
	Mat matRGB;
	Mat matRGBA;
	
	private double[] std_arr = {46.865383, 0.047570, 0.090633, 0.288609};
	private double[] mean_arr = {86.201874, 0.064796, 0.085331, 2.360834};
	private double MAX_SVMOUT = 2.5;
	private double MIN_SVMOUT = -1.8;
	private int feature_len = 4;
	private double[] feature_arr;
	private int[] mRGBData;
	
	private String recommend1 = "The mole is normal.";  
	private String recommend2 = "Recommend: Clinic visit.";
	private String recommend3 = "Strong Recommend: Clinic visit!";
	File pathSVM = new File(Environment.getExternalStorageDirectory(),"samples/SVM.xml");
	CvSVM my_SVM2;
//	CvSVM my_SVM;
//	private static final String VIDEO_STORAGE_KEY = "viewvideo";
//	private static final String VIDEOVIEW_VISIBILITY_STORAGE_KEY = "videoviewvisibility";
//	private VideoView mVideoView;
//	private Uri mVideoUri;

	private String mCurrentPhotoPath;

	private static final String JPEG_FILE_PREFIX = "IMG_";
	private static final String JPEG_FILE_SUFFIX = ".jpg";
	private static final String FILE_TYPE = ".jpg"; // tai sao khong the luu bigpicture kieu png
	private AlbumStorageDirFactory mAlbumStorageDirFactory = null;
	
	private BaseLoaderCallback  mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
//                    m = new Mat(10,10,CvType.CV_8UC1);//test matran co hoat dong hay khog
                    // Load native library after(!) OpenCV initialization
                    System.loadLibrary("CancerDiagnosis");
//                    if (true)
//                    {
                    	my_SVM2 = new CvSVM();
                    	my_SVM2.load(pathSVM.getAbsolutePath());
//                		int id_svmfile = R.raw.svm;
//                		Resources res2=getResources();
//                		String svmpath1 = res2.getString(id_svmfile);
//                		my_SVM2.load("/res/raw/svm.xml");
//                    	my_SVM2.load("file:///android_asset/svm.xml");
                    	
//                    }
            		
//             		 Log.i(TAG, "OpenCV loaded successfully");
//                    Button t = (Button)findViewById(R.id.btnLoadPicture);
//                    t.setText("loaded_OCV");
//                    m.release();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };
    
//	 @Override
  public void onResume()
  {
      super.onResume();
      OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_6, this, mLoaderCallback);
  }
    
	/* Photo album for this application */
	private String getAlbumName() {
		return getString(R.string.album_name);//MyPictures
	}

	private File getAlbumDir()// return "/mnt/sdcard/Pictures/MyPictures/" 
	{
		File storageDir = null;

		if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) 
		{
			storageDir = mAlbumStorageDirFactory.getAlbumStorageDir(getAlbumName());
			//storageDir = "/mnt/sdcard/Pictures/MyPictures/"
			if (storageDir != null) 
			{
				if (! storageDir.mkdirs())
				{
					if (! storageDir.exists())
					{
						Log.d("MyPictures", "failed to create directory");
						return null;
					}
				}
			}
			
		}
		else 
		{
			Log.v(getString(R.string.app_name), "External storage is not mounted READ/WRITE.");
		}
		
		return storageDir;
	}

	private File createImageFile() throws IOException {
		// Create an image file name
		String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());//20130814_052244
		String imageFileName = JPEG_FILE_PREFIX + timeStamp + "_";//IMG_20130814_052244_
		File albumF = getAlbumDir();// "/mnt/sdcard/Pictures/MyPictures"
		File imageF = File.createTempFile(imageFileName, FILE_TYPE, albumF);
		// /mnt/sdcard/Pictures/MyPictures/IMG_20130814_052244_1965651860.png
		return imageF;
	}

	private File setUpPhotoFile() throws IOException {
		
		File f = createImageFile();
		mCurrentPhotoPath = f.getAbsolutePath();
		
		return f;
	}

	private void dispatchTakePictureIntent(int actionCode) {

		Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);		
		switch(actionCode) 
		{
			case ACTION_TAKE_PHOTO_B: 
			{
				File f = null;
				try 
				{
					f = setUpPhotoFile();
					mCurrentPhotoPath = f.getAbsolutePath();
	//				If the EXTRA_OUTPUT is not present, then a small sized image is returned as a Bitmap object 
	//				in the extra field. If the EXTRA_OUTPUT is present, then the full-sized image will be written 
	//				to the Uri value of EXTRA_OUTPUT.
					takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(f));
					//anh duoc chup se nam trong Uri cua takePictureIntent va duoc luu tam trong duong dan 
					// mCurrentPhotoPath
				} catch (IOException e) 
				{
					e.printStackTrace();
					f = null;
					mCurrentPhotoPath = null;
				}
				break;
			}
			/////my code: get mCurrentPhotoPath in case small picture
			case ACTION_TAKE_PHOTO_S: 
			{
				File f1 = null;	
				try 
				{
					f1 = setUpPhotoFile();
					mCurrentPhotoPath = f1.getAbsolutePath();// lay duong dan de luu file
				} catch (IOException e) 
				{
					e.printStackTrace();
					f1 = null;
					mCurrentPhotoPath = null;
				}
				break;
			}

			default:
				break;			
		} // switch

		startActivityForResult(takePictureIntent, actionCode);
	}

//	private void dispatchTakeVideoIntent() {
//		Intent takeVideoIntent = new Intent(MediaStore.ACTION_VIDEO_CAPTURE);
//		startActivityForResult(takeVideoIntent, ACTION_TAKE_VIDEO);
//	}
	//mycode load picture intent
	private void dispatchLoadPictureIntent() {
		Intent loadPictureIntent = new Intent(Intent.ACTION_PICK,
				android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
		startActivityForResult(loadPictureIntent, ACTION_LOAD_IMAGE);
	}
	// diagnosis intent
	private void dispatchDiagnosisIntent() {
		Intent diagnosisIntent = new Intent();
//		Intent diagnosisIntent = new Intent(Intent.ACTION_PICK,
//				android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
		startActivityForResult(diagnosisIntent, ACTION_DIAGNOSIS);
	}
	
	private void GetRGBFromBitmap(Bitmap bitmap)
	{
		int picw = bitmap.getWidth();
		int pich = bitmap.getHeight();
		mRGBData = new int[picw * pich];
        bitmap.getPixels(mRGBData, 0, picw, 0, 0, picw, pich);
       
        for (int y = 0; y < pich; y++)
        {
        	for (int x = 0; x < picw; x++)
	        {
	            int index = y * picw + x;
	            int tmp = mRGBData[index]; 
	            int anpha = (tmp >> 24) & 0xff; 
	            int Red = (tmp >> 16) & 0xff;     //bitwise shifting
	            int Green = (tmp >> 8) & 0xff;
	            int Blue = tmp & 0xff;
//	            int xx = Red+Green+Blue;
	            //R,G.B - Red, Green, Blue
	             //to restore the values after RGB modification, use next statement
//	            mRGBData[index] = 0xff000000 | (R << 16) | (G << 8) | B;
	        }
       }
	}
	private Bitmap LoadColorImageFromSDcard(File path)
	{
		BitmapFactory.Options options = new BitmapFactory.Options();
//		set bitmap co kieu ARGB
		options.inPreferredConfig = Bitmap.Config.ARGB_8888;
		
//		http://stackoverflow.com/questions/8575122/load-image-from-specific-folder-on-the-sdcard
//		File path = new File(Environment.getExternalStorageDirectory(),"samples/melanoma/all/34.jpg");
//		Bitmap mBitmap = BitmapFactory.decodeFile(path.getPath()+"/"+ "34.jpg");
		Bitmap mBitmap = BitmapFactory.decodeFile(path.getPath(),options);
		return mBitmap;		
	}
	private void feature_normalization()
	{
		int i;
		for (i = 0; i< feature_len; i++)
		{
			feature_arr[i] = (feature_arr[i]-mean_arr[i])/std_arr[i];
		}
	}
	private double post_process_score (double score)
	{
		double res = 0;
		if (score<MIN_SVMOUT)
			score = MIN_SVMOUT;
		if (score>MAX_SVMOUT)
			score = MAX_SVMOUT;
		if (score <0)//anh xa benign ve 0-50
		{
			res = 50*(score-MIN_SVMOUT)/(0-MIN_SVMOUT);
		}
		else//anh xa melanoma ve 50-100
		{
			res = 50 + 50*(score)/(MAX_SVMOUT);
		}
		return res;
	}
	 public float SVM_predict()
    {
    	float score;    
    	Mat feature_mat = new Mat(1, 4, CvType.CV_32F);
    	feature_normalization(); 
        for(int i=0;i<feature_len;i++)
        {
        	feature_mat.put(0, i, feature_arr[i]);
        }  
    	score =  my_SVM2.predict(feature_mat,true);
    	feature_mat.release();
        return score;
    }
	private void processImage(){	
		
		debugMessage();
		
//		URL fileURL = getClass().getClassLoader().getResource("svm.xml");
//    	String fileName = fileURL.getFile();

//		String svmpath = res2.getResourceName(id_svmfile);
//		"res/raw/svm.xml"
//		my_SVM = new CvSVM();
////    	my_SVM.load(svmpath+".xml");
	
		
//		doc Bitmap tu mImageView
//		AssetManager assets = getAssets();
////		assets.
//		try {
////			AssetFileDescriptor f = assets.openFd("svm.xml");
////			f.getFileDescriptor().
//		} catch (IOException e) {
//			// TODO Auto-generated catch block
//			e.printStackTrace();
//			
//		}
//		File pathskin = new File()
//		BitmapDrawable t = (BitmapDrawable) mImageView.getDrawable();
//		Bitmap Iori = t.getBitmap();
	
//		File pathori = new File(Environment.getExternalStorageDirectory(),"samples/34.jpg");

//		Bitmap Iori = LoadColorImageFromSDcard(pathori);
//		File pathb = new File(Environment.getExternalStorageDirectory(),"samples/34t.jpg");
//		Bitmap Ib = LoadColorImageFromSDcard(pathb);
		
		//resize longer edge to 256
//		int H = Iori.getHeight(); 
//		int W = Iori.getWidth();
//		double scale = 256.0/Math.max(H,W);
//		int Hnew = (int) Math.ceil(scale*H);
//		int Wnew = (int) Math.ceil(scale*W);
//		Bitmap Iori1 = Bitmap.createScaledBitmap(Iori, Wnew, Hnew, false);
//		Bitmap Ib1 = Bitmap.createScaledBitmap(Ib, Wnew, Hnew, false);
//		
//		mImageView.setImageBitmap(Iori);
//		mImageView.setVisibility(View.VISIBLE);
		
//		matIori = new Mat(Iori.getHeight(), Iori.getWidth(), CvType.CV_8UC4); //4 channel are ARGB
////		Button t = (Button)findViewById(R.id.btnLoadPicture);
////		t.setText("loaded_OCV");
//        Utils.bitmapToMat(Iori, matIori);//
//              
//      matIb = new Mat(Hnew, Wnew, CvType.CV_8UC1); //1 channel is gray
//        matIb = new Mat(Ib.getHeight(), Ib.getWidth(), CvType.CV_8UC4); //load binary image as 4chanel
//        Utils.bitmapToMat(Ib, matIb);
        
        //matfeature = new Mat(1,4,CvType.CV_32FC1);
        //ColorFeature(tmpIori.getNativeObjAddr(), matIb.getNativeObjAddr(), feature.getNativeObjAddr());
        
//        mTextView.append("Conket");
        
//        matIori.release();
//    	matIb.release();
//    	feature.release();
        
		if (mImageBitmap!=null)
		{
			
			mTextView.setTextSize(TypedValue.COMPLEX_UNIT_SP, 18);
			
			matRGBA = new Mat();
			Utils.bitmapToMat(mImageBitmap, matRGBA);
			matIori_tmp = new Mat(matRGBA.rows(),matRGBA.cols(),CvType.CV_8UC3);
			Imgproc.cvtColor(matRGBA, matIori_tmp, Imgproc.COLOR_RGBA2RGB);
			
			//File pathori = new File(Environment.getExternalStorageDirectory(),"samples/34.jpg");
			//matIori = Highgui.imread(pathori.getPath());
			////phai new truoc khi cvtColor neu ko se bi loi
			//matIori_tmp = new Mat(matIori.rows(),matIori.cols(),CvType.CV_8UC3);
		    //Imgproc.cvtColor(matIori, matIori_tmp, Imgproc.COLOR_BGR2RGB);
	        //Bitmap Iori = Bitmap.createBitmap(matIori.cols(), matIori.rows(), Bitmap.Config.ARGB_8888);
	        //Utils.matToBitmap(matIori_tmp, Iori);
			//mImageView.setImageBitmap(Iori);
			//mImageView.setVisibility(View.VISIBLE);
			
			//File pathb = new File(Environment.getExternalStorageDirectory(),"samples/34t.jpg");
	        //matIb = Highgui.imread(pathb.getPath(),0);
		
	        //segmentation
//	        File pathskin = new File(Environment.getExternalStorageDirectory(),"samples/skin_threshold_cbcr.bmp");
//	        matskinmode = Highgui.imread(pathskin.getPath());
//	        matSkin = new Mat(matskinmode.rows(), matskinmode.cols(),CvType.CV_8UC1);
//	        Imgproc.cvtColor(matskinmode,matSkin,Imgproc.COLOR_BGR2GRAY);
			
	        Resources res1=getResources();
	        int id1 = R.drawable.skin_threshold_cbcr;
	        
	        BitmapFactory.Options options = new BitmapFactory.Options();
//			options.inPreferredConfig = Bitmap.Config.ARGB_8888;
//			options.inJustDecodeBounds = false;
	    	Bitmap bm1=BitmapFactory.decodeResource(res1, id1, options);
	    	
	    	matSkin = new Mat();
	    	Utils.bitmapToMat(bm1,matSkin);
	    	Imgproc.cvtColor(matSkin,matSkin,Imgproc.COLOR_RGBA2GRAY);
	    	
	    	matGray = new Mat(matIori_tmp.rows(), matIori_tmp.cols(), CvType.CV_8UC1);
	    	Imgproc.cvtColor(matIori_tmp, matGray, Imgproc.COLOR_RGB2GRAY);
	    	
	    	matresSkin = new Mat(matIori_tmp.rows(), matIori_tmp.cols(), CvType.CV_8UC3);
	    	matresBW = new Mat(matIori_tmp.rows(), matIori_tmp.cols(), CvType.CV_8UC1);
	    	
	    	int[] flag = OtsuSegmentation(matGray.getNativeObjAddr(), matIori_tmp.getNativeObjAddr(), matSkin.getNativeObjAddr(), matresSkin.getNativeObjAddr(), matresBW.getNativeObjAddr());
	    	//0: ok. 1: skin + nomole; 2: noskin; 3: nomole + noskin
	    	
	    	if (flag[0] <= 1)
	    	{
				Bitmap res = Bitmap.createBitmap(matIori_tmp.cols(), matIori_tmp.rows(), Bitmap.Config.ARGB_8888);
		        Utils.matToBitmap(matresSkin, res);
				mImageView.setImageBitmap(res);
				mImageView.setVisibility(View.VISIBLE);
				
				if (flag[0] == 0)
				{
//					String abc = "";
					//feature_arr = ColorFeature(matIori_tmp.getNativeObjAddr(), matIb.getNativeObjAddr());
				
					feature_arr = ColorFeature(matIori_tmp.getNativeObjAddr(), matresBW.getNativeObjAddr());
			        
			        //output = output + Double.toString(feature_arr[0]) + " - " + Double.toString(feature_arr[1]) + " - ";
			        //output = output+Double.toString(feature_arr[2]) + " - " + Double.toString(feature_arr[3]);
			        //mTextView.append(output);
			       
			        double score = SVM_predict();
			        
			        //String output = "Result: "+ "SVM score: " + Double.toString(score);
			        double score_post = post_process_score(score);
			        String output = "Risk score: " + Integer.toString((int)(score_post));
			        mTextView.setText(output);
			        if (score_post <= 35)
			        	mTextView.append("\n" + recommend1); //normal mode
			        else
			        {
			        	if(score_post <=70)
			        		mTextView.append("\n" + recommend2);//clinic
			        	else
			        		mTextView.append("\n" + recommend3);//strong clinic 
			        }
			        mTextView.append("\nFINISH!");
			       
				}
				else
				{
					mTextView.setText("No mole is detected!");
				}
				
	    	}
	    	else
	    	{
	    		mTextView.setText("No skin region is detected!");
	    	}
//	        matIori.release();
//	        matIori_tmp.release();
//	        matIb.release();
//	        matGray.release();
//	        matresSkin.release();
//	        matresBW.release();
//        	my_SVM2.clear();
	   
//			mTextView.setText("dang test!");
		}
		else
		{
			mTextView.setText("Results: Please load or capture an image.");
		}
	}

	///////
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch (requestCode) 
		{
			case ACTION_TAKE_PHOTO_B: 
			{
				if (resultCode == RESULT_OK) 
				{
					handleBigCameraPhoto();// khong co tham so intent --> anh duoc luu tam khi chup. qua trinh xu ly se 
					// load anh len
				}
				break;
			} // ACTION_TAKE_PHOTO_B
	
			case ACTION_TAKE_PHOTO_S: 
			{
				if (resultCode == RESULT_OK) 
				{
					handleSmallCameraPhoto(data); // anh duoc luu trong extra field cua intent data
				}
				break;
			} // ACTION_TAKE_PHOTO_S
	
//			case ACTION_TAKE_VIDEO: {
//				if (resultCode == RESULT_OK) {
//					handleCameraVideo(data);
//				}
//				break;
//				
//			} // ACTION_TAKE_VIDEO
			case ACTION_LOAD_IMAGE: 
			{
				if (resultCode == RESULT_OK)
				{
					handleLoadImage(data);
				}
				break;
			}// ACTION_LOAD_IMAGE
			case ACTION_DIAGNOSIS: 
			{
				if (resultCode == RESULT_OK)
				{
					handleDiagnosis();
				}
				break;
			}
		}
	}
	private void scale_bitmap()
	{
		int H = mImageBitmap.getHeight();
		int W = mImageBitmap.getWidth();
		if (Math.max(H, W) > MAXSIZE)
		{
			double scale = (1.0*MAXSIZE)/Math.max(H, W);
			int Hnew = (int)Math.round(H*scale);
			int Wnew = (int)Math.round(W*scale);
			mImageBitmap = Bitmap.createScaledBitmap(mImageBitmap, Wnew, Hnew, false);
		}
	}
	private void setPic_1()
	{
		BitmapFactory.Options bmOptions = new BitmapFactory.Options();
		bmOptions.inJustDecodeBounds = true;//khong return bitmap ma chi lay cac thong so cua bitmap
		BitmapFactory.decodeFile(mCurrentPhotoPath, bmOptions);
		int photoW = bmOptions.outWidth;
		int photoH = bmOptions.outHeight;
//		double scaleFactor = (1.0*MAXSIZE)/Math.max(photoH, photoW);
		double scaleFactor = Math.max(photoW/MAXSIZE, photoH/MAXSIZE);
		// Set bitmap options to scale the image decode target */
		bmOptions.inJustDecodeBounds = false;
		bmOptions.inSampleSize = (int) scaleFactor;//scale = k -->giam H den 1/K, W den 1/K
		//bmOptions.inPurgeable = true;
		bmOptions.inPreferredConfig = Bitmap.Config.ARGB_8888;
		mImageBitmap = BitmapFactory.decodeFile(mCurrentPhotoPath, bmOptions);//luc nay moi load bitmap
		mImageView.setImageBitmap(mImageBitmap);
//		mVideoUri = null;
		mImageView.setVisibility(View.VISIBLE);
	}
	private void setPic() //giam kich thuoc anh de giam memory khi view.
	{
		/* There isn't enough memory to open up more than a couple camera photos */
		/* So pre-scale the target bitmap into which the file is decoded */

		/* Get the size of the ImageView */
//		int targetW = mImageView.getWidth();
//		int targetH = mImageView.getHeight();
		//mycode
		int targetW = targetW_ori; 
		int targetH = targetH_ori; 
		
//		BitmapFactory.Options options = new BitmapFactory.Options();
//		options.inPreferredConfig = Bitmap.Config.ARGB_8888;
		
	    //
		/* Get the size of the image */
		BitmapFactory.Options bmOptions = new BitmapFactory.Options();
//		bmOptions.inJustDecodeBounds = true;
//		BitmapFactory.decodeFile(mCurrentPhotoPath, bmOptions);
//		int photoW = bmOptions.outWidth;
//		int photoH = bmOptions.outHeight;
//		
//		/* Figure out which way needs to be reduced less */
//		int scaleFactor = 1;
		//ko scale 
//		if ((targetW > 0) || (targetH > 0)) {
//			scaleFactor = Math.min(photoW/targetW, photoH/targetH);	
//		}

		/* Set bitmap options to scale the image decode target */
//		bmOptions.inJustDecodeBounds = false;
//		bmOptions.inSampleSize = scaleFactor;
//		bmOptions.inPurgeable = true;
		bmOptions.inPreferredConfig = Bitmap.Config.ARGB_8888;
		/* Decode the JPEG file into a Bitmap */
		mImageBitmap = BitmapFactory.decodeFile(mCurrentPhotoPath, bmOptions);
		
		scale_bitmap();
		
		/* Associate the Bitmap to the ImageView */
		mImageView.setImageBitmap(mImageBitmap);
//		mVideoUri = null;
		mImageView.setVisibility(View.VISIBLE);
//		mVideoView.setVisibility(View.INVISIBLE);
	}
	private void galleryAddPic() //load lai anh tu mCurrentPhotoPath va save voi original size, ko lien quan toi mImageBitmap
	{
// luu file dung write, dung cach nay co the luu png thoai mai		
		BitmapFactory.Options bmOptions = new BitmapFactory.Options();
		
		//////////////////////////////////////////////
		//lay thong tin bitmap de scale 
		bmOptions.inJustDecodeBounds = true;//khong return bitmap ma chi lay cac thong so cua bitmap
		BitmapFactory.decodeFile(mCurrentPhotoPath, bmOptions);
		int photoW = bmOptions.outWidth;
		int photoH = bmOptions.outHeight;
		//compute scale factor	
		double scaleFactor = Math.max(photoW/MAXSIZE, photoH/MAXSIZE);
		// Set bitmap options to scale the image decode target */
		bmOptions.inJustDecodeBounds = false;
		bmOptions.inSampleSize = (int) scaleFactor;//scale = k -->giam H den 1/K, W den 1/K
		//////////////////////////////////////////////
		
		bmOptions.inPreferredConfig = Bitmap.Config.ARGB_8888;
//		bmOptions.inJustDecodeBounds = false;
//		bmOptions.inPurgeable = true;
		Bitmap bitmap = BitmapFactory.decodeFile(mCurrentPhotoPath, bmOptions);
		ByteArrayOutputStream outputBuffer = new ByteArrayOutputStream();
		bitmap.compress(Bitmap.CompressFormat.JPEG, 100, outputBuffer);
	    byte[] byteImage1=outputBuffer.toByteArray();
	    //save file to internal storage
	    try {
	      FileOutputStream outputStream = new FileOutputStream(mCurrentPhotoPath);
	      outputStream.write(byteImage1);
	      outputStream.flush();
	      outputStream.close();
//	      By using this line you can able to see saved images in the gallery view.
//	      neu muon nhanh hon chi can mount trong folder MyPictures?
	      sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED,Uri.parse("file://" + Environment.getExternalStorageDirectory())));
	      mCurrentPhotoPath = null;
	    } catch (Exception e) {
	      e.printStackTrace();
	    }
	}
	
	private void handleBigCameraPhoto() //anh duoc chup da nam trong Uri
	{
		if (mCurrentPhotoPath != null) {
			setPic();
			//setPic_1();
			galleryAddPic();
			mCurrentPhotoPath = null;
			mTextView.setText("");
		}
	}
	private void handleSmallCameraPhoto(Intent intent) {
		Bundle extras = intent.getExtras();
		mImageBitmap = (Bitmap) extras.get("data");
//		int h = mImageBitmap.getHeight(); // 120
//		int w = mImageBitmap.getWidth();  // 160
		mImageView.setMinimumHeight(mImageBitmap.getHeight());
		mImageView.setMinimumWidth(mImageBitmap.getWidth());
		mImageView.setImageBitmap(mImageBitmap);
//		mVideoUri = null;
		mImageView.setVisibility(View.VISIBLE);
//		mVideoView.setVisibility(View.INVISIBLE);
		
//		save file su dung write -->OK
		ByteArrayOutputStream outputBuffer = new ByteArrayOutputStream();
		mImageBitmap.compress(Bitmap.CompressFormat.JPEG, 100, outputBuffer);
	    byte[] byteImage1=outputBuffer.toByteArray();
	    try {
	      FileOutputStream outputStream = new FileOutputStream(mCurrentPhotoPath);
	      outputStream.write(byteImage1);
	      outputStream.flush();
	      outputStream.close();
//	      By using this line you can able to see saved images in the gallery view.
//	      neu muon nhanh hon chi can mount trong folder MyPictures?
	      sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED,Uri.parse("file://" + Environment.getExternalStorageDirectory())));
	      mCurrentPhotoPath = null;
	    } catch (Exception e) {
	      e.printStackTrace();
	    }
	}
//	private void handleCameraVideo(Intent intent) {
//		mVideoUri = intent.getData();
//		mVideoView.setVideoURI(mVideoUri);
//		mImageBitmap = null;
//		mVideoView.setVisibility(View.VISIBLE);
//		mImageView.setVisibility(View.INVISIBLE);
//	}

    //mycode: handle load image
	private void handleLoadImage(Intent intent){
		Uri selectedImage = intent.getData();
		String[] filePathColumn = { MediaStore.Images.Media.DATA };
		Cursor cursor = getContentResolver().query(selectedImage,filePathColumn, null, null, null);
		cursor.moveToFirst();
		int columnIndex = cursor.getColumnIndex(filePathColumn[0]);
		String picturePath = cursor.getString(columnIndex);
		cursor.close();
		
		BitmapFactory.Options options = new BitmapFactory.Options();
		mImageBitmap = null;
		/////////////////////////////////////////////////////////////////
		//get size of bitmap
		options.inJustDecodeBounds = true;//khong return bitmap ma chi lay cac thong so cua bitmap
		BitmapFactory.decodeFile(picturePath, options);
		int photoW = options.outWidth;
		int photoH = options.outHeight;
		//compute scale factor	
		double scaleFactor = Math.max(photoW/MAXSIZE, photoH/MAXSIZE);
		// Set bitmap options to scale the image decode target */
		options.inJustDecodeBounds = false;
		options.inSampleSize = (int) scaleFactor;//scale = k -->giam H den 1/K, W den 1/K
		///////////////////////////////////////////////////////////////
		options.inPreferredConfig = Bitmap.Config.ARGB_8888;
		mImageBitmap = BitmapFactory.decodeFile(picturePath,options);
		scale_bitmap(); //scale bitmap to 512
		mImageView.setImageBitmap(mImageBitmap);
		
//		mVideoUri = null;
		mImageView.setVisibility(View.VISIBLE);
//		mVideoView.setVisibility(View.INVISIBLE);
		mTextView.setText("");
	}
	private void handleDiagnosis()
	{
		
	}
	//////////////////////////////////////////
	Button.OnClickListener mTakePicOnClickListener = 
		new Button.OnClickListener() {
		@Override
		public void onClick(View v) {
			dispatchTakePictureIntent(ACTION_TAKE_PHOTO_B);
		}
	};

	Button.OnClickListener mTakePicSOnClickListener = 
		new Button.OnClickListener() {
		@Override
		public void onClick(View v) {
			dispatchTakePictureIntent(ACTION_TAKE_PHOTO_S);
		}
	};

//	Button.OnClickListener mTakeVidOnClickListener = 
//		new Button.OnClickListener() {
//		@Override
//		public void onClick(View v) {
//			dispatchTakeVideoIntent();
//		}
//	};
	//my code processing load image button
	Button.OnClickListener mLoadPicOnClickListener = 
			new Button.OnClickListener() {
			@Override
			public void onClick(View v) {
				dispatchLoadPictureIntent();
			}
		};
	Button.OnClickListener mDiagOnClickListener = 
			new Button.OnClickListener() {
			@Override
			public void onClick(View v) {
//				dispatchDiagnosisIntent();
				processImage();
			}
		};
	///
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
//		
//		BaseLoaderCallback  mLoaderCallback = new BaseLoaderCallback(this) {
//	        @Override
//	        public void onManagerConnected(int status) {
//	            switch (status) {
//	                case LoaderCallbackInterface.SUCCESS:
//	                {
//	                    Log.i(TAG, "OpenCV loaded successfully");
//	                    // Load native library after(!) OpenCV initialization
////	                    System.loadLibrary("CancerDiagnosis");
////	                    setContentView(R.layout.main);
////	                    Button t = (Button)findViewById(R.id.btnLoadPicture);
////	                    t.setText("loaded_OCV");
//	                } break;
//	                default:
//	                {
//	                    super.onManagerConnected(status);
//	                } break;
//	            }
//	        }
//	    };
//		
		setContentView(R.layout.main);

		Log.i(TAG, "Trying to load OpenCV library");
		if (!OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_6, this, mLoaderCallback))
		{
		  Log.e(TAG, "Cannot connect to OpenCV Manager");
		}
		
		mImageView = (ImageView) findViewById(R.id.imageView1);
		mTextView = (TextView)findViewById(R.id.textView1); 
		//mycode: luu kich thuoc imageView luc khoi tao
		targetW_ori = mImageView.getWidth(); // 0
		targetH_ori = mImageView.getHeight(); // 0
		//////////////
//		mVideoView = (VideoView) findViewById(R.id.videoView1);
//		mVideoUri = null;
		mImageBitmap = null;
		mImageBitmap_scaled = null;
		Button picBtn = (Button) findViewById(R.id.btnIntend);
		setBtnListenerOrDisable(picBtn, mTakePicOnClickListener,MediaStore.ACTION_IMAGE_CAPTURE);

//		Button picSBtn = (Button) findViewById(R.id.btnIntendS);
//		setBtnListenerOrDisable( picSBtn, mTakePicSOnClickListener,MediaStore.ACTION_IMAGE_CAPTURE);

//		Button vidBtn = (Button) findViewById(R.id.btnIntendV);
//		setBtnListenerOrDisable( 
//				vidBtn, 
//				mTakeVidOnClickListener,
//				MediaStore.ACTION_VIDEO_CAPTURE
//		);
		//my code: select image from gallery
		Button loadBtn = (Button) findViewById(R.id.btnLoadPicture);
		// khong can kiem tra setBtnListenerOrDisable ma set button Listener luon
		loadBtn.setOnClickListener(mLoadPicOnClickListener);  		
//		setBtnListenerOrDisable( 
//				loadBtn, 
//				mLoadPicOnClickListener,
//				MediaStore.Images.Media.EXTERNAL_CONTENT_URI
//		);
		Button DiagBtn = (Button) findViewById(R.id.btnDiagnosis);
		DiagBtn.setOnClickListener(mDiagOnClickListener);
		///////////////////////////////////
		
		mAlbumStorageDirFactory = new FroyoAlbumDirFactory();
//		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.FROYO) {
//			mAlbumStorageDirFactory = new FroyoAlbumDirFactory();
//		} else {
//			mAlbumStorageDirFactory = new BaseAlbumDirFactory();
//		}
		
	}

	// Some lifecycle callbacks so that the image can survive orientation change
	@Override
	protected void onSaveInstanceState(Bundle outState) {
		outState.putParcelable(BITMAP_STORAGE_KEY, mImageBitmap);
//		outState.putParcelable(VIDEO_STORAGE_KEY, mVideoUri);
		outState.putBoolean(IMAGEVIEW_VISIBILITY_STORAGE_KEY, (mImageBitmap != null) );
//		outState.putBoolean(VIDEOVIEW_VISIBILITY_STORAGE_KEY, (mVideoUri != null) );
		super.onSaveInstanceState(outState);
	}

	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		super.onRestoreInstanceState(savedInstanceState);
		mImageBitmap = savedInstanceState.getParcelable(BITMAP_STORAGE_KEY);
//		mVideoUri = savedInstanceState.getParcelable(VIDEO_STORAGE_KEY);
		mImageView.setImageBitmap(mImageBitmap);
		mImageView.setVisibility(
				savedInstanceState.getBoolean(IMAGEVIEW_VISIBILITY_STORAGE_KEY) ? 
						ImageView.VISIBLE : ImageView.INVISIBLE
		);
//		mVideoView.setVideoURI(mVideoUri);
//		mVideoView.setVisibility(
//				savedInstanceState.getBoolean(VIDEOVIEW_VISIBILITY_STORAGE_KEY) ? 
//						ImageView.VISIBLE : ImageView.INVISIBLE
//		);
	}

	/**
	 * Indicates whether the specified action can be used as an intent. This
	 * method queries the package manager for installed packages that can
	 * respond to an intent with the specified action. If no suitable package is
	 * found, this method returns false.
	 * http://android-developers.blogspot.com/2009/01/can-i-use-this-intent.html
	 *
	 * @param context The application's environment.
	 * @param action The Intent action to check for availability.
	 *
	 * @return True if an Intent with the specified action can be sent and
	 *         responded to, false otherwise.
	 */
	public static boolean isIntentAvailable(Context context, String action) {
		final PackageManager packageManager = context.getPackageManager();
		final Intent intent = new Intent(action);
		List<ResolveInfo> list =
			packageManager.queryIntentActivities(intent,
					PackageManager.MATCH_DEFAULT_ONLY);
		return list.size() > 0;
	}

	private void setBtnListenerOrDisable( Button btn, Button.OnClickListener onClickListener,String intentName) 
	{
		if (isIntentAvailable(this, intentName)) {
			btn.setOnClickListener(onClickListener);        	
		} else {
			btn.setText( 
				getText(R.string.cannot).toString() + " " + btn.getText());
			btn.setClickable(false);
		}
	}
	private native void debugMessage();
	public native double[] ColorFeature(long matAddrBgr,long matAddrBinary);
	public native void FindFeatures(long matAddrBrg);
	public native int[] OtsuSegmentation(long matAddrGray, long matAddrRgb, long matAddrSkin, long matoutAddrskin, long matAddrbw);

}