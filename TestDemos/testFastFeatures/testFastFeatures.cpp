#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>

#include "fast.hpp"

using namespace cv;
using namespace correspondence;

static void help( char** argv )
{
  std::cout<<"\nUsage: "<<argv[0]<<"[path/to/image]"<< std::endl;
}

int main( int argc, char** argv ) 
{
  if( argc != 2 ) {
    help(argv);
    return -1;
  }

  // Load image
  Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE );
  if( !img.data ) {
    std::cout<< " --(!) Error reading image " << argv[1] << std::endl;
    return -1;
  }

  double t = (double)getTickCount();
  FeatureList corners;
  fast10_detect_both((byte*)img.data, img.cols, img.rows, img.cols, 15, corners);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout << "detection time [s]: " << t/1.0 << std::endl;

  Mat imgColor;
  cvtColor(img, imgColor, CV_GRAY2RGB);

  for(size_t i = 0; i < corners.allFeatures.size(); ++i)
  {
	  cv::Point pt(corners.allFeatures[i].x, corners.allFeatures[i].y);
	  cv::line(imgColor, pt, pt, CV_RGB(255, 0, 0), 1); 
  }
  for(int i = 0; i < static_cast<int>(corners.nonmaxFeatures.size()); ++i)
  {
	  cv::Point pt(corners.nonmaxFeatures[i].x, corners.nonmaxFeatures[i].y);
	  cv::line(imgColor, pt, pt, CV_RGB(0, 255, 0), 1); 
  }

  namedWindow("Features", CV_WINDOW_KEEPRATIO);
  imshow("Features", imgColor);
  waitKey(0);
}
