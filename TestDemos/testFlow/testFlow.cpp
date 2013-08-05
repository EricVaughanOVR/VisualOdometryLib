#include "CorrespondenceDefs.hpp"
#include "Census.hpp"
#include "fast.hpp"
#include "Matcher.hpp"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
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

  Mat matL = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE );
  if( !matL.data ) 
  {
    std::cout<< "Error reading image " << argv[1] << std::endl;
    return -1;
  }
  Mat matR = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE );
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

  CensusCfg cfg(SPARSE_8, imageL.rows, imageL.cols, imageL.stride, imageL.pxStep);

  //Do Census Transform
  double t = (double)cv::getTickCount();
  Image censusL(matL.rows, matL.cols, 1/*size of descriptor in bytes*/, offset);
  Image censusR(matR.rows, matR.cols, 1/*size of descriptor in bytes*/, offset);
  t = (double)cv::getTickCount();
  censusTransformSSE(imageL, cfg, censusL);
  censusTransformSSE(imageR, cfg, censusR);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"SSE Census Transform "<<t/1.0<<std::endl;

  //Do Feature Detection
  FeatureList kpsL, kpsR;
  fast10_detect_both(imageL.data, imageL.cols, imageL.rows, imageL.stride, 8, kpsL);
  fast10_detect_both(imageR.data, imageR.cols, imageR.rows, imageR.stride, 8, kpsR);

  //Do Stereo Matching
  MatchingParams params(FLOW, SPARSECW_16, 30, static_cast<int>(imageL.cols * .1), 1, censusL.stride, censusL.pxStep);
  Matcher census(cfg, params, imageL.rows, imageL.cols);

  std::vector<Match> matches;
  t = (double)cv::getTickCount();
  
  census.matchSparse(censusL, censusR, kpsL, kpsR, matches, 36);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"Matching Time "<<t/1.0<<std::endl;
  
  std::cout<<"Number of matches: "<<matches.size()<<std::endl;

  if(!matches.empty())
  {
    namedWindow("Matches", CV_WINDOW_KEEPRATIO);
    // Draw matches
    Mat imgMatch;
    cvtColor(matL, imgMatch, CV_GRAY2RGB);
    
    for(size_t i = 0; i < matches.size(); ++i)
    {
      line(imgMatch, 
        cv::Point(kpsL.allFeatures[matches[i].feature1Idx].x, 
                  kpsL.allFeatures[matches[i].feature1Idx].y),
        cv::Point(kpsR.allFeatures[matches[i].feature2Idx].x, 
                  kpsR.allFeatures[matches[i].feature2Idx].y),
        CV_RGB(255, 0, 0), 1);
    }
    imshow("Matches", imgMatch);
    waitKey(0);
  }

  return 0;
}
