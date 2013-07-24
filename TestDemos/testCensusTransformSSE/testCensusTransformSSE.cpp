#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Census.hpp"

using namespace correspondence;
using namespace cv;

int main(int argc, char* argv)
{
  Mat img = imread("../../../Resources/ImageData/TeddyLeft.png", CV_LOAD_IMAGE_GRAYSCALE);
  Image image;
  image.cols = img.cols;
  image.rows = img.rows;
  image.stride = img.step[0];
  image.data = img.data;

  CensusCfg cfg;

  cfg.type = eSamplingWindow::DENSE_3;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.windowSize, image.stride);

  cfg.type = eSamplingWindow::DENSE_5;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.windowSize, image.stride);

  cfg.type = eSamplingWindow::DENSE_7;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.windowSize, image.stride);

  cfg.type = eSamplingWindow::DENSE_9;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.windowSize, image.stride);

  cfg.type = eSamplingWindow::DENSE_11;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.windowSize, image.stride);

  cfg.type = eSamplingWindow::DENSE_13;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.windowSize, image.stride);
  censusTransformSSE(image, cfg);

  return 0;
}