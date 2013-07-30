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

  int numCorners = 0, numNonmax = 0;
  
  double t = (double)getTickCount();
  Feature* totFeatures = new Feature;
  Feature* nonmax_features = new Feature;
  fast9_detect_both((byte*)img.data, img.cols, img.rows, img.cols, 15, 
    &numNonmax, &nonmax_features, &numCorners, &totFeatures);
  t = ((double)getTickCount() - t)/getTickFrequency();
  std::cout << "detection time [s]: " << t/1.0 << std::endl;

  Mat imgColor;
  cvtColor(img, imgColor, CV_GRAY2RGB);

  for(int i = 0; i < numCorners; ++i)
  {
	  cv::Point pt((totFeatures + i)->x, (totFeatures + i)->y);
	  cv::line(imgColor, pt, pt, CV_RGB(255, 0, 0), 1); 
  }
  for(int i = 0; i < numNonmax; ++i)
  {
	  cv::Point pt((nonmax_features + i)->x, (nonmax_features + i)->y);
	  cv::line(imgColor, pt, pt, CV_RGB(0, 255, 0), 1); 
  }

  namedWindow("Features", CV_WINDOW_KEEPRATIO);
  imshow("Features", imgColor);
  waitKey(0);

  delete totFeatures;
  delete nonmax_features;
}
