#include "CorrespondenceDefs.hpp"

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
  };

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
  };

  Image::~Image()
  {
    _mm_free(data);
  }

  void Image::zeroMem()
  {
    memset(data, 0, stride * rows * sizeof(byte));     
  };
  //End Image class

  //Begin MatchingParams struct
  MatchingParams::MatchingParams()
    : mode(STEREO),
      corrType(DENSE_11),
      filterDist(20),//TODO normalize to descriptor size
      maxDisparity(0),
      epipolarRange(1)
  {
    init();
  };

  MatchingParams::MatchingParams(eMatchMode _mode, eCorrelationWindow _corrType, int _filterDist, 
                  int _maxDisparity, int _epipolarRange)
    : mode(_mode),
      corrType(_corrType),
      filterDist(_filterDist),
      maxDisparity(_maxDisparity),
      epipolarRange(_epipolarRange)
  {
    init();
  };

  void MatchingParams::init()
  {
    switch(mode)
    {
    case DENSE_13 :
      windowSize = 13;
      edgeSize = 6;
      break;
    case DENSE_11 :
      windowSize = 11;
      edgeSize = 5;
      break;
    case DENSE_9 :
      windowSize = 9;
      edgeSize = 4;
      break;
    case DENSE_7 : 
      windowSize = 7;
      edgeSize = 3;
      break;
    case DENSE_5 :
      windowSize = 15;
      edgeSize = 2;
      break;
    };
  };
  //End MatchingParams struct

  //Begin CensusCfg class
  CensusCfg::CensusCfg()
    : type(SPARSE_16),
      imgRows(0),
      imgCols(0),
      imgStride(0)
  {};

  CensusCfg::CensusCfg(eSamplingPattern _type, int _rows, int _cols, int _stride)
    : type(_type),
      imgRows(_rows),
      imgCols(_cols),
      imgStride(_stride)
  {
    prepOffsetsLUT();
  };

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
  void CensusCfg::prepOffsetsLUT()
  {
    switch(type)
    {
    case SPARSE_8 :
      patternSize = 9;
      edgeSize = 4;
      lut_sparse8(pattern, imgStride);
      break;
    case SPARSE_16 :
      patternSize = 9;
      edgeSize = 4;
      lut_sparse16(pattern, imgStride);
      break;
    case DENSE_3 :
      patternSize = 3;
      edgeSize = 1;
      lut_dense(pattern, imgStride, patternSize);
    };
  } 
  //End CensusCfg class

};//namespace correspondence