#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int top_slider = 5;
int top_slider_max = 200;
int gamL_slider = 4, gamH_slider = 20, shrC_slider = 10, cut_slider = 5;
const int gamL_max = 10, gamH_max = 50, shrC_max = 100, cut_max = 200;
int gammaL, gammaH, sharpC, cutoff;
char TrackbarName[50];

Mat image, p1, p2, padded, imFiltered;

int dft1, dft2;
Mat homomorphicFilter(double yl, double yh, double c, double d0){
	Mat filter = Mat(image.size(), CV_32FC2, Scalar(0));
	Mat tmp = Mat(dft1, dft2, CV_32F);

	for(int i=0; i<dft1; i++){
		for(int j=0; j<dft2; j++){
			//tmp.at<float> (i,j) = (gh - gl)*(1 - exp(-c*(( (i-dft1/2)*(i-dft1/2) + (j-dft2/2)*(j-dft2/2) ) / (d0*d0) ))) + gl;
			float d2 = pow(i - dft1/2.0, 2) + pow(j - dft2/2.0, 2);
			float exp1 = - c*(d2/pow(d0, 2));
			float valor = (yh - yl)*(1 - expf(exp1) ) + yl;
			tmp.at<float> (i,j) = valor;
		}
	}
	Mat comps[]= {tmp,tmp};
	//imshow("Filter", tmp);
	merge(comps, 2, filter);
	return filter;
}
/*void calcHomomorphicFilter() {
    Mat filter = Mat(padded.size(), CV_32FC2, Scalar(0));
    Mat tmp = Mat(dft1, dft2, CV_32F);

    for (int i = 0; i < dft_M; i++) {
        for (int j = 0; j < dft_N; j++) {
            float d2 = pow(i - dft1/2.0, 2) + pow(j - dft2/2.0, 2);
            float exp1 = - (d2/pow(d0, 2));
            float valor = (yh - yl)*(1 - expf(exp1) ) + yl;
            tmp.at<float> (i,j) = valor;
        }
    }

    Mat comps[] = {tmp, tmp};
    merge(comps, 2, filter);

    Mat dftClone = imageDft.clone();

    mulSpectrums(dftClone,filter,dftClone,0);

    deslocaDFT(dftClone);

    idft(dftClone, dftClone);

    vector<Mat> planos;

    split (dftClone, planos);

    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);

    ...
}*/


void deslocaDFT(Mat& image ){
	Mat tmp, A, B, C, D;
	image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
	int cx = image.cols/2;
	int cy = image.rows/2;
	A = image(Rect(0, 0, cx, cy));
	B = image(Rect(cx, 0, cx, cy));
	C = image(Rect(0, cy, cx, cy));
	D = image(Rect(cx, cy, cx, cy));
	A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);
	C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

void applyFilter(void){
	vector<Mat> planos; planos.clear();
	Mat zeros = Mat_<float>::zeros(padded.size());
	Mat realInput = Mat_<float>(padded);
	Mat complex;
	cout<<"running"<<endl;
	realInput += Scalar::all(1);
	log(realInput,realInput);
	//normalize(realInput, realInput, 0, 1, CV_MINMAX);
	//imshow("logimage",realInput);
	planos.push_back(realInput);
	planos.push_back(zeros);
	merge(planos, complex);
	cout<<"running"<<endl;
	dft(complex, complex);
	deslocaDFT(complex);
	resize(complex,complex,padded.size());
	normalize(complex,complex,0,1,CV_MINMAX);
	cout<<"running"<<endl;
	p2 = homomorphicFilter(gammaL,gammaH,sharpC,cutoff);
	cout<<"running"<<endl;
	mulSpectrums(complex,p2,complex,0);
	deslocaDFT(complex);
	idft(complex, complex);
	//normalize(complex, complex, 0, 1, CV_MINMAX);

	planos.clear();
	split(complex, planos);
	exp(planos[0],planos[0]);
	planos[0] -= Scalar::all(1);
	normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
	imFiltered = planos[0].clone();
	imwrite("Homomorphic.jpeg", imFiltered);
}

void on_trackbar(int, void*){
	gammaL = (double) gamL_slider/10;
	gammaH = (double) gamH_slider/10;
	sharpC = (double) shrC_slider;
	cutoff = (double) cut_slider;
	applyFilter();
	imshow("Homomorphic",imFiltered);
	
}






int main(int argc, char* argv[]){
	image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("opened", WINDOW_NORMAL);
	namedWindow("Filter",WINDOW_NORMAL);
	
	imshow("opened",image);
	waitKey();
	dft1 = image.rows;
	dft2 = image.cols;
	copyMakeBorder(image, padded, 0, dft1 - image.rows, 0, dft2 - image.cols, BORDER_CONSTANT, Scalar::all(0));
	imFiltered = padded.clone();
	namedWindow("Homomorphic", WINDOW_NORMAL);
	char TrackbarName[50];
	
	sprintf( TrackbarName, "Gamma L x %d", gamL_max );
	createTrackbar( TrackbarName, "Homomorphic", &gamL_slider, gamL_max, on_trackbar);

	sprintf( TrackbarName, "Gamma H x %d", gamH_max );
	createTrackbar( TrackbarName, "Homomorphic", &gamH_slider, gamH_max, on_trackbar);

	sprintf( TrackbarName, "C x %d", shrC_max );
	createTrackbar( TrackbarName, "Homomorphic", &shrC_slider, shrC_max, on_trackbar);

	sprintf( TrackbarName, "Cutoff Frequency x %d", cut_max );
	createTrackbar( TrackbarName, "Homomorphic", &cut_slider, cut_max, on_trackbar);
	on_trackbar(0,0);
	waitKey(0);
	//imshow("Filter", p2);
}
