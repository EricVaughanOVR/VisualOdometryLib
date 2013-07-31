#include "Match.hpp"

using namespace correspondence;

void matchDense(const Image& censusIm1, const Image& censusIm2,
                const CensusCfg& cfg, std::vector<correspondence::Match>& rMatches)
{
  //Implement the moving window technique for computing dense disparity maps, as described in 'Fast Census Transform-based Stereo Algorithm using SSE2'
}

void matchSparse(const Image& censusIm1, const Image& censusIm2, const CensusCfg& cfg, 
                 const FeatureList& kps1, FeatureList& kps2, std::vector<Match>& rMatches)
{
  //1. Loop through each feature from the first image
  //2. For each feature, usePrepRegion to get a list of possible matches, (Features) from the second image
  //3. Compare each possible match to the left-hand feature, using calcSHD
  //4. Keep track of the right-hand Feature with the smallest SHD
  //5. At the end, if the best feature is within epsilon (normalize for descriptor length) then add it to rMatches

  for(size_t i = 0; i < kps1.nonmaxFeatures.size(); ++i)//For each left-hand Feature
  {
    std::vector<KpRow> potMatches;
    getPotentialMatches(kps1.nonmaxFeatures[i], kps2, cfg, potMatches);

    //Call matchFeature, to get the best match from the pool of potential matches

    //If the match is high-enough quality, add it to rMatches
  }
}

void getPotentialStereo(const Feature& kp1, FeatureList& kps2, const CensusCfg& cfg, std::vector<KpRow>& rPotMatches)
{
  //What are the beginning and ending row numbers?
  //What are the beginning and ending columns?
  int epipolar = static_cast<int>(cfg.params.epipolarRange * .5);
  int firstRow = kp1.y - epipolar;
  int lastRow = kp1.y + epipolar;
  int firstCol = kp1.x - cfg.params.maxDisparity;
  int lastCol = kp1.x;

  int edgeSize = static_cast<int>(cfg.windowSize * .5);
  if(firstRow < edgeSize - 1)
    firstRow = edgeSize - 1;
  if(lastRow > cfg.imgRows + edgeSize)
    lastRow = cfg.imgRows + edgeSize;
  if(firstCol < edgeSize - 1)
    firstCol = edgeSize - 1;
  if(lastCol > cfg.imgCols + edgeSize)
    lastCol = cfg.imgCols + edgeSize;


  rPotMatches.clear();
  int numRows = lastRow - firstRow + 1;
  rPotMatches.reserve(numRows);

  for(int i = 0; i < numRows; ++i)
  {
    KpRow row;
    std::vector<Feature>::iterator iter = kps2.allFeatures.begin() + kps2.rowIdxs[kp1.y];
    while(iter->x < firstCol)
      ++iter;
    if(iter->x < lastCol)
      row.begin = iter;
    while(iter->x < lastCol)
      ++iter;
    row.end = --iter;
    rPotMatches.push_back(row);
  }
}

void getPotentialFlow(const Feature& kp1, FeatureList& kps2, const CensusCfg& cfg, 
                      std::vector<KpRow>& rPotMatches)
{

}

void getPotentialMatches(const Feature& kp1, FeatureList& kps2, const CensusCfg& cfg, 
                         std::vector<KpRow>& rPotMatches)
{
  //1. Given a matching mode and correlationWindowType, determine the image region that encloses each of the required pixels
  //2. If Stereo, choose an epipolar region, and provide room for the size of the correlation window of each contained Feature
  //3. If Flow, choose a region surrounding the left-hand Feature and capture potential matches within it.  Then, capture all of the pixels required for an SHD of each potential match
  if(cfg.params.mode == STEREO)
    getPotentialStereo(kp1, kps2, cfg, rPotMatches);
  else
    getPotentialFlow(kp1, kps2, cfg, rPotMatches);
}

uint32_t calcHammingDist(const uint16_t _1, const uint16_t _2)
{
  //XOR the desc
  uint32_t newBitStr = _1 ^ _2;

  //From Bit Twiddling Hacks
  uint32_t result; // store the total here
  static const int S[] = {1, 2, 4, 8, 16}; // Magic Binary Numbers
  static const int B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF};

  result = newBitStr - ((newBitStr >> 1) & B[0]);
  result = ((result >> S[1]) & B[1]) + (result & B[1]);
  result = ((result >> S[2]) + result) & B[2];
  result = ((result >> S[3]) + result) & B[3];
  result = ((result >> S[4]) + result) & B[4];

  return result;
}

int calcHammingDistSSE(__m128i* _1, __m128i* _2, __m128i* _mask_lo, __m128i* _mask_popcnt)
{
  //Use this one if PSHUFB
  //Use PSHUFB to calculate the popcount, with a 4-bit LUT
  __m128i v = _mm_xor_si128(*_1, *_2);
  __m128i lo = _mm_and_si128(v, *_mask_lo);
  __m128i hi = _mm_and_si128(_mm_srli_epi16(v, 4), *_mask_lo);
  lo = _mm_shuffle_epi8(*_mask_popcnt, lo);
  hi = _mm_shuffle_epi8(*_mask_popcnt, hi);
  //_mm_add_
  _mm_add_epi8(lo, hi);

  return 0;
}

uint32_t calcSHD(const Image& region1, const Image& region2,
                 const Feature& kp1, const Feature& kp2)
{
  //1. For each pixel in the correlationWindow of each image, calculate the Hamming Distance and add it to the total
  //2. When finished, return the total
  return 0;
}

correspondence::Match matchFeature(const Image& im1, const Feature& kp1, const std::vector<KpRow>& kps2)
{
  //1. For each possible matching Feature from the right-hand window, compute the SHD
  //2. Compare the latest SHD to the minimum, if the new one is less, update it and the Feature that it is associated with
  Match match;
  return match;
}