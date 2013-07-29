#include "Match.hpp"

using namespace correspondence;

void matchDense(const correspondence::Descriptors& censusIm1, const correspondence::Descriptors& censusIm2,
                const correspondence::eMatchMode, const correspondence::eSamplingPattern, 
                std::vector<correspondence::Match>& rMatches)
{
}

void matchSparse(const correspondence::Descriptors& censusIm1, const correspondence::Descriptors& censusIm2,
                 const correspondence::eMatchMode, const correspondence::eSamplingPattern, 
                 const std::vector<correspondence::Feature>& kps1, 
                 const std::vector<correspondence::Feature>& kps2,
                 std::vector<correspondence::Match>& rMatches)
{
}

void prepRegion(const correspondence::Descriptors& censusIm2, const correspondence::Feature& feature1, 
                correspondence::Descriptors& rSubImage)
{
}

uint32_t calcHammingDist(const uint32_t _1, const uint32_t _2)
{
  return 0;
}

uint32_t calcSHD(const correspondence::Image& region1, const correspondence::Image& region2)
{
  return 0;
}

correspondence::Match matchFeature(const correspondence::Descriptors& im1, const correspondence::Descriptors& im2, 
                                  const correspondence::Feature& kp1, 
                                  const std::vector<correspondence::Feature>& kps2)
{
  Match match;
  return match;
}