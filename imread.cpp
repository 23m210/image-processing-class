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
  bgr2ycrcb(image);

  stb::vector<cv::Mat> ycrcb;
  cv::split(image, ycrcb);

  for (int c = 0; c < image.channels(); ++c) {
    cv::Mat buf;
    ycrcb[c].convertTo(buf, CV_32F);

    blkproc(buf, blk::dct2);
    blkproc(buf, blk::quantize);

    // decoder
    blkproc(buf, blk::dequantize);
    blkproc(buf, blk::idct2);

    buf.convertTo(ycrcb[c], ycrcb[c].type());
  }

  cv::merge(ycrcb, image);

  cv::cvtColor(image, image);
  cv::imshow("image", image);
  cv::waitKey();
  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}
