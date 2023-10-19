#include "my_process.h"
#include <vector>

void My_Image_Process::equalize_hist(cv::Mat src, cv::Mat& dst)
{
	// 灰度直方图均衡化
	// 省略一切检查措施，直接运行

	std::vector<int> pdf(256,0); // 建立概率密度函数，数量
	std::vector<int> cdf(256,0); // 建立累积分布函数，数量

	// 统计
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			uchar pixel = src.at<uchar>(i, j);
			pdf[static_cast<int>(pixel)]++;
		}
	}
	// 累积
	int number = 0;
	for (int i = 0; i < pdf.size(); i++)
	{
		number += pdf[i];
		cdf[i] = number;
	}
	// 映射函数
	std::vector<int> pixel_map(256, 0);
	for (int i = 0; i < cdf.size(); i++)
	{
		pixel_map[i] = 256 * cdf[i] / (src.rows * src.cols);
	}

	// 图像映射
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			uchar pixel = src.at<uchar>(i, j);
			dst.at<uchar>(i, j) = pixel_map[static_cast<int>(pixel)];
		}
	}

}

void My_Image_Process::sample_equalize_hist()
{
	cv::Mat src = cv::imread("test_images/hist_test.jpg", 0);
	cv::Mat dst1, dst2;
	dst1.create(src.size(), src.type());
	dst2.create(src.size(), src.type());

	equalize_hist(src, dst1);
	cv::equalizeHist(src, dst2);

	cv::imshow("src", src);
	cv::imshow("my_process", dst1);
	cv::imshow("cv_process", dst2);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
