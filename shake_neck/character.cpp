#include "character.hpp"

Character::Character(cv::Mat &face_image, cv::Mat &body_image)
	: face(face_image), body(body_image){}


void Character::paste(cv::Mat dst, cv::Mat src, int x, int y, int width, int height)
{
	cv::Mat resized_img;
	cv::resize(src, resized_img, cv::Size(width, height));

	if (x >= dst.cols || y >= dst.rows)
	{
		return;
	}
	int w = (x >= 0) ? std::min(dst.cols - x, resized_img.cols) : std::min(std::max(resized_img.cols + x, 0), dst.cols);
	int h = (y >= 0) ? std::min(dst.rows - y, resized_img.rows) : std::min(std::max(resized_img.rows + y, 0), dst.rows);
	int u = (x >= 0) ? 0 : std::min(-x, resized_img.cols - 1);
	int v = (y >= 0) ? 0 : std::min(-y, resized_img.rows - 1);
	int px = std::max(x, 0);
	int py = std::max(y, 0);

	cv::Mat roi_dst = dst(cv::Rect(px, py, w, h));
	cv::Mat roi_resized = resized_img(cv::Rect(u, v, w, h));
	roi_resized.copyTo(roi_dst);
}

void Character::draw(cv::Mat &background, const cv::Point &face_point, const cv::Point &body_point)
{
	paste(background, body, body_point.x - (body.cols / 2), body_point.y - (body.rows / 2), body.cols, body.rows);
	paste(background, face, face_point.x - (face.cols / 2), face_point.y - (face.rows / 2), face.cols, face.rows);
}

cv::Point Character::recm_body_point(cv::Point face_point)
{
	return cv::Point(face_point.x, face_point.y + (face.rows / 2) + (body.rows / 2));
}