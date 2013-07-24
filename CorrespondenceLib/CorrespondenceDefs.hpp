#ifndef CORRESPONDENCEDEFS_HPP
#define CORRESPONDENCEDEGS_HPP

#include <stdint.h>
#include <vector>
#include <memory>

namespace correspondence
{
  typedef uint8_t byte;

  struct Image
  {
    int rows, cols, stride, pxStep;
    std::shared_ptr<byte> data;

    Image(const int _rows, const int _cols, const int _stride, const int _pxStep)
      : rows(_rows),
        cols(_cols),
        stride(_stride),
        pxStep(_pxStep),
        data((byte*)_mm_malloc(stride * rows * pxStep, 16))
    {
    }

    inline byte* at(const int _row, const int _col) const
    {
      //Note: only handles a positive value for stride
      return data.get() + _row * stride + _col;
    };
  };

  struct Feature
  {
    int x, y, idx;
  };

  struct Match
  {
    Feature feature1, feature2;
    int dist;
  };

  struct FeatureList
  {
    std::vector<Feature> allFeatures;
    std::vector<Feature> nonmaxFeatures;

    //Element for each row of the image, value indicates the first element of that row 
    std::vector<int> rowIdxs;
  };

  enum eSamplingWindow
  {
    SPARSE_8,//8, 12 or 16 sampling points
    SPARSE_12,
    SPARSE_16,
    DENSE_3,//various sizes of dense sampling windows
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
    int filterDist;
    int maxDisparity;
    int epipolarRange;
  };

  struct CensusCfg
  {
    //Use what sampling pattern
    eSamplingWindow type;
    //Dimension of the square sampling pattern.  e.g. the SPARSE_16 pattern has an equivalent window of a 9x9
    int windowSize;
    //The LUT of the sampling pattern
    std::vector<int> pattern;
    //Dense or Sparse
    eMatchMode matchMode;
    //sampling pattern of the correlation window
    eSamplingWindow corrType;
    //The lookup table of the correlation window
    std::vector<int> corrPattern;
  };

  struct Descriptors
  {
    Image tfmdIm;
    eSamplingWindow type;
  };
};//namespace correspondence
#endif
