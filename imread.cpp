#include <cstdio>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
// #include <opencv2/quality/qualitypsnr.hpp>
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

  int qtable_L[64], qtable_C[64];
  create_qtable(0, scale, qtable_L);
  create_qtable(1, scale, qtable_C);

  bgr2ycrcb(image);
  std::vector<cv::Mat> ycrcb;
  std::vector cv::Mat buf(image);
  cv::split(image, ycrcb);

  constexpr float D = 2;
  // encoder
  for (int c = 0; c < image.channels(); ++c) {
    int *qtable = qtable_L;
    if (c > 0) {
      qtable = qtable_C;
      cv::resize(ycrcb[c], ycrcb[c], cv::Size(), 1 / D, 1 / D,
                 cv::INTER_LINEAR_EXACT);
    }
    ycrcb[c].convertTo(buf[c], CV_32F);
    buf[c] -= 128.0;
    blkproc(buf[c], blk::dct2);
    blkproc(buf[c], blk::quantize, qtable);
  }

  // decoder
  for (int c = 0; c < image.channels(); ++c) {
    int *qtable = qtable_L;
    if (c > 0) {
      qtable = qtable_C;
      blkproc(buf[c], blk::dequantize, qtable);
      blkproc(buf[c], blk::idct2);
      buf[c] += 128.0;
      buf[c].convertTo(ycrcb[c], ycrcb[c].type());
      if (c > 0) {
        cv::resize(ycrcb[c], ycrcb[c], cv::Size(), 1 / 0.5, 1 / 0.5,
                   cv::INTER_AREA);
      }
    }

    cv::merge(ycrcb, image);

    cv::cvtColor(image, image, cv::COLOR_YCrCb2BGR);

    // cv::qualitypsnr(image, image);
    // int i, mse, Io, Ic;
    // int M, N;

    // i = 10.0 * log10((255 * 255) / mse);  // PSNR計算を行う

    // mse = 1 / M * N * (Io - Ic);

    // OpenCv quality psnr 検索

    cv::imshow("image", image);
    cv::waitKey();
    cv::destroyAllWindows();

    return EXIT_SUCCESS;
  }
