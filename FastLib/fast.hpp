#ifndef FAST_HPP
#define FAST_HPP

#include "CorrespondenceDefs.hpp"

int fast9_corner_score(const correspondence::byte* p, const int pixel[], int bstart);
int fast10_corner_score(const correspondence::byte* p, const int pixel[], int bstart);
int fast11_corner_score(const correspondence::byte* p, const int pixel[], int bstart);
int fast12_corner_score(const correspondence::byte* p, const int pixel[], int bstart);

void fast9_detect(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);
void fast10_detect(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);
void fast11_detect(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);
void fast12_detect(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);

void fast9_score(const correspondence::byte* i, int stride, correspondence::FeatureList& corners, int b);
void fast10_score(const correspondence::byte* i, int stride, correspondence::FeatureList& corners, int b);
void fast11_score(const correspondence::byte* i, int stride, correspondence::FeatureList& corners, int b);
void fast12_score(const correspondence::byte* i, int stride, correspondence::FeatureList& corners, int b);


void fast9_detect_nonmax(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);
void fast10_detect_nonmax(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);
void fast11_detect_nonmax(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);
void fast12_detect_nonmax(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);

void fast9_detect_both(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);
void fast10_detect_both(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);
void fast11_detect_both(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);
void fast12_detect_both(const correspondence::byte* im, int xsize, int ysize, int stride, int b, correspondence::FeatureList& corners);

void nonmax_suppression(correspondence::FeatureList& corners);

#endif
