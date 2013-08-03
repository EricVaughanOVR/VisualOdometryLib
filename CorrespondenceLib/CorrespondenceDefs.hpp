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
    pt()
      : x(0),
        y(0)
    {
    }
  };

  class Image
  {
  public:
    Image(const int _rows, const int _cols, const int _pxStep, const pt _offset);

    //Copies the supplied data into this object's aligned memory
    Image(const int _rows, const int _cols, const int _pxStep, const pt _offset, 
      const byte* _data);

    ~Image();

    inline void zeroMem();

    inline byte* at(const int _row, const int _col) const
    {
      //Note: only handles a positive value for stride
      return data + _row * stride + _col * pxStep;
    };

    int rows, cols, pxStep, stride;
    byte* data;
    pt offset;
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
    SPARSE_16
  };

  enum eCorrelationWindow
  {
    //Pattern shapes are tweaked for optimization
    SPARSECW_8,
    SPARSECW_16,
    DENSECW_5,//4 rows * 5 cols
    DENSECW_7,//7 rows * 8 cols
    DENSECW_9,//8 rows * 9 cols
    DENSECW_11,//12 rows * 11 cols
    DENSECW_13//12 rows * 13 cols
  };

  enum eMatchMode
  {
    FLOW,
    STEREO
  };

  struct MatchingParams
  {
    MatchingParams();

    MatchingParams(const eMatchMode _mode, const eCorrelationWindow _corrType, const int _filterDist, 
                   const int _maxDisparity, const int _epipolarRange, const int _stride);

    void prepCorrLUT(const int _stride);

    //Flow or Stereo
    eMatchMode mode;
    //Sampling type of the correlation window
    eCorrelationWindow corrType;
    //Dimension of the square correlation window
    int windowSize;
    //Size of implicit img border where SHD cannot be found
    int edgeSize;
    //Sampling pattern of the correlation window
    std::vector<int> pattern;
    //What is the maximum normalized Hamming Distance to accept?
    int filterDist;
    //Disparity constraint in pixels
    int maxDisparity;
    //Epsilon for epipolar constraint, in pixels
    int epipolarRange;
  };

  class CensusCfg
  {
  public:
    CensusCfg();

    CensusCfg(const eSamplingPattern _type, const int _rows, const int _cols, const int _stride);

    void prepSamplingLUT();

    //Use what sampling pattern
    eSamplingPattern type;
    //Dimension of the square sampling pattern.  e.g. the SPARSE_16 pattern has an equivalent window of a 9x9
    int patternSize;
    //Size of implicit img border where descriptors cannot be found
    int edgeSize;
    //img dims
    int imgRows, imgCols, imgStride;
    //The LUT of the sampling pattern
    std::vector<int> pattern;
  };

  struct KpRow
  {
    std::vector<Feature>::iterator begin;
    std::vector<Feature>::iterator end;
  };
};//namespace correspondence
#endif
