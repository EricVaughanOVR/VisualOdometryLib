#include "CorrespondenceDefs.hpp"
#include <xmmintrin.h>
#include <string.h>

namespace 
{
  void lut_sparse8(std::vector<int>& offsets, const int stride, const int pxStep)
  {
    offsets.resize(8);
    offsets[0] = -4 * stride;
    offsets[1] = -3 * stride - 3 * pxStep;
    offsets[2] = -3 * stride + 2 * pxStep;
    offsets[3] = -4;
    offsets[4] = 3;
    offsets[5] = 2 * stride + 3 * pxStep;
    offsets[6] = 3 * stride - 3 * pxStep;
    offsets[7] = 3 * stride;
  }

  void lut_sparse16(std::vector<int>& offsets, const int stride, const int pxStep)
  {
    offsets.resize(16);
    offsets[0] = -4 * stride;
    offsets[1] = -3 * stride - 2 * pxStep;
    offsets[2] = -3 * stride + 2 * pxStep;
    offsets[3] = -2 * stride - 3 * pxStep;
    offsets[4] = -2 * stride - 2 * pxStep;
    offsets[5] = -2 * stride + 2 * pxStep;
    offsets[6] = -2 * stride + 4 * pxStep;
    offsets[7] = -4;
    offsets[8] = 3;
    offsets[9] = 2 * stride - 3 * pxStep;
    offsets[10] = 2 * stride + 4 * pxStep;
    offsets[11] = 3 * stride - 3 * pxStep;
    offsets[12] = 3 * stride;
    offsets[13] = 3 * stride + 3 * pxStep;
    offsets[14] = 4 * stride - 2 * pxStep;
    offsets[15] = 4 * stride + 2 * pxStep;
  }

  void lut_dense(std::vector<int>& offsets, const int stride, const int pxStep, 
                 const int rows, const int cols)
  {
    offsets.resize(rows * cols);
    int row = static_cast<int>(-rows * .5);
    int col = static_cast<int>(-cols * .5);
    for(int i = 0; i < static_cast<int>(offsets.size()); ++i)
    {
      offsets[i] = row * stride + col * pxStep;
      if(col < static_cast<int>(cols * .5) - 1)
      {
        ++col;
      }
      else
      {
        col = static_cast<int>(-.5 * cols);
        ++row;
      }
    }
  }

};
namespace correspondence
{
  //Begin Image class
  Image::Image(const int _rows, const int _cols, const int _pxStep, const pt _offset)
    : rows(_rows),
      cols(_cols),
      pxStep(_pxStep),
      stride(_cols * _pxStep + 16 - ((_cols * _pxStep) % 16)),
      offset(_offset)
  {
    data = (byte*)_mm_malloc(stride * sizeof(byte) * rows * pxStep, 16);
    zeroMem();
  }

  //Copies the supplied data into this object's aligned memory
  Image::Image(const int _rows, const int _cols, const int _pxStep, const pt _offset, 
    const byte* _data)
    : rows(_rows),
      cols(_cols),
      pxStep(_pxStep),
      stride(_cols * _pxStep + 16 - ((_cols * _pxStep) % 16)),
      offset(_offset)
  {
    data = (byte*)_mm_malloc(stride * rows * pxStep, 16);
    byte* dataPtr = data;
    int offset = 0;
    for(int i = 0; i < rows; ++i)
    {
      memcpy(dataPtr, _data + offset, _cols);
      offset += _cols * sizeof(byte);
      dataPtr += stride;
    }
  }

  Image::~Image()
  {
    _mm_free(data);
  }

  Image& Image::operator=(const Image& other)
  {
    if(other.offset.x != 0 || other.offset.y != 0)
      return *this;
    if(rows != other.rows || cols != other.cols || stride != other.stride ||
      pxStep != other.pxStep)
    {
      _mm_free(data);
      data = (byte*)_mm_malloc(other.stride * other.rows * other.pxStep, 16);
      rows = other.rows;
      cols = other.cols;
      stride = other.stride;
      offset = other.offset;
      pxStep = other.pxStep;
    }
    //TODO Figure out a more elgant way to update from a buffer while keeping data aligned.  Probably smart pointers.
    memcpy(data, other.data, other.stride * other.rows * other.pxStep);
    return *this;
  }

  void Image::zeroMem()
  {
    memset(data, 0, stride * rows * sizeof(byte));     
  }
  //End Image class

  //Begin MatchingParams struct
  MatchingParams::MatchingParams()
    : mode(STEREO),
      corrType(DENSECW_11),
      filterDist(20),//TODO normalize to descriptor size
      maxDisparity(0),
      epipolarRange(1)
  {
  }

  MatchingParams::MatchingParams(const eMatchMode _mode, const eCorrelationWindow _corrType, const int _maxDisparity, 
    const int _epipolarRange, const int _stride, const int _pxStep)
    : mode(_mode),
      corrType(_corrType),
      maxDisparity(_maxDisparity),
      epipolarRange(_epipolarRange)
  {
    prepCorrLUT(_pxStep, _stride);
  }

    

  void MatchingParams::prepCorrLUT(const int _pxStep, const int _stride)
  {
    switch(corrType)
    {
    case DENSECW_13 :
      lut_dense(pattern, _stride, _pxStep, 12, 13);
      windowSize = 13;
      edgeSize = 6;
      break;
    case DENSECW_11 :
      lut_dense(pattern, _stride, _pxStep, 11, 12);
      windowSize = 12;
      edgeSize = 6;
      break;
    case DENSECW_9 :
      lut_dense(pattern, _stride, _pxStep, 8, 9);
      windowSize = 9;
      edgeSize = 4;
      break;
    case DENSECW_7 : 
      lut_dense(pattern, _stride, _pxStep, 7, 8);
      windowSize = 8;
      edgeSize = 4;
      break;
    case DENSECW_5 :
      lut_dense(pattern, _stride, _pxStep, 4, 5);
      windowSize = 5;
      edgeSize = 2;
      break;
    case SPARSECW_16 :
      lut_sparse16(pattern, _pxStep, _stride);
      windowSize = 9;
      edgeSize = 4;
      break;
    case SPARSECW_8 :
      lut_sparse8(pattern, _pxStep, _stride);
      windowSize = 9;
      edgeSize = 4;
    };
  }
  //End MatchingParams struct

  //Begin CensusCfg class
  CensusCfg::CensusCfg()
    : type(SPARSE_16),
      imgRows(0),
      imgCols(0),
      imgStride(0)
  {}

  CensusCfg::CensusCfg(const eSamplingPattern _type, const int _rows, const int _cols, const int _stride, const int _pxStep)
    : type(_type),
      imgRows(_rows),
      imgCols(_cols),
      imgStride(_stride)
  {
    prepSamplingLUT(_pxStep);
  }

  void CensusCfg::prepSamplingLUT(const int _pxStep)
  {
    switch(type)
    {
    case SPARSE_8 :
      patternSize = 9;
      edgeSize = 4;
      lut_sparse8(pattern, imgStride, _pxStep);
      break;
    case SPARSE_16 :
      patternSize = 9;
      edgeSize = 4;
      lut_sparse16(pattern, imgStride, _pxStep);
      break;
    };
  } 
  //End CensusCfg class

};//namespace correspondence