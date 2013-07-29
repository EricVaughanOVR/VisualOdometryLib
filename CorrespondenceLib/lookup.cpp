#include "Census.hpp"

using namespace correspondence;

void prepOffsetsLUT(correspondence::eSamplingPattern type, std::vector<int>& offsets, int& windowSize,
                    const int stride)
{
  if(type == SPARSE_8)
  {
    lut_sparse8(offsets, stride);
    windowSize = 9;
  }
  else if(type == SPARSE_16)
  {
    lut_sparse16(offsets, stride);
    windowSize = 9;
  }
  else if(type == DENSE_3)
  {
    lut_dense(offsets, stride, windowSize);
  }
}

void prepOffsetsLUT(correspondence::eCorrelationWindow type, std::vector<int>& offsets, int& windowSize,
                    const int stride)
{
  switch(type)
  {
  case DENSE_5 :
    windowSize = 5;
    break;
  case DENSE_7 :
    windowSize = 7;
    break;
  case DENSE_9 :
    windowSize = 9;
    break;
  case DENSE_11 :
    windowSize = 11;
    break;
  case DENSE_13 :
    windowSize = 13;
  }
    lut_dense(offsets, stride, windowSize);
}

void lut_sparse8(std::vector<int>& offsets, const int stride)
{
  offsets.resize(8);
  offsets[0] = -4 * stride;
  offsets[1] = -3 * stride - 3;
  offsets[2] = -3 * stride + 2;
  offsets[3] = -4;
  offsets[4] = 3;
  offsets[5] = 2 * stride + 3;
  offsets[6] = 3 * stride - 3;
  offsets[7] = 3 * stride;
}

void lut_sparse16(std::vector<int>& offsets, const int stride)
{
  offsets.resize(16);
  offsets[0] = -4 * stride;
  offsets[1] = -3 * stride - 2;
  offsets[2] = -3 * stride + 2;
  offsets[3] = -2 * stride - 3;
  offsets[4] = -2 * stride - 2;
  offsets[5] = -2 * stride + 2;
  offsets[6] = -2 * stride + 4;
  offsets[7] = -4;
  offsets[8] = 3;
  offsets[9] = 2 * stride - 3;
  offsets[10] = 2 * stride + 4;
  offsets[11] = 3 * stride - 3;
  offsets[12] = 3 * stride;
  offsets[13] = 3 * stride + 3;
  offsets[14] = 4 * stride - 2;
  offsets[15] = 4 * stride + 2;
}

void lut_dense(std::vector<int>& offsets, const int stride, const int size)
{
  offsets.resize(size * size - 1);
  int row = static_cast<int>(-size * .5);
  int col = row;
  for(int i = 0; i < static_cast<int>(offsets.size()); ++i)
  {
    if(i == static_cast<int>(offsets.size() * .5))
      ++col;
    offsets[i] = row * stride + col;
    if(col < static_cast<int>(size * .5))
    {
      ++col;
    }
    else
    {
      col = static_cast<int>(-.5 * size);
      ++row;
    }
  }
}
