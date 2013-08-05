#include <xmmintrin.h>
#include <algorithm>
#include "Census.hpp"

using namespace correspondence;

namespace
{
  void censusTransformSinglePx(const correspondence::byte* objectPx, const std::vector<int>& offsetsLUT,
                                      correspondence::byte** pResultPx)
  {
    int bitCount = 0;
    for(int k = 0; k < static_cast<int>(offsetsLUT.size()); ++k)
    {
      //Do comparison here
      **pResultPx += (*(objectPx + offsetsLUT[k]) >= *objectPx) << bitCount;
      if(bitCount < 7)
      {
        ++bitCount;
      }
      else//When we have converted a byte, increment resultPtr
      {
        bitCount = 0;
        ++*pResultPx;
      }
    }
  }

  //After calculating the census descriptor for the current group of 16 pixels,
  //move the results into the census-image, by interleaving the __m128 vectors
  //TRICKY: vect1 and vect2 MUST be 16-byte aligned or we will crash!
  void storeSSE16(__m128i* src, __m128i* dst)
  {
    __m128i tmp = _mm_unpacklo_epi8(*src, *(src + 1));
    *(dst + 1) = _mm_unpackhi_epi8(*src, *(src + 1));
    _mm_store_si128(dst, tmp);
  }

};

void censusTransformSSE(const Image& im, const CensusCfg& cfg, Image& rResult)
{
  Image census(rResult.rows, rResult.cols, rResult.pxStep, rResult.offset);
  //Copy pattern to local var
  std::vector<int> offsetsLUT;
  offsetsLUT.insert(offsetsLUT.end(), cfg.pattern.begin(), cfg.pattern.end());
  //Convert offset terms from bytes to __m128i

  int edgeSize = static_cast<int>(cfg.patternSize * .5);
  int pxSize = cfg.pattern.size() / 8;

  __m128i bitconst = _mm_set1_epi8(1);
  __m128i bitmask;

  for(int i = edgeSize; i < im.rows - edgeSize; ++i)
  {
    for(int j = 0; j < im.stride; j += 16)//Will always divide evenly
    {
      //struct 'Image' ensures that each row is always aligned
      __m128i* resultPtr = (__m128i*)(census.at(i, j));
      __m128i* objectPx = (__m128i*)im.at(i, j);
    
      bitmask = bitconst;
      int bitCount = 0;
      for(int k = 0; k < static_cast<int>(offsetsLUT.size()); ++k)
      {
        //Load the next pixel of each center pixel's sampling window
        __m128i samplePx = _mm_loadu_si128((__m128i*)(im.at(i, j) + offsetsLUT[k]));
        //do comparison
        //TRICKY To do 16 at a time, it's necessary to do some funny business, because _mm_cmpgt_epi8 is signed only.
        *resultPtr = _mm_or_si128(*resultPtr, _mm_and_si128(_mm_cmpeq_epi8(_mm_max_epu8(samplePx, *objectPx), samplePx), bitmask));

        bitmask = _mm_add_epi8(bitmask, bitmask);//multiply bitmask by 2, to target the next bit of each descriptor
        ++bitCount;
        if(bitCount == 8)
        {
          ++resultPtr;
          bitmask = bitconst;
          bitCount = 0;
        }
      }
      //Store the result into census
      if(cfg.type == SPARSE_16)
        storeSSE16((__m128i*)census.at(i, j), (__m128i*)census.at(i, j));
    }
  }
  rResult = census;
}

void censusTransformScalar(const Image& im, const CensusCfg& cfg, Image& rResult)
{
  Image census(rResult.rows, rResult.cols, rResult.pxStep, rResult.offset);
  int edgeSize = static_cast<int>(cfg.patternSize * .5);
  int pxSize = cfg.pattern.size() / 8;

  byte* resultPtr;

  for(int i = edgeSize; i < im.rows - edgeSize; ++i)
  {
    //Set resultPtr to beginning of the row
    resultPtr = census.at(i, edgeSize * pxSize);
    for(int j = edgeSize; j < im.cols - edgeSize; ++j)
    {
      //Now we have chosen a pixel to examine
      int bitCount = 0;
      censusTransformSinglePx(im.at(i, j), cfg.pattern, &resultPtr);
    }
  }
  rResult = census;
}
