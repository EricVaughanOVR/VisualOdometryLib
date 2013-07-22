#include <stdlib.h>
#include "fast.hpp"

using namespace correspondence;

Feature* fast9_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners)
{
	Feature* corners;
	int num_corners;
	int* scores;
	Feature* nonmax;

	corners = fast9_detect(im, xsize, ysize, stride, b, &num_corners);
	scores = fast9_score(im, stride, corners, num_corners, b);
	nonmax = nonmax_suppression(corners, scores, num_corners, ret_num_corners);

	free(corners);
	free(scores);

	return nonmax;
}

Feature* fast10_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners)
{
	Feature* corners;
	int num_corners;
	int* scores;
	Feature* nonmax;

	corners = fast10_detect(im, xsize, ysize, stride, b, &num_corners);
	scores = fast10_score(im, stride, corners, num_corners, b);
	nonmax = nonmax_suppression(corners, scores, num_corners, ret_num_corners);

	free(corners);
	free(scores);

	return nonmax;
}

Feature* fast11_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners)
{
	Feature* corners;
	int num_corners;
	int* scores;
	Feature* nonmax;

	corners = fast11_detect(im, xsize, ysize, stride, b, &num_corners);
	scores = fast11_score(im, stride, corners, num_corners, b);
	nonmax = nonmax_suppression(corners, scores, num_corners, ret_num_corners);

	free(corners);
	free(scores);

	return nonmax;
}

Feature* fast12_detect_nonmax(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_corners)
{
	Feature* corners;
	int num_corners;
	int* scores;
	Feature* nonmax;

	corners = fast12_detect(im, xsize, ysize, stride, b, &num_corners);
	scores = fast12_score(im, stride, corners, num_corners, b);
	nonmax = nonmax_suppression(corners, scores, num_corners, ret_num_corners);

	free(corners);
	free(scores);

	return nonmax;
}

void fast9_detect_both(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax, 
  Feature** ret_nonmax_corners, int* ret_num_corners,  Feature** ret_corners)
{
	int* scores;

	*ret_corners = fast9_detect(im, xsize, ysize, stride, b, ret_num_corners);
	scores = fast9_score(im, stride, *ret_corners, *ret_num_corners, b);
	*ret_nonmax_corners = nonmax_suppression(*ret_corners, scores, *ret_num_corners, ret_num_nonmax);

	free(scores);
}

void fast10_detect_both(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax, 
  Feature** ret_nonmax_corners, int* ret_num_corners,  Feature** ret_corners)
{
	int* scores;

	*ret_corners = fast10_detect(im, xsize, ysize, stride, b, ret_num_corners);
	scores = fast10_score(im, stride, *ret_corners, *ret_num_corners, b);
	*ret_nonmax_corners = nonmax_suppression(*ret_corners, scores, *ret_num_corners, ret_num_nonmax);

	free(scores);
}

void fast11_detect_both(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax, 
  Feature** ret_nonmax_corners, int* ret_num_corners,  Feature** ret_corners)
{
	int* scores;

	*ret_corners = fast11_detect(im, xsize, ysize, stride, b, ret_num_corners);
	scores = fast11_score(im, stride, *ret_corners, *ret_num_corners, b);
	*ret_nonmax_corners = nonmax_suppression(*ret_corners, scores, *ret_num_corners, ret_num_nonmax);

	free(scores);
}

void fast12_detect_both(const byte* im, int xsize, int ysize, int stride, int b, int* ret_num_nonmax, 
  Feature** ret_nonmax_corners, int* ret_num_corners,  Feature** ret_corners)
{
	int* scores;

	*ret_corners = fast12_detect(im, xsize, ysize, stride, b, ret_num_corners);
	scores = fast12_score(im, stride, *ret_corners, *ret_num_corners, b);
	*ret_nonmax_corners = nonmax_suppression(*ret_corners, scores, *ret_num_corners, ret_num_nonmax);

	free(scores);
}