#include <iostream>
#include "Census.hpp"

using namespace correspondence;

int main(int argc, char* argv)
{
  byte* vector = (byte*)_mm_malloc(32 * sizeof(byte), 16);
  __m128i* v = (__m128i*)vector;
  *v = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
  *(v + 1) = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
  
  storeSSE16(v, v);

  byte* result = vector;
  bool success = true;
  for(int i = 0; i < 16; ++i)
  {
    if(i != (int)*(result++) || i != (int)*(result++))
      success = false;
  }

  (success) ? std::cout<<"testStoreSSE16 succeeded"<<std::endl : std::cout<<"testStoreSSE16 failed"<<std::endl;
 
  _mm_free(vector);
  std::cin.ignore();

  return 0;
}