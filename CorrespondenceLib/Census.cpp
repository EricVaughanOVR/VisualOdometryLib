#include <xmmintrin.h>
#include "Census.hpp"

using namespace correspondence;

void censusTransform(const Image& im, Image& output, eSamplingWindow type, byte** pResult)
{
}

void censusTransformSSE(const Image& im, const CensusCfg& cfg, Image& rResult)
{
  //Copy pattern to local var
  std::vector<int> offsetsLUT;
  offsetsLUT.insert(offsetsLUT.end(), cfg.pattern.begin(), cfg.pattern.end());

  int edgeSize = static_cast<int>(cfg.windowSize * .5);
  int pxSize = cfg.pattern.size() / 8;

  __m128i* byteStr = (__m128i*)_mm_malloc(256 * cfg.pattern.size() / 8, 16);
  for(int i = edgeSize; i < im.rows - edgeSize; ++i)
  {
    //Set resultPtr to beginning of the row
    __m128i* resultPtr = (__m128i*)rResult.at(i, edgeSize * pxSize);

    int j = edgeSize; 
    for(j; j < im.cols - edgeSize; j += 16)
    {
      //Load 16 Object Pixels
      __m128i* objectPx = (__m128i*)im.at(i,j);//TODO must ensure that we load on 16 byte boundary
      for(int k = 0; k < static_cast<int>(offsetsLUT.size()); ++k)
      {
        //Load the next pixel of each center pixel's sampling window
        __m128i* samplePx = objectPx + offsetsLUT[k];//TODO must ensure that we load on 16 byte boundary
        //do comparison
        *byteStr = _mm_cmpgt_epi8(*objectPx, *samplePx);
        ++byteStr;//Increment for next 16 sample point compares
      }
      //Store the result into rResult
      //TODO Unpack
    }
    //TODO need to rewind j if remainder is not 0
    //Finish the remainder of the row with scalarTransform
    for(j; j < im.cols - cfg.windowSize * .5; ++j)
    {
    }
  }
}

void censusTransformScalar(const Image& im, const CensusCfg& cfg, Image& rResult)
{
  int edgeSize = static_cast<int>(cfg.windowSize * .5);
  int pxSize = cfg.pattern.size() / 8;

  byte* resultPtr;

  for(int i = edgeSize; i < im.rows - edgeSize; ++i)
  {
    //Set resultPtr to beginning of the row
    resultPtr = rResult.at(i, edgeSize * pxSize);
    for(int j = edgeSize; j < im.cols - edgeSize; ++j)
    {
      //Now we have chosen a pixel to examine
      int bitCount = 0;
      censusTransformSinglePx(im.at(i, j), cfg.pattern, &resultPtr);
    }
  }
}