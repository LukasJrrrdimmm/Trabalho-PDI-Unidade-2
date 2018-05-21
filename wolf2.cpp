#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;


int main(){
	Mat image;
  	image = imread("wolf.png",CV_LOAD_IMAGE_GRAYSCALE);
	if(!image.data)
   		cout << "nao abriu" << endl;
	imshow("janela", image);  
  	waitKey();
	for(int i=0;i<image.size().height;i++){
		  for(int j=0;j<image.size().width;j++){
		    	image.at<uchar>(i,j)=255 - image.at<uchar>(i,j);
		  }
	}
	imshow("janela", image);  
  	waitKey();
	image2 = imread("wolf.png",CV_LOAD_IMAGE_GRAYSCALE);
	ref = imread("wolf.png",CV_LOAD_IMAGE_GRAYSCALE);
k = 0;
l = 0;
	for(int i=0;i<image2.size().height;i++){
		  for(int j=0;j<image2.size().width;j++){
		    	
		  }
	}
	
}
