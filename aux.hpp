#ifndef AUX_HPP
#define AUX_HPP

#include <opencv2/opencv.hpp>
#include <iostream>


class Aux{
	public:
		Aux();
		virtual ~Aux();
		void showImg(char**){
			cv::Mat image;
		  	image = cv::imread("wolf.png",CV_LOAD_IMAGE_GRAYSCALE);
			if(!image.data)
		   		std::cout << "nao abriu" << std::endl;
			cv::imshow("janela", image);  
		  	cv::waitKey();
		}
		void filter();
		void kmeans();
		void histogram();
		void equalize();
		
	private:
		
};

#endif
