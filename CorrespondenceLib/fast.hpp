#ifndef FAST_H
#define FAST_H

#include "CorrespondenceDefs.hpp"

int fast9_corner_score(const correspondence::byte* p, const int pixel[], int bstart);
int fast10_corner_score(const correspondence::byte* p, const int pixel[], int bstart);
int fast11_corner_score(const correspondence::byte* p, const int pixel[], int bstart);
int fast12_corner_score(const correspondence::byte* p, const int pixel[], int bstart);

correspondence::Feature* fast9_detect(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
correspondence::Feature* fast10_detect(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
correspondence::Feature* fast11_detect(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
correspondence::Feature* fast12_detect(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);

int* fast9_score(const correspondence::byte* i, int stride, correspondence::Feature* corners, int num_corners, int b);
int* fast10_score(const correspondence::byte* i, int stride, correspondence::Feature* corners, int num_corners, int b);
int* fast11_score(const correspondence::byte* i, int stride, correspondence::Feature* corners, int num_corners, int b);
int* fast12_score(const correspondence::byte* i, int stride, correspondence::Feature* corners, int num_corners, int b);


correspondence::Feature* fast9_detect_nonmax(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
correspondence::Feature* fast10_detect_nonmax(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
correspondence::Feature* fast11_detect_nonmax(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);
correspondence::Feature* fast12_detect_nonmax(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners);

void fast9_detect_both(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax, 
  correspondence::Feature** ret_nonmax_corners, int* ret_num_corners,  correspondence::Feature** ret_corners);
void fast10_detect_both(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax,
  correspondence::Feature** ret_nonmax_corners, int* ret_num_corners,  correspondence::Feature** ret_corners);
void fast11_detect_both(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax, 
  correspondence::Feature** ret_nonmax_corners, int* ret_num_corners,  correspondence::Feature** ret_corners);
void fast12_detect_both(const correspondence::byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax, 
  correspondence::Feature** ret_nonmax_corners, int* ret_num_corners,  correspondence::Feature** ret_corners);

correspondence::Feature* nonmax_suppression(const correspondence::Feature* corners, const int* scores, int num_corners, int* ret_num_nonmax);


#endif
