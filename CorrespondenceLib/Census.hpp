#ifndef CENSUS_HPP
#define CENSUS_HPP

#include "CorrespondenceDefs.hpp"

/*
  1. Select descriptor
  2. Perform Census Transform on each image (for every pixel, taking advantage of SIMD)
  3. Run FAST on the original (un-transformed) images
  4. Match the images
*/

//Determines the instruction set, and calls appropriate function
void censusTranform(const correspondence::Image& im, const correspondence::CensusCfg& cfg,
                    correspondence::Image& rResult);

//If SSE is available
//TRICKY: im MUST be 16-byte aligned or we will crash!
void censusTransformSSE(const correspondence::Image& im, const correspondence::CensusCfg& cfg,
                        correspondence::Image& rResult);

//If SSE is not available
void censusTransformScalar(const correspondence::Image& im, const correspondence::CensusCfg& cfg,
                           correspondence::Image& rResult);

inline void censusTransformSinglePx(const correspondence::byte* objectPx, const std::vector<int>& offsetsLUT,
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
      //**pResultPx = (**pResultPx + (*(objectPx + offsetsLUT[k]) > *objectPx));
      bitCount = 0;
      ++*pResultPx;
    }
  }
};

//After calculating the census descriptor for the current group of 16 pixels,
//move the results into the census-image, by interleaving the __m128 vectors
//TRICKY: vect1 and vect2 MUST be 16-byte aligned or we will crash!
void storeSSE16(__m128i* src, __m128i* dst);

//Calculate the sampling offsets for the given pixel
void prepOffsetsLUT(const correspondence::eSamplingPattern type, std::vector<int>& offsets, int& windowSize, 
                    const int stride);

void lut_sparse8(std::vector<int>& offsets, const int stride);

void lut_sparse16(std::vector<int>& offsets, const int stride);

void lut_dense(std::vector<int>& offsets, const int stride, const int size);

#endif
