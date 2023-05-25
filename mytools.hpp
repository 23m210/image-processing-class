#pragma once
#include <opencv2/core.hpp>

#define BSIZE 16

void bgr2ycbcr(cv::Mat &image);

void mozaic(stb::vector<cv::Mat> 8);
