#include <iostream>
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

int main(int argv, char** argc)
{
/*  //Set Hue, Saturation, and Values numbers for target
  int iLowH = 164;
  int iHighH = 179;

  int iLowS = 129;
  int iHighS = 223;

  int iLowV = 68;
  int iHighV = 196;    
*/  


  int iLowH = 0;
  int iHighH = 179;

  int iLowS = 138;
  int iHighS = 255;

  int iLowV = 0;
  int iHighV = 255;    
/*
  VideoCapture cap(-1);
    	if(!cap.isOpened())  // check if we succeeded
        {	
	 return -1;
	}
*/  
  Mat frame;
  frame = imread("target.jpg",1);	
  
  
  //while(true)
 {    	

  float t_start = clock();	
  //Mat frame;
  //cap >> frame; 
  Mat imgHSV,imgOriginal;
  //Mat imgOriginal = imread("3.jpg", CV_LOAD_IMAGE_COLOR);

  Size size(350,350);
  resize(frame,imgOriginal,size);

  //namedWindow("Original", 1);
  //resizeWindow("Original", 640, 480);

  cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
  Mat imgThresholded;
  
  //inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);
  inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);
    
  //morphological opening (remove small objects from the foreground)
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

  //morphological closing (fill small holes in the foreground)
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
  //Display image after applying thresholded ranges of Hue, Saturation, and Value
  imshow("Thresholded Image", imgThresholded);

  //
  GaussianBlur(imgThresholded, imgThresholded, Size(9, 9), 2, 2);
  
  vector<Vec3f> circles;
  //cout << imgThresholded.rows << endl;
  
  HoughCircles(imgThresholded, circles, CV_HOUGH_GRADIENT, 1, imgThresholded.rows/8, 200, 50, 0, 0);
  
  for(size_t i = 0; i < circles.size(); i++){
    Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    int radius = cvRound(circles[i][2]);

    circle(imgOriginal, center, 3, Scalar(0, 0, 255), -1, 8, 0);

    circle(imgOriginal, center, radius, Scalar(0, 255, 0), 3, 8, 0);
  }
  
  //namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
  imshow("Hough Circle Transform Demo", imgOriginal);
  //if(waitKey(30)>= 0) break;
  waitKey(0);

  float t_end = clock();
  cout << (t_end - t_start)/(CLOCKS_PER_SEC)*1000 << endl;

 }

  //}
}
