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

  //Allocate space for an 'n' length descriptor for each pixel
  byte* census = (byte*)_mm_malloc(offsetsLUT.size() * im.cols * im.stride, 16);

  //Allocate space to store all transform information for a single group of 16 center pixels
  byte* resultBuf = (byte*)_mm_malloc(offsetsLUT.size() * 16, 16);
  byte* bufPtr = resultBuf;

  for(int i = static_cast<int>(cfg.windowSize * .5); i < im.rows - cfg.windowSize * .5; ++i)
  {
    int j = static_cast<int>(cfg.windowSize * .5);
    for(j; j < im.cols - cfg.windowSize * .5; j += 16)
    {
      //Load 16 Center Pixels
      __m128i centerPx = _mm_loadu_si128((__m128i*)im.at(i,j));//TODO see about ensuring memory alignment
      for(int k = 0; k < static_cast<int>(offsetsLUT.size()); ++k)
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
}


//Temporary, use to check accuracy of new function, then use the new function as ground-truth
uint16_t transform9x9(const byte* img, byte* pixelLoc, std::vector<int>& m_sampleOffsets_9x9)
{
  uint16_t result = 0;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[0]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[1]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[2]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[3]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[4]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[5]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[6]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[7]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[8]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[9]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[10]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[11]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[12]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[13]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[14]) > *pixelLoc)) << 1;
  result = (result + (*(pixelLoc + m_sampleOffsets_9x9[15]) > *pixelLoc));

  return result;
}

void censusTransformScalar(const Image& im, const CensusCfg& cfg, Image& rResult)
{
  //Copy pattern to local var
  //std::vector<int> offsetsLUT;
  //offsetsLUT.insert(offsetsLUT.end(), cfg.pattern.begin(), cfg.pattern.end());

  int edgeSize = static_cast<int>(cfg.windowSize * .5);
  int pxSize = cfg.pattern.size() / 8;

  byte* resultPtr;

  for(int i = edgeSize; i < im.rows - edgeSize; ++i)
  {
    resultPtr = rResult.at(i, edgeSize * pxSize);//Set resultPtr to beginning of the row
    for(int j = edgeSize; j < im.cols - edgeSize; ++j)
    {
      //Now we have chosen a pixel to examine
      int bitCount = 0;
      censusTransformSinglePx(im.at(i, j), cfg.pattern, &resultPtr);
    }
  }
}