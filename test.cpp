#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

const int cam = 0;

Mat src, src_gray;
Mat dst, detected_edges, detected_circles;

vector<Vec3f> circles;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
string window_name = "Edge Map";
int bRadius;

void ContourDetect(int, void*)
{
	//vector<vector<Point>> contours;
	vector<Vec4i> heirarchy;
	//findCountours(src_gray,countours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
}

void CannyThreshold(int, void*)
{
	int r = (kernel_size*2)+1;
	threshold(src_gray,src_gray,150,0,THRESH_TRUNC);
	threshold(src_gray,src_gray,50,0,THRESH_TOZERO);
	blur( src_gray, detected_edges, Size(r,r) );
	Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio,3 );
	dst = Scalar::all(0);
	src.copyTo( dst, detected_edges);
	imshow( window_name, dst );
}

/*
void HoughCircleDetect(int, void*)
{
	blur(src_gray, detected_circles, Size(5,5));
	HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT,1,src_gray.rows/8,200,100,0,0);
	for(size_t i = 0; i < circles.size(); i++) {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    	int radius = cvRound(circles[i][2]);
    	circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
    	circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
	}
	imshow( window_name, src);
}
*/

int main()
{
	namedWindow( window_name, CV_WINDOW_AUTOSIZE );
	createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
	createTrackbar( "Blur radius:", window_name, &kernel_size, 20, CannyThreshold);
	if(cam)
	{
		VideoCapture cap(0);
		cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
		while(1)
		{
			cap.read(src);
			dst.create( src.size(), src.type() );
			cvtColor( src, src_gray, CV_BGR2GRAY );
			CannyThreshold(0, 0);
			if(waitKey(30)==27)
			{
				break;
			}
		}
	}
	else
	{
		src = imread("test_images/1.png",1);
		dst.create(src.size(),src.type());
		cvtColor(src,src_gray,CV_BGR2GRAY);
		CannyThreshold(0,0);
		waitKey(0);
	}	
  return 0;
  }
