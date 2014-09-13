#include <jni.h>
#include <Factory.h>
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <android/log.h>
#include <math.h>
#define LOG    "ffmpegDemo-jni" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG,__VA_ARGS__) // 定义LOGF类型
using namespace cv;
bool isFirst = true;
CvPoint2D32f * points = new CvPoint2D32f[60];
IplImage iplImg;
IplImage *iplImgc;
IplImage *iplImgpyrA,*iplImgpyrB;
IplImage *iplGrayA;
IplImage *iplGrayB;
IplImage *iplGrayC;
IplImage *iplGrayD;
int level = 3;
char status[60];
float track_error[60];
CvPoint2D32f * pointsc = new CvPoint2D32f[60];
int count = 50;
CvSize winSize;
int singleWidth;
int singleHeight;
float distance;
int trackedNum;
CvPoint2D32f direction;
float dis(CvPoint2D32f a,CvPoint2D32f b){
	return (float)sqrt((a.x*3-b.x*3)*(a.x*3-b.x*3)+(a.y*3-b.y*3)*(a.y*3-b.y*3));
}

JNIEXPORT void JNICALL Java_org_zjicm_aicamera_Factory_changeImage(JNIEnv *, jclass, jlong matAddbf){
	Mat matAddaf = *(Mat*)matAddbf;
	iplImg = matAddaf;
	//if(ifface==1)
	//cvCircle(&iplImg,cvPoint(960+(faceX/1000.0*960.0),540+(faceY/1000.0*540.0)),5,cvScalar(0,255,255),-1);
	if(isFirst){
		iplGrayA = cvCreateImage(cvSize(320,180),IPL_DEPTH_8U,1);
		iplGrayC = cvCreateImage(cvGetSize(&iplImg),IPL_DEPTH_8U,1);
		iplGrayD = cvCreateImage(cvGetSize(&iplImg),IPL_DEPTH_8U,1);
		iplGrayB = cvCreateImage(cvSize(320,180),IPL_DEPTH_8U,1); //单通道灰度图，用于光流跟踪
		CvSize pyr_sz = cvSize(iplImg.width+8,iplImg.height/3);
		iplImgpyrA = cvCreateImage(pyr_sz,IPL_DEPTH_32F,1);
		iplImgpyrB = cvCreateImage(pyr_sz,IPL_DEPTH_32F,1);
	}
	cvCopy(iplGrayB,iplGrayA);
	cvCvtColor(&iplImg,iplGrayC,CV_BGR2GRAY);
	//cvSmooth(iplGrayD,iplGrayC,CV_MEDIAN,3,3);
	cvResize(iplGrayC,iplGrayB,CV_INTER_AREA);

	//IplImage *tmpIpl = cvCreateImage(cvGetSize(&iplImg),IPL_DEPTH_8U,1);
	//cvCopy(iplGrayB,tmpIpl);
	//cvSmooth(tmpIpl,iplGrayB,CV_BLUR_NO_SCALE);
	//cvReleaseImage(&tmpIpl);


	//cvCvtColor(iplGray,&iplImg,CV_GRAY2BGR);      /*检测成功灰度图   iplgray是有效的*/
	//cvCircle(&iplImg,cvPoint(20,20),3,cvScalar(255,0,0),-1);
	if(isFirst){

		int width = iplGrayB->width;
		int height = iplGrayB->height;
		winSize = cvGetSize(&iplImg);
		singleWidth = width/10;
		singleHeight = height/6;
		int len=0;
		for(int i=0;i<5;i++)
		{
			for(int j=0;j<10;j++)
			{
				points[i*10+j].x=singleWidth/2+j*singleWidth;
				points[i*10+j].y=singleHeight/2+i*singleHeight;
			}
		}
	}

	/*for(int i=0;i<6;i++)
	{
		for(int j=0;j<10;j++)
		{
			cvCircle(&iplImg,cvPointFrom32f(points[i*10+j]),2,cvScalar(255,0,0),-1);
		}
	}*/
	if(!isFirst){

		cvCalcOpticalFlowPyrLK(
				iplGrayA,
				iplGrayB,
				iplImgpyrA,
				iplImgpyrB,
				points,
				pointsc,
				count,
				cvSize(15,15),
				level,
				status,
				track_error,
				cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.3),
				0
		);
		trackedNum=0;
		direction.x=0;
		direction.y=0;     //init direction for the following sum
		for(int i=0;i<5;i++)
			for(int j=0;j<10;j++)
			{

				cvCircle(&iplImg,cvPoint(cvPointFrom32f(points[i*10+j]).x*3,cvPointFrom32f(points[i*10+j]).y*3),2,cvScalar(255,255,0),-1);
				distance=dis(points[i*10+j],pointsc[i*10+j]);
				if(status[i*10+j]==1
						&&distance<96&&distance>15
				){
					direction.x+=(pointsc[i*10+j].x-points[i*10+j].x);
					direction.y+=(pointsc[i*10+j].y-points[i*10+j].y);
					trackedNum++;
					cvLine(&iplImg,cvPoint(cvPointFrom32f(points[i*10+j]).x*3,cvPointFrom32f(points[i*10+j]).y*3),cvPoint(cvPointFrom32f(pointsc[i*10+j]).x*3,cvPointFrom32f(pointsc[i*10+j]).y*3),CV_RGB(255,255,0),2);
					cvCircle(&iplImg,cvPoint(cvPointFrom32f(pointsc[i*10+j]).x*3,cvPointFrom32f(pointsc[i*10+j]).y*3),2,cvScalar(255,255,0),-1);
				}
				//points[i*10+j]=pointsc[i*10+j];
			}
		direction.x/=trackedNum;
		direction.y/=trackedNum;
		 __android_log_print(ANDROID_LOG_INFO, "Direction", "direction:  %f   %f ", direction.x,direction.y);

		//Draw the routes
	}
	/*for(int i=0;i<matAddaf.rows;i++)
	{
		int *p = (int*)(matAddaf.datastart+i*matAddaf.step);
		for(int j=0;j<matAddaf.cols;j++)
		{
				//if(j%3==0) continue;

				int tmp = *p;
				//LOGI(LOG,"%d",tmp);
				*p=255-*p;
				p++;

		}
	}*/
	//return matAddbf;
	if(isFirst)
	isFirst = false;
}


