#include "Matcher.hpp"
#include <smmintrin.h>

using namespace correspondence;

Matcher::Matcher()
{
};

Matcher::Matcher(const CensusCfg& _cfg, const MatchingParams& _params, int _rows, int _cols)
  : cfg(_cfg),
    params(_params)
{
}

Matcher::~Matcher()
{
}

void Matcher::matchDense(const Image& censusIm1, const Image& censusIm2,
                         Image& rDenseMap)
{
  //TODO Implement the moving window technique for computing dense disparity maps, as described in 'Fast Census Transform-based Stereo Algorithm using SSE2'
  for(int i = params.edgeSize; i < censusIm1.rows - params.edgeSize; ++i)//For each row
  {
    for(int j = params.edgeSize; j < censusIm1.cols - params.edgeSize; ++j)//For each col
    {
      Feature kp1, kp2;
      kp1.x = j;
      kp1.y = i;
      kp2.x = j;
      kp2.y = i;

      int bestMatch = 3000, secondBestMatch = 3000, bestScore = 3000, secondBestScore = 3000;//These store the disparity and score of the 2 best matches for the left-image pixel
      for(int k = 0; k < params.maxDisparity; ++k)//For each potential match
      {
        kp2.x = j - k;

        int thisScore = calcSHD(censusIm1, censusIm2, kp1, kp2);
        if(thisScore <= bestScore)
        {
          secondBestMatch = bestMatch;
          secondBestScore = bestScore;
          bestMatch = k;
          bestScore = thisScore;
        }
        else if(thisScore < secondBestScore)
        {
          secondBestMatch = k;
          secondBestScore = thisScore;
        }
      }
      //if(static_cast<float>(bestScore) < static_cast<float>(0.7 * secondBestScore))
        *rDenseMap.at(i, j) = bestMatch * (255 / params.maxDisparity);
    }
  }

}

void Matcher::matchSparse(const Image& censusIm1, const Image& censusIm2, const FeatureList& kps1, 
                 FeatureList& kps2, std::vector<Match>& rMatches, const int flowWindow)
{
  //1. Loop through each feature from the first image
  //2. For each feature, use getPotentialMatches to get a list of possible matches, (Features) from the second image
  //3. Compare each possible match to the left-hand feature, using calcSHD
  //4. Keep track of the right-hand Feature with the smallest SHD
  //5. At the end, if the best feature is within epsilon (normalize for descriptor length) then add it to rMatches

  rMatches.clear();
  rMatches.reserve(kps1.nonmaxFeatures.size());

  for(size_t i = 0; i < kps1.nonmaxFeatures.size(); ++i)//For each left-hand Feature
  {
    //If kp1 is too close to edge of img
    if(kps1.nonmaxFeatures[i].y < params.edgeSize || kps1.nonmaxFeatures[i].y > cfg.imgRows - params.edgeSize ||
      kps1.nonmaxFeatures[i].x < params.edgeSize || kps1.nonmaxFeatures[i].x > cfg.imgCols - params.edgeSize)
      continue;
    std::vector<KpRow> potMatches;
    getPotentialMatches(kps1.nonmaxFeatures[i], kps2, potMatches, flowWindow);
    
    //Call matchFeature, to get the best match from the pool of potential matches
    if(!potMatches.empty())
    {
      Match match = matchFeature(censusIm1, kps1.nonmaxFeatures[i], censusIm2, potMatches);
      //If the match is high-enough quality, add it to rMatches
      //TODO add uniqueness check
      if(match.dist < params.filterDist)
        rMatches.push_back(match);
    }
  }
}

void Matcher::getPotentialMatches(const Feature& kp1, FeatureList& kps2, std::vector<KpRow>& rPotMatches, const int flowWindow)
{
  rPotMatches.clear();
  int firstRow, lastRow, firstCol, lastCol, epipolar;
  //1. Given a matching mode and correlationWindowType, determine the image region that encloses each of the required pixels
  //2. If Stereo, choose an epipolar region, and provide room for the size of the correlation window of each contained Feature
  //3. If Flow, choose a region surrounding the left-hand Feature and capture potential matches within it.  Then, capture all of the pixels required for an SHD of each potential match
  if(params.mode == STEREO)
  {
    epipolar = static_cast<int>(params.epipolarRange * .5);
    firstRow = kp1.y - epipolar;
    lastRow = kp1.y + epipolar;
    firstCol = kp1.x - params.maxDisparity;
    lastCol = kp1.x;

    if(firstRow < params.edgeSize + 1)
      firstRow = params.edgeSize + 1;
    if(lastRow > cfg.imgRows - params.edgeSize)
      lastRow = cfg.imgRows - params.edgeSize;
    if(firstCol < params.edgeSize + 1)
      firstCol = params.edgeSize + 1;
    if(lastCol > cfg.imgCols - params.edgeSize)
      lastCol = cfg.imgCols - params.edgeSize;
  }
  else
  {
    int radius = static_cast<int>(flowWindow * .5);
    firstRow = kp1.y - radius;
    lastRow = kp1.y + radius;
    firstCol = kp1.x - radius;
    lastCol = kp1.x + radius;

    if(firstRow < radius - 1)
      firstRow = radius - 1;
    if(lastRow > cfg.imgRows - radius)
      lastRow = cfg.imgRows - radius;
    if(firstCol < radius - 1)
      firstCol = radius - 1;
    if(lastCol > cfg.imgCols - radius)
      lastCol = cfg.imgCols - radius;
  }

  int rowCount = 0;
  int numRows = lastRow - firstRow + 1;
  rPotMatches.reserve(numRows);

  for(int i = firstRow; i <= lastRow; ++i)
  {
    if(kps2.rowIdxs[i] < 0)//If the row is empty
      continue;
    std::vector<Feature>::iterator iter = kps2.allFeatures.begin() + kps2.rowIdxs[i];

    bool beginSet = false;
    while(iter != kps2.allFeatures.end() && iter->x <= lastCol && iter->y == i)
    {
      if(iter->x >= firstCol)
      {
        if(!beginSet)
        {
          beginSet = true;
          KpRow row;
          row.begin = iter;
          row.end = iter;
          rPotMatches.push_back(row);
        }
        else if(rPotMatches.back().end->x < iter->x)
          rPotMatches.back().end = iter;
      }
      ++iter;
    }
  }
}

