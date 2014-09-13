package org.zjicm.aicamera;

import android.hardware.Camera;
import android.hardware.Camera.Face;
import android.hardware.Camera.FaceDetectionListener;
import android.os.Handler;
import android.os.Message;
import android.util.Log;


public class MyFaceDetectionListener implements Camera.FaceDetectionListener {
	public static final int FACEDETECTED = 0;
	public static final int FACENOTDETECTED = 1;
	public static final int CAMERASTARTED = 2;
	Handler handler;
	public MyFaceDetectionListener(Handler handler){
		this.handler = handler;
	}
	@Override
	
	public void onFaceDetection(Face[] faces, Camera camera) {
		// TODO Auto-generated method stub
		//Message msg = handler.obtainMessage();
		if(faces.length>0){
			Log.i("info", "x:"+faces[0].rect.centerX());
			//msg.obj=faces[0];
			//msg.what = FACEDETECTED;
			//msg.sendToTarget();
		}
		else{
			//msg.what = FACENOTDETECTED;
			//msg.sendToTarget();
		}
	}

}