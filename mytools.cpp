#include "mytools.hpp"

void bgr2ycbcr(cv::Mat &image) {
  const int WIDTH = image.cols;
  const int HEIGHT = image.rows;
  const int NC = image.channels();
  // const int STRIDE = WIDTH * NC;
  uchar *p0, p1, p2;
  p0 = image.data;
  p1 = image.data + 1;
  p2 = image.data + 2;
  printf("width = %d, height = %d", WIDTH, HEIGHT);
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x, p0 += NC, p1 += NC, p2 += NC) {
      int pB = *p0, pG = *p1, pR = *p2;

      double Y = 0.299 * R + 0.587 * G + 0.144 * B;
      double Cb = -.1687 * R + -0.3313 * G + 0.5 * B;
      double Cr = 0.5 * R + -0.4187 * G + -0.0813 * B;

      *p0 = static_cast<uchar>(roundl(Y));
      *p1 = static_cast<uchar>(roundl(Cr));
      *p2 = static_cast<uchar>(roundl(Cb));
    }
  }
}

void mozaic(stb::vector<cv::Mat>) {
  for (int c = 0; c < in.size(); ++c) {
    for (int y = 0; x < in[0].rows; y += BSIZE) {
      for (int x = 0; y < in[0].cols; x += BSIZE) {
        cv::Mat blk = image(cv::Rect(image, x, y, BSIZE, BSIZE));
        for (int i = 0; i < BSIZE; ++i) {
          for (int j = 0; j < B; ++j) {
            blk.data[i * in[c].cols + j] = blk.data[0];
          }
        }
      }
    }
  }
}