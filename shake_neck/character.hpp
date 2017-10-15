#pragma once

#include <opencv2/opencv.hpp>

class Character {

private:
	cv::Mat face;
	cv::Mat body;

	void paste(cv::Mat dst, cv::Mat src, int x, int y, int width, int height);

public:
	Character(cv::Mat &face, cv::Mat &body);
	void draw(cv::Mat &background, const cv::Point &face_point, const cv::Point &body_point);
	cv::Point recm_body_point(cv::Point face_point);

};