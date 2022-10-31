#ifndef POINTSETPOLYGONIZATION_CONVEX_HULL_H
#define POINTSETPOLYGONIZATION_CONVEX_HULL_H

#include "includes.h"
#include "cgalStructsFunctions.h"
#include "prints.h"

vector<Point_2> ConvexHullAlg(vector<Point_2>, int, string);
void removeUsedPoints(vector<Point_2>, vector<Point_2>&);
void addUntrackedCollinearPointsInPolygonalLine(vector<Point_2>&, vector<Point_2>&);
Point_2 findClosestVisiblePointToSegment(Segment_2 , vector<Point_2>, vector<Point_2> );
#endif