#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Census.hpp"

using namespace correspondence;
using namespace cv;

int main(int argc, char* argv)
{
  //Mat img = imread("../../../Resources/ImageData/TeddyLeft.png", CV_LOAD_IMAGE_GRAYSCALE);
  Mat img = imread("../../../Resources/ImageData/CensusTestImage_24x24.png", CV_LOAD_IMAGE_GRAYSCALE);
  double t = (double)cv::getTickCount();
  Image image(img.rows, img.cols, 1, (byte*)img.data, img.cols * img.step[1] + img.rows * img.step[0]);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"memcpy align image "<<t/1.0<<std::endl;

  CensusCfg cfg;

  cfg.type = eSamplingWindow::SPARSE_16;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.windowSize, image.stride);

  Image scalarResult(img.rows, img.cols, 2);//pxStep = 16 descriptor length is 16 bits
 
  t = (double)cv::getTickCount();
  censusTransformScalar(image, cfg, scalarResult);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"Scalar Census Transform "<<t/1.0<<std::endl;

  Image sseResult(img.rows, img.cols, 2);
  t = (double)cv::getTickCount();
  censusTransformSSE(image, cfg, sseResult);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"SSE Census Transform "<<t/1.0<<std::endl;

  namedWindow("Original Image", CV_WINDOW_KEEPRATIO);
  namedWindow("Census Image", CV_WINDOW_KEEPRATIO);
  namedWindow("SSE Census Image", CV_WINDOW_KEEPRATIO);
  Mat censusImg(Size(img.cols, img.rows), CV_16U, scalarResult.data, scalarResult.stride);
  Mat sseCensusImg(Size(img.cols, img.rows), CV_16U, sseResult.data, sseResult.stride);

  imshow("Census Image", censusImg);
  imshow("Original Image", img);
  imshow("SSE Census Image", sseCensusImg);
  waitKey();

  return 0;
}