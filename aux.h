#ifndef AUX_H
#define AUX_H

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class Aux{
	public:
		Aux();
		virtual ~Aux();
		void showImg(char**);
		void filter();
		void kmeans();
		void histogram();
		void equalize();
		
	private:
		
};

#endif
