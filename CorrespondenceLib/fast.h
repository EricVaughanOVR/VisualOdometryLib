#ifndef FAST_H
#define FAST_H

#include "CorrespondenceDefs.hpp"

int fast9_corner_score(const byte* p, const int pixel[], int bstart);
int fast10_corner_score(const byte* p, const int pixel[], int bstart);
int fast11_corner_score(const byte* p, const int pixel[], int bstart);
int fast12_corner_score(const byte* p, const int pixel[], int bstart);

Feature* fast9_detect(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
Feature* fast10_detect(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
Feature* fast11_detect(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
Feature* fast12_detect(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);

int* fast9_score(const byte* i, int stride, Feature* corners, int num_corners, int b);
int* fast10_score(const byte* i, int stride, Feature* corners, int num_corners, int b);
int* fast11_score(const byte* i, int stride, Feature* corners, int num_corners, int b);
int* fast12_score(const byte* i, int stride, Feature* corners, int num_corners, int b);


Feature* fast9_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
Feature* fast10_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
Feature* fast11_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
Feature* fast12_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);

void fast9_detect_both(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax, 
  Feature** ret_nonmax_corners, int* ret_num_corners,  Feature** ret_corners);
void fast10_detect_both(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax,
  Feature** ret_nonmax_corners, int* ret_num_corners,  Feature** ret_corners);
void fast11_detect_both(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax, 
  Feature** ret_nonmax_corners, int* ret_num_corners,  Feature** ret_corners);
void fast12_detect_both(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax, 
  Feature** ret_nonmax_corners, int* ret_num_corners,  Feature** ret_corners);

Feature* nonmax_suppression(const Feature* corners, const int* scores, int num_corners, int* ret_num_nonmax);


#endif
