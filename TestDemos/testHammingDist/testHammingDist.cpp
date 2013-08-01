#include "Match.hpp"
#include <iostream>

int main(int argc, char* argv)
{
  uint32_t distance = calcHammingDist(45, 46);

  __m128i v1 = {0, 0, 0, 0, 0, 0, 0, 0, 65, 0, 0, 0, 0, 0, 0, 45};
  __m128i v2 = {0, 0, 123, 0, 0, 3, 0, 0, 0, 2, 0, 0, 0, 0, 0, 34};

  __m128i XORed = _mm_xor_si128(v1, v2);
  int totSSE4 = 0;
  for(int i = 0; i < 4; ++i)
    totSSE4 += _mm_popcnt_u32(XORed.m128i_u32[i]);
  
  uint32_t totSSSE3 = calcHammingDistSSE(v1, v2);

  return totSSSE3;
}
