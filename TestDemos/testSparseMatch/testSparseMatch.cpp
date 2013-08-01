#include "CorrespondenceDefs.hpp"
#include "Census.hpp"
#include "fast.hpp"
#include "Match.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace correspondence;
using namespace cv;

int main(int argc, char* argv)
{
  //Load images
  Mat matL = imread("../../../Resources/ImageData/TeddyLeft.png", CV_LOAD_IMAGE_GRAYSCALE);
  Mat matR = imread("../../../Resources/ImageData/TeddyRight.png", CV_LOAD_IMAGE_GRAYSCALE);
  pt offset;
  offset.x = 0;
  offset.y = 0;
  Image imageL(matL.rows, matL.cols, 1, offset, (byte*)matL.data);
  Image imageR(matR.rows, matR.cols, 1, offset, (byte*)matR.data);

  CensusCfg cfg;
  cfg.imgCols = imageL.cols;
  cfg.imgRows = imageR.rows;
  cfg.type = eSamplingPattern::SPARSE_16;
  prepOffsetsLUT(cfg.type, cfg.pattern, cfg.patternSize, imageL.stride);

  //Do Census Transform
  double t = (double)cv::getTickCount();
  Image censusL(matL.rows, matL.cols, 2, offset);
  Image censusR(matR.rows, matR.cols, 2, offset);
  t = (double)cv::getTickCount();
  censusTransformSSE(imageL, cfg, censusL);
  censusTransformSSE(imageR, cfg, censusR);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"SSE Census Transform "<<t/1.0<<std::endl;

  //Do Feature Detection
  FeatureList kpsL, kpsR;
  fast10_detect_both(imageL.data, imageL.cols, imageL.rows, imageL.stride, 15, kpsL);
  fast10_detect_both(imageR.data, imageR.cols, imageR.rows, imageR.stride, 15, kpsR);

  //Do Stereo Matching
  
  cfg.params.mode = STEREO;
  cfg.params.corrType = DENSE_5;
  cfg.params.windowSize = 5;
  cfg.params.epipolarRange = 1;
  cfg.params.filterDist = 10;
  std::vector<Match> matches;
  cfg.params.maxDisparity = static_cast<int>(imageL.cols * .1);
  t = (double)cv::getTickCount();
  
  matchSparse(censusL, censusR, cfg, kpsL, kpsR, matches);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout<<"Matching Time "<<t/1.0<<std::endl;
  
  namedWindow("Matches", CV_WINDOW_KEEPRATIO);

  std::vector<cv::DMatch> dmatches;
  std::vector<KeyPoint> cvKpsL, cvKpsR;

  for(int i = 0; i < matches.size(); ++i)
  {
    DMatch _match;
    _match.queryIdx = matches[i].feature1Idx;
    _match.trainIdx = matches[i].feature2Idx;
    dmatches.push_back(_match);
  }

  for(int i = 0; i < kpsL.allFeatures.size(); ++i)
  {
    KeyPoint kp;
    kp.pt.x = kpsL.allFeatures[i].x;
    kp.pt.y = kpsL.allFeatures[i].y;
    cvKpsL.push_back(kp);
  }

  for(int i = 0; i < kpsR.allFeatures.size(); ++i)
  {
    KeyPoint kp;
    kp.pt.x = kpsR.allFeatures[i].x;
    kp.pt.y = kpsR.allFeatures[i].y;
    cvKpsR.push_back(kp);
  }

  // Draw matches
  Mat imgMatch;
  std::vector<char> mask;
  drawMatches(matL, cvKpsL, matR, cvKpsR, dmatches, imgMatch, cv::Scalar::all(-1), cv::Scalar::all(-1), mask, 2);


  imshow("Matches", imgMatch);
  waitKey(0);

  return 0;
}
