#ifndef POINTSETPOLYGONIZATION_CGALSTRUCTSFUNCTIONS_H
#define POINTSETPOLYGONIZATION_CGALSTRUCTSFUNCTIONS_H

#include "includes.h"

vector<Segment_2> getPolygonEdgesFromPoints(vector<Point_2>);
Polygon_2 getSimplePolygonFromPoints(vector<Point_2>);
bool isPolygonSimple(vector<Point_2>);
bool comparePointXdesc(Point_2, Point_2);
bool comparePointXasc(Point_2, Point_2);
bool comparePointYdesc(Point_2, Point_2);
bool comparePointYasc(Point_2, Point_2);
int findIndexOfPointInPointSet(vector<Point_2>, Point_2);
vector<Point_2> sortPointset(vector<Point_2>, string);
vector<Point_2> createConvexHull(vector<Point_2>);
vector<Segment_2> findChainOfEdges(Point_2, Point_2, vector<Point_2>, bool);
bool isEdgeVisibleFromPoint(Point_2, Segment_2, vector<Segment_2>);
bool segmentsEquivalent(Segment_2, Segment_2);
vector<Point_2> insertPointToPolygonPointSet(Point_2, Segment_2, vector<Point_2>);
bool sameOrientation(vector<Point_2> , vector<Point_2> );
void printErrorPointNotFound(Point_2, vector<Point_2>);
vector<int> createVectorOfRandomInts(int range);
bool segmentIntersectsPolygonEdge(Segment_2 , Segment_2);
bool segmentIntersectsSegment(Segment_2 , Segment_2);
bool segmentIntersectsSegmentXXX(Segment_2, Segment_2);
bool edgesAreAdjacent(Segment_2, Segment_2);

#endif
