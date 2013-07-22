#include <xmmintrin.h>
#include "Census.hpp"

using namespace correspondence;

void censusTransform(const Image& im, Image& output, eSamplingWindow type)
{
  
}

void censusTransformSSE(const Image* im, Image* output, const CensusCfg& cfg)
{
  std::vector<int> offsetsLUT;

  __m128 imData;

  //Load im + eSamplingWindow size

  for(int i = cfg.windowSize * .5; i < im->rows - cfg.windowSize * .5; ++i)
  {
    for(int j = cfg.windowSize * .5; j < im->cols - cfg.windowSize * .5; ++j)
    {
      //Load 
    }
  }
}