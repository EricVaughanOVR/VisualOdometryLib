#ifndef CORRESPONDENCEDEFS_HPP
#define CORRESPONDENCEDEGS_HPP

#include <stdint.h>
#include <vector>

namespace correspondence
{
  struct Match
  {
    int idx1, idx2, dist;
  };

  typedef uint8_t byte;

  struct Image
  {
    byte* data;
    int rows, cols, stride;
  };

  struct Feature
  {
    int x, y;
  };

  struct FeatureList
  {
    std::vector<Feature> allFeatures;
    std::vector<Feature> nonmaxFeatures;
    int allFeaturesNum, nonmaxFeaturesNum;

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
    MATCH_DENSE,//attempt to match every pixel in the image
    MATCH_SPARSE//only match pxiels that are marked as points-of-interest
  };

  enum eMatchMode
  {
    FLOW,
    STEREO
  };

  struct ExtractCfg
  {
    eSamplingWindow type;
    eMatchMode extractMode;
  };

  struct DescriptorList
  {
    byte* descriptors;
    eSamplingWindow type;
    int numDescriptors;
  };
};//namespace correspondence
#endif