namespace
{
  uint32_t calcSHD_1B(const Image& census1, const Image& census2,
                 const Feature& kp1, const Feature& kp2, const MatchingParams& params)
  {
    int step = 4;
    int totalDist = 0;

    uint8_t* pxL = census1.at(kp1.y, kp1.x);
    uint8_t* pxR = census2.at(kp2.y, kp2.x);

    for(int i = 0; i < params.pattern.size(); i += step)
    {
      uint32_t l = *(pxL + params.pattern[i]) << 24 | *(pxL + params.pattern[i + 1]) << 16 |
        *(pxL + params.pattern[i + 2]) << 8 | *(pxL + params.pattern[i + 3]);
      uint32_t r = *(pxR + params.pattern[i])  << 24 | *(pxR + params.pattern[i + 1]) << 16 |
        *(pxR + params.pattern[i + 2]) << 8 | *(pxR + params.pattern[i + 3]);

      totalDist += _mm_popcnt_u32(l^r);
    }

    return totalDist;
  }

  uint32_t calcSHD_2B(const Image& census1, const Image& census2,
                 const Feature& kp1, const Feature& kp2, const MatchingParams& params)
  {
    int step = 2;
    int totalDist = 0;

    uint8_t* pxL = census1.at(kp1.y, kp1.x);
    uint8_t* pxR = census2.at(kp2.y, kp2.x);

    for(int i = 0; i < params.pattern.size(); i += step)
    {
      
      uint32_t l = *(pxL + params.pattern[i]) << 24 | *(pxL + params.pattern[i] + 1) << 16 |
        *(pxL + params.pattern[i + 1]) << 8 | *(pxL + params.pattern[i + 1] + 1);
      uint32_t r = *(pxR + params.pattern[i]) << 24 | *(pxR + params.pattern[i] + 1) << 16 |
        *(pxR + params.pattern[i + 1]) << 8 | *(pxR + params.pattern[i + 1] + 1);
        
      totalDist += _mm_popcnt_u32(l^r);
    }

    return totalDist;
  }

};

uint32_t Matcher::calcSHD(const Image& census1, const Image& census2,
                 const Feature& kp1, const Feature& kp2)
{
  //1. For each pixel in the correlationWindow of each image, calculate the Hamming Distance and add it to the total
  //2. When finished, return the total
  
  uint32_t totalDist;

  //TRICKY only supporting pxStep of either 2 or 4 right now
  if(census1.pxStep == 2)
    totalDist = calcSHD_2B(census1, census2, kp1, kp2, params);
  else if(census2.pxStep == 1)
    totalDist = calcSHD_1B(census1, census2, kp1, kp2, params);

  return totalDist;
}

Match Matcher::matchFeature(const Image& census1, const Feature& kp1, const Image& census2, 
                                   const std::vector<KpRow>& potMatches)
{
  //1. For each possible matching Feature from the right-hand window, compute the SHD
  //2. Compare the latest SHD to the minimum, if the new one is less, update it and the Feature that it is associated with
  Match bestMatch;
  bestMatch.dist = 3000;//Higher than any possible distance, so it will be given a valid value immediately
  bestMatch.feature1Idx = kp1.idx;
  
  for(size_t i = 0; i < potMatches.size(); ++i)
  {
    for(std::vector<Feature>::iterator iter = potMatches[i].begin; iter != potMatches[i].end; ++iter)
    {
      //Use FAST score to reject obviously bad matches out-of-hand
      if((kp1.score < 0 && iter->score > 0) || (kp1.score > 0 && iter->score < 0))
        continue;
      int dist = static_cast<int>(calcSHD(census1, census2, kp1, *iter));
      if(bestMatch.dist > dist)
      {
        bestMatch.dist = dist;
        bestMatch.feature2Idx = iter->idx;
        if(bestMatch.dist == 0)
          break;
      }
    }
  }
  
  return bestMatch;
}