#ifndef CENSUS_HPP
#define CENSUS_HPP

#include "CorrespondenceDefs.hpp"

//If SSE is available
//TRICKY: im MUST be 16-byte aligned or we will crash!
void censusTransformSSE(const correspondence::Image& im, const correspondence::CensusCfg& cfg,
                        correspondence::Image& rResult);

//If SSE is not available
void censusTransformScalar(const correspondence::Image& im, const correspondence::CensusCfg& cfg,
                           correspondence::Image& rResult);

#endif//CENSUS_HPP
