#include <cstdio>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.c>
#include <vector>

#include "mytools.hpp"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("An imput image file is missing.\n");
    return EXIT_FAILURE;
  }
  cv::Mat image;
  image = cv::imread(argv[1], cv::ImreadModes::IMREAD_COLOR);
  if (image.empty()) {
    printf("Image file is not found.\n");
    return EXIT_FAILURE;
  }
  bgr2ycbcr(image);

  stb::vector<cv::Mat> ycrcb;
  cv::split(image, ycrcb);

  mozaic()

      cv::merge(ycrcb, image);

  cv::cvtColor(image, image, cv::COLOR_Ycrcb2BGR);
  cv::imshow("image", image);
  cv::waitKey();
  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}
