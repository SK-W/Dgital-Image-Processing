#pragma once
#include <opencv.hpp>


class My_Image_Process
{
public:
	My_Image_Process() {};
	~My_Image_Process() {};

public:
	// 直方图均衡化
	void equalize_hist(cv::Mat src, cv::Mat &dst);
	void sample_equalize_hist();
	
	// 直方图规定化 src1->src2
	void equalize_hist_match(cv::Mat src1, cv::Mat src2, cv::Mat& dst);
	void sample_equalize_hist_match();




	


private:

};

