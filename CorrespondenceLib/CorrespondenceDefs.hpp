#ifndef CORRESPONDENCEDEFS_HPP
#define CORRESPONDENCEDEGS_HPP

#include <stdint.h>
#include <vector>

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

enum eDescriptorType
{
  SPARSE_4,//4, 8, or 16-bit sparse Census-based descriptor
  SPARSE_8,
  SPARSE_16,
  DENSE_3x3,//various sizes of dense Census descriptors
  DENSE_5x5,
  DENSE_7x7,
  DENSE_9x9,
  DENSE_11x11,
  DENSE_13x13
};

enum eMode
{
  MATCH_DENSE,//attempt to match every pixel in the image
  MATCH_SPARSE//only match pxiels that are marked as points-of-interest
};


#endif
