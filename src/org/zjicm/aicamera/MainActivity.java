package org.zjicm.aicamera;
import java.util.*;

import android.hardware.Camera.Face;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.app.Activity;
import android.graphics.Path;
import android.hardware.Camera;
import android.hardware.Camera.Face;
import android.util.Log;
import android.view.*;
import android.widget.FrameLayout;

import org.opencv.android.*;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener;
import org.opencv.core.CvType;
import org.opencv.core.Mat;

public class MainActivity extends Activity implements CvCameraViewListener,View.OnTouchListener{
	private static final String TAG = "MainActivity";
	private mCameraView mCvCameraView;
	private int mCameraHeight;
	private int mCameraWidth;
	private Face mFace;
	private int ifface=0;
	
	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {

        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
                    System.loadLibrary("Camera");
                    /* Now enable camera view to start receiving frames */
                    mCvCameraView.setOnTouchListener(MainActivity.this);
                    mCvCameraView.enableView();
                    
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//OpenCVLoader.initDebug();
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		Log.d(TAG,"Creating and Setting the View");
		Handler handler = new Handler(){
			 public void handleMessage(Message msg) {   
	              switch (msg.what) {   
	              	case MyFaceDetectionListener.CAMERASTARTED:
	              		//mCvCameraView.returnCamera().startFaceDetection();
	              	case MyFaceDetectionListener.FACEDETECTED:
	              		mFace = (Face)msg.obj;
	              		ifface=1;
	              		break;
	              	case MyFaceDetectionListener.FACENOTDETECTED:
	              		ifface=0;
	              		break;
	              }   
	              super.handleMessage(msg);   
	         }   
		};
		
		mCvCameraView = new mCameraView(this,-1,handler);
		mCvCameraView.setOnTouchListener(MainActivity.this); 
		mCvCameraView.enableFpsMeter();    
		mCvCameraView.setMaxFrameSize(960, 540);
		mCvCameraView.setVisibility(SurfaceView.VISIBLE);
		handler.sendEmptyMessageDelayed(MyFaceDetectionListener.CAMERASTARTED, 2000);
	    setContentView(mCvCameraView);  
		

		
		
		
		mCvCameraView.setCvCameraViewListener(this);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	public Mat onCameraFrame(Mat inputFrame) {
		// TODO Auto-generated method stub
			Factory.changeImage(inputFrame.getNativeObjAddr());
		
		return inputFrame;
	}

	@Override
	public void onCameraViewStarted(int width, int height) {
		
		mCameraWidth = width;
		mCameraHeight = height;
	}

	@Override
	public void onCameraViewStopped() {
		// TODO Auto-generated method stub
	}

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		// TODO Auto-generated method stub
		return false;
	}
	@Override
	public void onResume()
    {
        super.onResume();
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_6, this, mLoaderCallback);
    }
	@Override
    public void onPause()
    {
        super.onPause();
        if (mCvCameraView != null)
            mCvCameraView.disableView();
    }
	@Override
	public void onDestroy() {
        super.onDestroy();
        if (mCvCameraView != null)
            mCvCameraView.disableView();
    }

	

	
	
}
