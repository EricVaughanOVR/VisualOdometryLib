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
void censusTranform(const correspondence::Image* im, correspondence::Image* output, 
                    const correspondence::CensusCfg& cfg);

//If SSE is available
//TRICKY: im MUST be 16-byte aligned or we will crash!
void censusTransformSSE(const correspondence::Image* im, correspondence::Image* output, 
                        const correspondence::CensusCfg& cfg);

//If SSE is not available
void censusTransformScalar(const correspondence::Image* im, correspondence::Image* output, 
                        const correspondence::CensusCfg& cfg);

//After calculating the current (i-th) bit of the census descriptor for the current group of 16 pixels,
//move the results into the census-image, by interleaving the __m128 vectors
//TRICKY: vect1 and vect2 MUST be 16-byte aligned or we will crash!
void storeSSE16(const correspondence::byte* vect1, const correspondence::byte& vect2, correspondence::byte* dst);

//Calculate the sampling offsets for the given pixel
void prepOffsetsLUT(correspondence::eSamplingWindow type, std::vector<int>& offsets);

void lut_Sparse8(std::vector<int>& offsets, const correspondence::Feature pt, const correspondence::Image& im);

void lut_Sparse16(std::vector<int>& offsets, const correspondence::Feature pt, const correspondence::Image& im);

void lut_dense3(std::vector<int>& offsets, const correspondence::Feature pt, const correspondence::Image& im);

void lut_dense5(std::vector<int>& offsets, const correspondence::Feature pt, const correspondence::Image& im);

void lut_dense7(std::vector<int>& offsets, const correspondence::Feature pt, const correspondence::Image& im);

void lut_dense9(std::vector<int>& offsets, const correspondence::Feature pt, const correspondence::Image& im);

void lut_dense11(std::vector<int>& offsets, const correspondence::Feature pt, const correspondence::Image& im);

void lut_dense13(std::vector<int>& offsets, const correspondence::Feature pt, const correspondence::Image& im);

#endif
