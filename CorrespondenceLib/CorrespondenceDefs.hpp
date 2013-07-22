#ifndef CORRESPONDENCEDEFS_HPP
#define CORRESPONDENCEDEGS_HPP

#include <stdint.h>
#include <vector>

namespace correspondence
{
  typedef uint8_t byte;

  struct Image
  {
    byte* data;
    int rows, cols, stride;
  };

  struct Feature
  {
    int x, y, idx;
  };

  struct Match
  {
    Feature feature1, feature2;
    int dist;
  };

  struct FeatureList
  {
    std::vector<Feature> allFeatures;
    std::vector<Feature> nonmaxFeatures;

    //Element for each row of the image, value indicates the first element of that row 
    std::vector<int> rowIdxs;
  };

  enum eSamplingWindow
  {
    SPARSE_8,//8 or 16 sampling points
    SPARSE_16,
    DENSE_3,//various sizes of dense sampling windows
    DENSE_5,
    DENSE_7,
    DENSE_9,
    DENSE_11,
    DENSE_13
  };

  enum eMatchMode
  {
    FLOW,
    STEREO
  };

  struct MatchingParams
  {
    int filterDist;
    int maxDisparity;
    int epipolarRange;
  };

  struct CensusCfg
  {
    eSamplingWindow type;
    eMatchMode extractMode;
    int windowSize;
  };

  struct Descriptors
  {
    Image tfmdIm;
    eSamplingWindow type;
  };
};//namespace correspondence
#endif
