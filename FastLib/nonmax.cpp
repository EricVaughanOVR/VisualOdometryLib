#include <stdlib.h>
#include "fast.hpp"

using namespace correspondence;
#define Compare(X, Y) ((X)>=(Y))

void nonmax_suppression(FeatureList& corners)
{
	int last_row;
	int* row_start;
	int i, j;
	const int sz = (int)corners.allFeatures.size();

	/*Point above points (roughly) to the pixel above the one of interest, if there
    is a feature there.*/
	int point_above = 0;
	int point_below = 0;

	
	if(sz < 1)
	{
		corners.nonmaxFeatures.clear();
		return;
	}

  corners.nonmaxFeatures.reserve(corners.allFeatures.size());

	/* Find where each row begins
	   (the corners are output in raster scan order). A beginning of -1 signifies
	   that there are no corners on that row. */
	last_row = corners.allFeatures[sz-1].y;
	row_start = (int*)malloc((last_row+1)*sizeof(int));

	for(i=0; i < last_row+1; i++)
		row_start[i] = -1;
	
	{
		int prev_row = -1;
		for(i=0; i< sz; i++)
			if(corners.allFeatures[i].y != prev_row)
			{
				row_start[corners.allFeatures[i].y] = i;
				prev_row = corners.allFeatures[i].y;
			}
	}
	
	
	
	for(i=0; i < sz; i++)
	{
		int score = corners.allFeatures[i].score;
		Feature pos = corners.allFeatures[i];
			
		/*Check left */
		if(i > 0)
			if(corners.allFeatures[i-1].x == pos.x-1 && corners.allFeatures[i-1].y == pos.y && Compare(corners.allFeatures[i-1].score, score))
				continue;
			
		/*Check right*/
		if(i < (sz - 1))
			if(corners.allFeatures[i+1].x == pos.x+1 && corners.allFeatures[i+1].y == pos.y && Compare(corners.allFeatures[i+1].score, score))
				continue;
			
		/*Check above (if there is a valid row above)*/
		if(pos.y != 0 && row_start[pos.y - 1] != -1) 
		{
			/*Make sure that current point_above is one
			  row above.*/
			if(corners.allFeatures[point_above].y < pos.y - 1)
				point_above = row_start[pos.y-1];
			
			/*Make point_above point to the first of the pixels above the current point,
			  if it exists.*/
			for(; corners.allFeatures[point_above].y < pos.y && corners.allFeatures[point_above].x < pos.x - 1; point_above++)
			{}
			
			
			for(j=point_above; corners.allFeatures[j].y < pos.y && corners.allFeatures[j].x <= pos.x + 1; j++)
			{
				int x = corners.allFeatures[j].x;
				if( (x == pos.x - 1 || x ==pos.x || x == pos.x+1) && Compare(corners.allFeatures[j].score, score))
					goto cont;
			}
			
		}
			
		/*Check below (if there is anything below)*/
		if(pos.y != last_row && row_start[pos.y + 1] != -1 && point_below < sz) /*Nothing below*/
		{
			if(corners.allFeatures[point_below].y < pos.y + 1)
				point_below = row_start[pos.y+1];
			
			/* Make point below point to one of the pixels belowthe current point, if it
			   exists.*/
			for(; point_below < sz && corners.allFeatures[point_below].y == pos.y+1 && corners.allFeatures[point_below].x < pos.x - 1; point_below++)
			{}

			for(j=point_below; j < sz && corners.allFeatures[j].y == pos.y+1 && corners.allFeatures[j].x <= pos.x + 1; j++)
			{
				int x = corners.allFeatures[j].x;
				if( (x == pos.x - 1 || x ==pos.x || x == pos.x+1) && Compare(corners.allFeatures[j].score, score))
					goto cont;
			}
		}
		
		corners.nonmaxFeatures.push_back(corners.allFeatures[i]);
		cont:
			;
	}
}

