#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 3
#define RAIO 3
int top_slider = 5;
int top_slider_max = 200;

char TrackbarName[50];

Mat image, border, frame, points, join;

void on_trackbar_canny(int, void*){
	Canny(image, border, top_slider, 3*top_slider);
	imshow("cannyborders", border);
	
}
int main(int argc, char** argv){
	
	int width, height;

	image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

	width=image.size().width;
	height=image.size().height;

	sprintf( TrackbarName, "Threshold inferior", top_slider_max );

	namedWindow("cannyborders",1);
	
	createTrackbar( TrackbarName, "cannyborders",
		&top_slider,
		top_slider_max,
		on_trackbar_canny );
	
	on_trackbar_canny(top_slider, 0 );
	waitKey();
	vector<int> yrange;
	vector<int> xrange;

	int gray;
	int x, y;

	srand(time(0));

	if(!image.data){
		cout << "nao abriu" << argv[1] << endl;
		cout << argv[0] << " imagem.jpg";
		exit(0);
	}
	xrange.resize(height/STEP);
	yrange.resize(width/STEP);

	iota(xrange.begin(), xrange.end(), 0); 
	iota(yrange.begin(), yrange.end(), 0);

	for(uint i=0; i<xrange.size(); i++){
		xrange[i]= xrange[i]*STEP+STEP/4;
	}

	for(uint i=0; i<yrange.size(); i++){
		yrange[i]= yrange[i]*STEP+STEP/4;
	}

	points = Mat(height, width, CV_8U, Scalar(255));

	random_shuffle(xrange.begin(), xrange.end());

	for(auto i : xrange){
		random_shuffle(yrange.begin(), yrange.end());
		for(auto j : yrange){
			x = i+rand()%(1*JITTER)-JITTER+1;
			y = j+rand()%(1*JITTER)-JITTER+1;
			gray = image.at<uchar>(x,y);
			circle(points,
			     cv::Point(y,x),
			     RAIO,
			     CV_RGB(gray,gray,gray),
			     -1,
			     CV_AA);
		}
	}
	join = points + border;
	
	imwrite("cannypontos.jpg", points);
	imwrite("cannypontos2.jpg", border);
	imwrite("cannypontosjoin.jpg", join);
	return 0;
}
