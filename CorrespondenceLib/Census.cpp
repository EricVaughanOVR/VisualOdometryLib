#include <xmmintrin.h>
#include "Census.hpp"

using namespace correspondence;

byte* censusTransform(const Image& im, Image& output, eSamplingWindow type)
{
  return new byte;//placeholder
}

byte* censusTransformSSE(const Image& im, const CensusCfg& cfg)
{
  //Copy pattern to local var
  std::vector<int> offsetsLUT;
  offsetsLUT.insert(offsetsLUT.end(), cfg.pattern.begin(), cfg.pattern.end());

  //Allocate space for an 'n' length descriptor for each pixel
  byte* census = (byte*)_mm_malloc(offsetsLUT.size() * im.cols * im.stride, 16);

  //Allocate space to store all transform information for a single group of 16 center pixels
  byte* resultBuf = (byte*)_mm_malloc(offsetsLUT.size() * 16, 16);
  byte* bufPtr = resultBuf;

  for(int i = cfg.windowSize * .5; i < im.rows - cfg.windowSize * .5; ++i)
  {
    int j = static_cast<int>(cfg.windowSize * .5);
    for(int j; j < im.cols - cfg.windowSize * .5; j += 16)
    {
      //Load 16 Center Pixels
      __m128i centerPx = _mm_loadu_si128((__m128i*)im.at(i,j));//TODO see about ensuring memory alignment
      for(int k = 0; k < offsetsLUT.size(); ++k)
      {
        //Load the next pixel of each center pixel's sampling window
        __m128i samplePx = _mm_loadu_si128((__m128i*)im.at(i,j) + offsetsLUT[k]);
        //do comparison
        __m128i* byteStr = (__m128i*)bufPtr;
        *byteStr = _mm_cmpgt_epi8(centerPx, samplePx);
        bufPtr += 16;//Increment for next 16 sample point compares
      }
      //Store the result into result
      //TODO Unpack
    }
    //TODO need to rewind j if remainder is not 0
    //Finish the remainder of the row
    for(j; j < im.cols - cfg.windowSize * .5; ++j)
    {
    }
  }
  return new byte;//placeholder
}