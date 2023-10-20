#include "my_process.h"
#include <vector>

void My_Image_Process::equalize_hist(cv::Mat src, cv::Mat& dst)
{
	// �Ҷ�ֱ��ͼ���⻯
	// ʡ��һ�м���ʩ��ֱ������

	std::vector<int> pdf(256,0); // ���������ܶȺ���������
	std::vector<int> cdf(256,0); // �����ۻ��ֲ�����������

	// ͳ��
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			uchar pixel = src.at<uchar>(i, j);
			pdf[static_cast<int>(pixel)]++;
		}
	}
	// �ۻ�
	int number = 0;
	for (int i = 0; i < pdf.size(); i++)
	{
		number += pdf[i];
		cdf[i] = number;
	}
	// ӳ�亯��
	std::vector<int> pixel_map(256, 0);
	for (int i = 0; i < cdf.size(); i++)
	{
		pixel_map[i] = 256 * cdf[i] / (src.rows * src.cols);
	}

	// ͼ��ӳ��
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

void My_Image_Process::equalize_hist_match(cv::Mat src1, cv::Mat src2, cv::Mat& dst)
{
	// ���������ͨ���������ʡ��һ�к�ʵ���裬Ĭ����ͨ��

	// ͨ������
	cv::Mat channels_src1[3];
	cv::Mat channels_src2[3];

	int h_1 = src1.rows;
	int w_1 = src1.cols;

	int h_2 = src2.rows;
	int w_2 = src2.cols;

	cv::split(src1, channels_src1);
	cv::split(src2, channels_src2);

	// ��ÿ��ͨ���ֱ����ֱ��ͼ�涨��
	for (int channel_idx = 0; channel_idx < 3; channel_idx++)
	{

		cv::Mat channel_src1 = channels_src1[channel_idx];
		cv::Mat channel_src2 = channels_src2[channel_idx];

		// src1  �ۻ��ֲ�
		std::vector<double> pdf_src1(256, 0); // ���������ܶȺ���������
		std::vector<double> cdf_src1(256, 0); // �����ۻ��ֲ�����������

		// ͳ��
		for (int i = 0; i < h_1; i++)
		{
			for (int j = 0; j < w_1; j++)
			{
				uchar pixel = channel_src1.at<uchar>(i, j);
				pdf_src1[static_cast<int>(pixel)]+= (1.0/h_1/w_1);
			}
		}
		// �ۻ�
		double cdf_src1_counter = 0.0;
		for (int i = 0; i < pdf_src1.size(); i++)
		{
			cdf_src1_counter += pdf_src1[i];
			cdf_src1[i] = cdf_src1_counter;
		}

		// src2  �ۻ��ֲ�
		std::vector<double> pdf_src2(256, 0); // ���������ܶȺ���������
		std::vector<double> cdf_src2(256, 0); // �����ۻ��ֲ�����������

		// ͳ��
		for (int i = 0; i < h_2; i++)
		{
			for (int j = 0; j < w_2; j++)
			{
				uchar pixel = channel_src2.at<uchar>(i, j);
				pdf_src2[static_cast<int>(pixel)] += (1.0 / h_2 / w_2);
			}
		}
		// �ۻ�
		double cdf_src2_counter = 0.0;
		for (int i = 0; i < pdf_src2.size(); i++)
		{
			cdf_src2_counter += pdf_src2[i];
			cdf_src2[i] = cdf_src2_counter;
		}

		// Ѱ����ӽ���ӳ��

		// ����ӳ���б�
		std::vector<int> pixel_map(256, 0);
		for (int i = 0; i < cdf_src1.size(); i++)
		{
			for (int j = 0; j < cdf_src2.size(); j++)
			{
				//std::cout << "i: "<<i << "j: "<<j << " src1 : " << cdf_src1[i] << "src2 : " << cdf_src2[j] << std::endl;
				// ��С������
				if (cdf_src1[i]>cdf_src2[j])
				{
					// �Ƚ���С��һ��
					if (j==0)
					{
						pixel_map[i] = 256 * cdf_src2[j];
					}
					else
					{
						if (std::abs(cdf_src1[i] - cdf_src2[j])> std::abs(cdf_src1[i] - cdf_src2[j-1]))
						{
							pixel_map[i] =j-1;
						}
						else
						{
							pixel_map[i] = j;
						}
					}
					//break;
				}
			}
		}
		// ͼ��ӳ��
		for (int i = 0; i < h_1; i++)
		{
			for (int j = 0; j < w_1; j++)
			{
				uchar pixel = channels_src1[channel_idx].at<uchar>(i, j);
				channels_src1[channel_idx].at<uchar>(i, j) = pixel_map[static_cast<int>(pixel)];
			}
		}
	}

	cv::merge(channels_src1, 3, dst);

}

void My_Image_Process::sample_equalize_hist_match()
{
	cv::Mat src1 = cv::imread("test_images/bird.jpg", 1);
	cv::Mat src2 = cv::imread("test_images/giraffe.jpg", 1);
	cv::Mat dst;
	dst.create(src1.size(), src1.type());

	equalize_hist_match(src1,src2, dst);

	cv::imshow("src", src1);
	cv::imshow("my_process", dst);
	cv::waitKey(0);
	cv::destroyAllWindows();
}