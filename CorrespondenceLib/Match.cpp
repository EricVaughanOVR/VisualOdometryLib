#include "Match.hpp"

using namespace correspondence;

void matchDense(const correspondence::Descriptors& censusIm1, const correspondence::Descriptors& censusIm2,
                const correspondence::eMatchMode, const correspondence::eCorrelationWindow, 
                std::vector<correspondence::Match>& rMatches)
{
  //Implement the moving window technique for computing dense disparity maps, as described in 'Fast Census Transform-based Stereo Algorithm using SSE2'
}

void matchSparse(const Descriptors& censusIm1, const Descriptors& censusIm2,
                 const MatchingParams& params, const CensusCfg& cfg, const eSamplingPattern pattern,
                 const std::vector<Feature>& kps1, 
                 const std::vector<Feature>& kps2,
                 std::vector<Match>& rMatches)
{
  //1. Loop through each feature from the first image
  //2. For each feature, usePrepRegion to get a list of possible matches, (Features) from the second image
  //3. Compare each possible match to the left-hand feature, using calcSHD
  //4. Keep track of the right-hand Feature with the smallest SHD
  //5. At the end, if the best feature is within epsilon (normalize for descriptor length) then add it to rMatches

  for(size_t i = 0; i < kps1.size(); ++i)//For each left-hand Feature
  {
    //Call PrepRegion for the current feature
      //Create subregion to look for matches within

      //get a list of potentialMatches(Features)

    //Call matchFeature, to get the best match from the pool of potential matches

    //If the match is high-enough quality, add it to rMatches
  }
}

void getPotentialStereo(const Feature& kp1, const Image& censusIm2, Descriptors& rPotMatches, const MatchingParams& params)
{
  //What are the beginning and ending rows?
  //What are the beginning and ending columns?
}

void getPotentialFlow(const Feature& kp1, const Image& censusIm2, Descriptors& rPotMatches, const MatchingParams& params)
{

}

void getPotentialMatches(const Feature& kp1, const Image& censusIm2, Descriptors& rPotMatches, const MatchingParams& params)
{
  //1. Given a matching mode and correlationWindowType, determine the image region that encloses each of the required pixels
  //2. If Stereo, choose an epipolar region, and provide room for the size of the correlation window of each contained Feature
  //3. If Flow, choose a region surrounding the left-hand Feature and capture potential matches within it.  Then, capture all of the pixels required for an SHD of each potential match
  if(params.mode == STEREO)
    getPotentialStereo(kp1, censusIm2, rPotMatches, params);
  else
    getPotentialFlow(kp1, censusIm2, rPotMatches, params);
}

uint32_t calcHammingDist(const uint16_t _1, const uint16_t _2)
{
  //Use PSHUFB to calculate the popcount, with a 4-bit LUT
  return 0;
}

uint32_t calcSHD(const Image& region1, const Image& region2,
                 const Feature& kp1, const Feature& kp2)
{
  //1. For each pixel in the correlationWindow of each image, calculate the Hamming Distance and add it to the total
  //2. When finished, return the total
  return 0;
}

correspondence::Match matchFeature(const Image& im1, const Feature& kp1, const Descriptors& kps2)
{
  //1. For each possible matching Feature from the right-hand window, compute the SHD
  //2. Compare the latest SHD to the minimum, if the new one is less, update it and the Feature that it is associated with
  Match match;
  return match;
}