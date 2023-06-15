#include <cstdio>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
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
  cv::Mat original = image.clone();
  if (argc < 3) {
    printf("Qfactor is missing.\n");
    return EXIT_FAILURE;
  }
  int QF = strtol(argv[2], nullptr, 10);
  if (QF < 0 || QF > 100) {
    printf("Valid range for Qfactor is from 0 to 100");
    return EXIT_FAILURE;
  }
  QF = (QF == 0) ? 1 : QF;
  float scale;
  if (QF < 50) {
    scale = floorf(5000.0 / QF);
  } else {
    scale = 200 - QF * 2;
  }
  scale /= 100.0;

  scale = (scale < FLT_EPSILON) ? -1.0 : scale;

  bgr2ycrcb(image);
  std::vector<cv::Mat> ycrcb;
  cv::split(image, ycrcb);

  constexpr float D = 2;
  for (int c = 0; c < image.channels(); ++c) {
    if (c > 0) {
      cv::resize(ycrcb[c], ycrcb[c], cv::Size(), D, D, cv::INTER_AREA);
    }
    cv::Mat buf;
    ycrcb[c].convertTo(buf, CV_32F);

    // encoder
    blkproc(buf, blk::dct2);
    blkproc(buf, blk::quantize, c, scale);

    // decoder
    blkproc(buf, blk::dequantize, c, scale);
    blkproc(buf, blk::idct2);

    buf.convertTo(ycrcb[c], ycrcb[c].type());
    if (c > 0) {
      cv::resize(ycrcb[c], ycrcb[c], cv::Size(), 1 / 0.5, 1 / 0.5,
                 cv::INTER_AREA);
    }
  }

  cv::merge(ycrcb, image);

  cv::cvtColor(image, image, cv::COLOR_YCrCb2BGR);

  // PSNR計算を行う
  //
  // OpenCv quality psnr 検索

  cv::imshow("image", image);
  cv::waitKey();
  cv::destroyAllWindows();

  return EXIT_SUCCESS;
}
