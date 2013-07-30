#ifndef MATCH_HPP
#define MATCH_HPP

#include "CorrespondenceDefs.hpp"

void matchDense(const correspondence::Descriptors& censusIm1, const correspondence::Descriptors& censusIm2,
                const correspondence::eMatchMode, const correspondence::eSamplingPattern, 
                std::vector<correspondence::Match>& rMatches);

void matchSparse(const correspondence::Descriptors& censusIm1, const correspondence::Descriptors& censusIm2,
                 const correspondence::MatchingParams& params, const correspondence::CensusCfg& cfg, 
                 const std::vector<correspondence::Feature>& kps1, 
                 const std::vector<correspondence::Feature>& kps2,
                 std::vector<correspondence::Match>& rMatches);

void getPotentialMatches(const correspondence::Feature& kp1, const correspondence::Image& censusIm2,
                correspondence::Descriptors& rPotMatches);

uint32_t calcHammingDist(const uint32_t _1, const uint32_t _2);

uint32_t calcSHD(const correspondence::Image& im1, const correspondence::Image& im2,
                 const correspondence::Feature& kp1, const correspondence::Feature& kp2);

correspondence::Match matchFeature(const correspondence::Image& im1, const correspondence::Feature& kp1, 
                                   const correspondence::Descriptors& kps2);

#endif
