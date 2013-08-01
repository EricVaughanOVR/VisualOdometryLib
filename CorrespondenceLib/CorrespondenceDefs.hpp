#ifndef CORRESPONDENCEDEFS_HPP
#define CORRESPONDENCEDEFS_HPP

#include <stdint.h>
#include <vector>
#include <memory>

namespace correspondence
{
  typedef uint8_t byte;

  struct pt
  {
    int x, y;
  };

  struct Image
  {
    int rows, cols, pxStep, stride;
    byte* data;
    pt offset;

    Image(const int _rows, const int _cols, const int _pxStep, const pt _offset)
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
    Image(const int _rows, const int _cols, const int _pxStep, const pt _offset, 
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

    ~Image()
    {
      _mm_free(data);
    }

    inline void zeroMem()
    {
      memset(data, 0, stride * rows * sizeof(byte));     
    };

    inline byte* at(const int _row, const int _col) const
    {
      //Note: only handles a positive value for stride
      return data + _row * stride + _col * pxStep;
    };
  };

  struct Feature
  {
    int x, y, idx, score;
  };

  struct Match
  {
    int feature1Idx, feature2Idx;
    int dist;
  };

  struct FeatureList
  {
    std::vector<Feature> allFeatures;
    std::vector<Feature> nonmaxFeatures;

    //Element for each row of the image, value indicates the first element of that row 
    std::vector<int> rowIdxs;
  };

  enum eSamplingPattern
  {
    SPARSE_8,//8 or 16 sampling points
    SPARSE_16,
    DENSE_3
  };

  enum eCorrelationWindow
  {
    DENSE_5,
    DENSE_7,
    DENSE_9,
    DENSE_11,
    DENSE_13
  };

  enum eMatchMode
  {
    FLOW,
    STEREO
  };

  struct MatchingParams
  {
    //Flow or Stereo
    eMatchMode mode;
    //Sampling pattern of the correlation window
    eCorrelationWindow corrType;
    //Dimension of the square correlation window
    int windowSize;
    //What is the maximum normalized Hamming Distance to accept?
    int filterDist;
    //Disparity constraint in pixels
    int maxDisparity;
    //Epsilon for epipolar constraint, in pixels
    int epipolarRange;
  };

  struct CensusCfg
  {
    //Use what sampling pattern
    eSamplingPattern type;
    //Dimension of the square sampling pattern.  e.g. the SPARSE_16 pattern has an equivalent window of a 9x9
    int patternSize;
    //img dims
    int imgRows, imgCols;
    //The LUT of the sampling pattern
    std::vector<int> pattern;

    MatchingParams params;
  };

  struct KpRow
  {
    std::vector<Feature>::iterator begin;
    std::vector<Feature>::iterator end;
  };
};//namespace correspondence
#endif
