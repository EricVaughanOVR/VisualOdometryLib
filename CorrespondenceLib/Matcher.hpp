#ifndef MATCH_HPP
#define MATCH_HPP

#include "CorrespondenceDefs.hpp"
#include <xmmintrin.h>

class Matcher
{
public:
  Matcher();

  Matcher(const correspondence::CensusCfg& _cfg, const correspondence::MatchingParams& _params,
          int _rows, int _cols);

  ~Matcher();
  
  void matchDense(const correspondence::Image& censusIm1, const correspondence::Image& censusIm2,
                  std::vector<correspondence::Match>& rMatches);

  void matchSparse(const correspondence::Image& censusIm1, const correspondence::Image& censusIm2,
                    const correspondence::FeatureList& kps1, correspondence::FeatureList& kps2,
                    std::vector<correspondence::Match>& rMatches);

private:
  void getPotentialMatches(const correspondence::Feature& kp1, correspondence::FeatureList& kps2, 
                           std::vector<correspondence::KpRow>& rPotMatches);

  /** Use this if !SSSE3 and !SSE4
    *
    */
  uint32_t calcHammingDist(const uint16_t _1, const uint16_t _2);

  /** Use this if SSSE3, if !SSSE3, but SSE4, use _mm_popcount_u32, instead
    *
    */
  uint32_t calcHammingDistSSE(__m128i _1, __m128i _2);

  uint32_t calcSHD(const correspondence::Image& census1, const correspondence::Image& census2,
                   const correspondence::Feature& kp1, const correspondence::Feature& kp2);

  correspondence::Match matchFeature(const correspondence::Image& census1, const correspondence::Feature& kp1, 
                                     const correspondence::Image& census2, 
                                     const std::vector<correspondence::KpRow>& potMatches);

  correspondence::MatchingParams params;
  correspondence::CensusCfg cfg;
  correspondence::Image hammingTfm;
};
#endif//MATCH_HPP
