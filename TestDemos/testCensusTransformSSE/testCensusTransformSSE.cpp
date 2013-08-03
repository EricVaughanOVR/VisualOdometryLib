#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Census.hpp"

using namespace correspondence;
using namespace cv;

int main(int argc, char** argv)
{
  if(argc != 2)
  {
    std::cout<<std::endl<<"Usage: "<<argv[0]<<"[path to image]"<<std::endl;
    return -1;
  }

  Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  if(!img.data)
  {
    std::cout<<"Error reading image " <<argv[1]<<std::endl;
    return -1;
  }

  double t = (double)cv::getTickCount();
  pt offset;
  offset.x = 0;
  offset.y = 0;
  Image image(img.rows, img.cols, 1, offset, (byte*)img.data);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"memcpy align image "<<t/1.0<<std::endl;

  CensusCfg cfg(SPARSE_16, image.rows, image.cols, image.stride, image.pxStep);

  Image scalarResult(img.rows, img.cols, 2, offset);//pxStep = 2 descriptor length is 16 bits
 
  t = (double)cv::getTickCount();
  censusTransformScalar(image, cfg, scalarResult);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"Scalar Census Transform "<<t/1.0<<std::endl;

  Image sseResult(img.rows, img.cols, 2, offset);
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
