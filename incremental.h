#ifndef POINTSETPOLYGONIZATION_INCREMENTAL_H
#define POINTSETPOLYGONIZATION_INCREMENTAL_H

#include "includes.h"
#include "cgalStructsFunctions.h"

vector<Point_2> IncrementalAlg(vector<Point_2>, int, string);
vector<Segment_2> getRedEdges(vector<Point_2>, vector<Point_2>, Point_2);
Segment_2 randomEdgeSelection(vector<Segment_2>);
Segment_2 minAreaEdgeSelection(vector<Segment_2>, Point_2, vector<Point_2>);
Segment_2 maxAreaEdgeSelection(vector<Segment_2>, Point_2, vector<Point_2>);
Segment_2 getPolygonEdgeToReplace(vector<Segment_2>, Point_2, vector<Point_2>, int);
vector<Segment_2> calculateVisibleEdges(vector<Segment_2>, vector<Point_2>, Point_2, vector<Point_2>);

#endif
