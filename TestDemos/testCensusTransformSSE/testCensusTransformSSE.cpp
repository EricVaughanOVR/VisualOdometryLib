#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Census.hpp"

using namespace correspondence;
using namespace cv;

int main(int argc, char* argv)
{
  Mat img = imread("../../../Resources/ImageData/TeddyLeft.png", CV_LOAD_IMAGE_GRAYSCALE);
  Image image(img.rows, img.cols, 8, (byte*)img.data, img.cols * img.step[1] + img.rows * img.step[0]);

  CensusCfg cfg;

  cfg.type = eSamplingWindow::SPARSE_16;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.windowSize, image.stride);

  Image scalarResult(img.rows, img.cols, 8);
  censusTransformScalar(image, cfg, scalarResult);

  Image sseResult(img.rows, img.cols, 8);
  censusTransformSSE(image, cfg, sseResult);

  return 0;
}