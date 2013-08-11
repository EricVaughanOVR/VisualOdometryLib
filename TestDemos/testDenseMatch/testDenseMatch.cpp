#include "CorrespondenceDefs.hpp"
#include "Census.hpp"
#include "fast.hpp"
#include "Matcher.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace correspondence;
using namespace cv;

int main(int argc, char** argv)
{
  if( argc != 3 ) 
  {
    std::cout<<std::endl<<"Usage: "<<argv[0]<<"[path to image1] [path to image2]"<<std::endl;
    return -1;
  }

  Mat matL = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE );
  if( !matL.data ) 
  {
    std::cout<< "Error reading image " << argv[1] << std::endl;
    return -1;
  }
  Mat matR = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE );

  if( !matR.data ) 
  {
    std::cout<< "Error reading image " << argv[2] << std::endl;
    return -1;
  }
  pt offset;
  offset.x = 0;
  offset.y = 0;
  Image imageL(matL.rows, matL.cols, 1, offset, (byte*)matL.data);
  Image imageR(matR.rows, matR.cols, 1, offset, (byte*)matR.data);

  CensusCfg cfg(SPARSE_16, imageL.rows, imageL.cols, imageL.stride, imageL.pxStep);

  //Do Census Transform
  double t = (double)cv::getTickCount();
  Image censusL(matL.rows, matL.cols, 2/*size of descriptor in bytes*/, offset);
  Image censusR(matR.rows, matR.cols, 2/*size of descriptor in bytes*/, offset);
  t = (double)cv::getTickCount();
  censusTransformSSE(imageL, cfg, censusL);
  censusTransformSSE(imageR, cfg, censusR);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"SSE Census Transform "<<t/1.0<<std::endl;

  //Do Stereo Matching
  MatchingParams params(STEREO, DENSECW_13, 1000, static_cast<int>(imageL.cols * .1), 1, censusL.stride, censusL.pxStep);
  Matcher census(cfg, params, imageL.rows, imageL.cols);

  Image denseMap(censusL.rows, censusL.cols, 1, pt());
  t = (double)cv::getTickCount();
  
  census.matchDense(censusL, censusR, denseMap);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"Matching Time "<<t/1.0<<std::endl;

  cv::namedWindow("Dense Depth Map", CV_WINDOW_KEEPRATIO);
  // Draw matches
  Mat depthMat(Size(denseMap.rows, denseMap.cols), CV_8U, denseMap.data, denseMap.stride);
  imshow("Dense Depth Map", depthMat);
  waitKey(0);

  return 0;
}
