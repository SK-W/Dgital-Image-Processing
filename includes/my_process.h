#pragma once
#include <opencv.hpp>


class My_Image_Process
{
public:
	My_Image_Process() {};
	~My_Image_Process() {};

public:
	// ֱ��ͼ���⻯
	void equalize_hist(cv::Mat src, cv::Mat &dst);
	void sample_equalize_hist();


private:

};

