#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking/tracker.hpp>
#include <opencv2/tracking/tldDataset.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int tracker_init(cv::Ptr<cv::TrackerKCF> &tracker, cv::Rect2d &rect)
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened())
	{
		std::cout << "CAN'T OPEN CAMERA." << std::endl;
		return -1;
	}

	cv::Mat frame;
	cv::namedWindow("Select face");

	while (true)
	{
		cap >> frame;
		if (frame.empty())
		{
			std::cout << "FAILD TO READ A FRAME" << std::endl;
			cv::destroyAllWindows();
			break;
		}

		cv::imshow("Select face", frame);
		
		switch (cv::waitKey(1))
		{
		case 'q':
			return -1;
		case 't':
			rect = cv::selectROI("tracker", frame);
			tracker->init(frame, rect);
			cv::destroyAllWindows();
			return 1;
		default:
			break;
		}
	}

	return -1;
}

cv::Point get_center(const cv::Rect2d &rect)
{
	return cv::Point(rect.tl().x + ((rect.br().x - rect.tl().x) / 2), rect.tl().y + ((rect.br().y - rect.tl().y) / 2));
}

cv::Point mirror_point(const cv::Point &p, int center)
{
	return cv::Point((center << 1) - p.x, p.y);
}

int main(int argc, char* argv[])
{
	cv::Ptr<cv::TrackerKCF> tracker = cv::TrackerKCF::create();
	cv::Rect2d roi;

	if (tracker_init(tracker, roi) == -1)
	{
		/*
		* 初期化に失敗
		*/
		return 0;
	}

	cv::Scalar color = cv::Scalar(0, 255, 0);
	cv::Mat frame;
	cv::VideoCapture cap(0);
	if (!cap.isOpened())
	{
		std::cout << "CAN'T OPEN CAMERA." << std::endl;
		return -1;
	}

	while (cv::waitKey(1) != 'q')
	{
		cv::Mat front(cv::Size(640, 480), CV_8UC3, cv::Scalar(0, 0, 0));
		cap >> frame;
		if (frame.empty())
		{
			break;
		}

		//更新
		tracker->update(frame, roi);

		cv::circle(front, mirror_point(get_center(roi), 320), 3, cv::Scalar(0, 255, 255));

		cv::imshow("syn - SHAKE YOUR NECK!!", front);

	}

	return 0;
}