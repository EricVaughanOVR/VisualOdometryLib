#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Census.hpp"

using namespace correspondence;
using namespace cv;

int main(int argc, char* argv)
{
  Mat img = imread("../../../Resources/ImageData/TeddyLeft.png", CV_LOAD_IMAGE_GRAYSCALE);
  Image image(img.cols, img.rows, img.step[0], 8);
  image.data.reset(img.data);

  CensusCfg cfg;

  cfg.type = eSamplingWindow::SPARSE_16;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.windowSize, image.stride);

  Image scalarResult(img.cols, img.rows, img.step[0], 8);
  censusTransformScalar(image, cfg, scalarResult);

  Image sseResult(img.cols, img.rows, img.step[0], 8);
  censusTransformSSE(image, cfg, sseResult);

  return 0;
}