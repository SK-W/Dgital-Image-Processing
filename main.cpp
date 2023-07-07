#include <iostream>
#include <opencv.hpp>

int main() 
{
	std::cout << "ss" << std::endl;S
	cv::Mat src = cv::imread("test_images/star.jpg");
	cv::imshow("src", src);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}