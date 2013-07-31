#include "Match.hpp"

int main(int argc, char* argv)
{
  uint32_t distance = calcHammingDist(45, 46);

  __m128i v1 = _mm_set1_epi8(45);
  __m128i v2 = _mm_set1_epi8(34);

  __m128i XORed = _mm_xor_si128(v1, v2);
  int tot = 0;
  for(int i = 0; i < 4; ++i)
    tot += _mm_popcnt_u32(XORed.m128i_u32[i]);

  return 0;
}
