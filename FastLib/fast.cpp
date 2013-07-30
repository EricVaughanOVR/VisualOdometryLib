#include <stdlib.h>
#include "fast.hpp"

using namespace correspondence;

void fast9_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, FeatureList& corners)
{
	fast9_detect(im, xsize, ysize, stride, b, corners);
	fast9_score(im, stride, corners, b);
	nonmax_suppression(corners);
}

void fast10_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, FeatureList& corners)
{
	fast10_detect(im, xsize, ysize, stride, b, corners);
	fast10_score(im, stride, corners, b);
	nonmax_suppression(corners);
}

void fast11_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, FeatureList& corners)
{
	fast11_detect(im, xsize, ysize, stride, b, corners);
	fast11_score(im, stride, corners, b);
	nonmax_suppression(corners);
}

void fast12_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, FeatureList& corners)
{
	fast12_detect(im, xsize, ysize, stride, b, corners);
	fast12_score(im, stride, corners, b);
	nonmax_suppression(corners);
}

void fast9_detect_both(const byte* im, int xsize, int ysize, int stride, int b, FeatureList& corners)
{
  fast9_detect(im, xsize, ysize, stride, b, corners);
  fast9_score(im, stride, corners, b);
	nonmax_suppression(corners);
}

void fast10_detect_both(const byte* im, int xsize, int ysize, int stride, int b, FeatureList& corners)
{
  fast10_detect(im, xsize, ysize, stride, b, corners);
  fast10_score(im, stride, corners, b);
	nonmax_suppression(corners);
}

void fast11_detect_both(const byte* im, int xsize, int ysize, int stride, int b, FeatureList& corners)
{
  fast11_detect(im, xsize, ysize, stride, b, corners);
  fast11_score(im, stride, corners, b);
	nonmax_suppression(corners);
}

void fast12_detect_both(const byte* im, int xsize, int ysize, int stride, int b, FeatureList& corners)
{
  fast12_detect(im, xsize, ysize, stride, b, corners);
  fast12_score(im, stride, corners, b);
	nonmax_suppression(corners);
}
