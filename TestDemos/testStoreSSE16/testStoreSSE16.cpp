#include <iostream>
#include "Census.hpp"

using namespace correspondence;

int main(int argc, char* argv)
{
  __m128i vect1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  __m128i vect2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  byte* result = (byte*)_mm_malloc(32 * sizeof(byte), 16);
  storeSSE16(&vect1, &vect2, result);

  for(int i = 0; i < 32; ++i)
  {
    std::cout<<(int)*(result + i)<<std::endl;
  }
  _mm_free(result);
  std::cin.ignore();

  return 0;
}