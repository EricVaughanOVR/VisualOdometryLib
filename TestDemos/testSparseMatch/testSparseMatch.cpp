#include "CorrespondenceDefs.hpp"
#include "Census.hpp"
#include "fast.hpp"
#include "Match.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace correspondence;
using namespace cv;

int main(int argc, char* argv)
{
  //Load images
  Mat matL = imread("../../../Resources/ImageData/TeddyLeft.png", CV_LOAD_IMAGE_GRAYSCALE);
  Mat matR = imread("../../../Resource/ImageData/TeddyRight.png", CV_LOAD_IMAGE_GRAYSCALE);
  pt offset;
  offset.x = 0;
  offset.y = 0;
  Image imageL(matL.rows, matL.cols, 1, offset, (byte*)matL.data);
  Image imageR(matR.rows, matR.cols, 1, offset, (byte*)matR.data);

  CensusCfg cfg;
  cfg.type = eSamplingPattern::SPARSE_16;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.windowSize, imageL.stride);

  //Do Census Transform
  double t = (double)cv::getTickCount();
  Image censusL(matL.rows, matL.cols, 2, offset);
  Image censusR(matR.rows, matR.cols, 2, offset);
  t = (double)cv::getTickCount();
  censusTransformSSE(imageL, cfg, censusL);
  censusTransformSSE(imageR, cfg, censusR);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"SSE Census Transform "<<t/1.0<<std::endl;

  //Do Stereo Matching

  namedWindow("Matches", CV_WINDOW_KEEPRATIO);

  Mat matches;
  imshow("Matches", matches);
  waitKey();

  return 0;
}
