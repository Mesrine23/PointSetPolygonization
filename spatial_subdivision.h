#ifndef PROJECT_SPATIAL_SUBDIVISION_H
#define PROJECT_SPATIAL_SUBDIVISION_H

#include "includes.h"
#include "convex_hull.h"
#include "incremental.h"
#include "cgalStructsFunctions.h"
#include "prints.h"
#include "simulated_annealing.h"

struct Spal{
    vector<Point_2> pointset;
    Point_2 leftmost;
    Point_2 rightmost;
    Segment_2 leftmostSeg;
    Segment_2 rightmostSeg;
    bool isFirst;
    bool isLast;
};

vector<Point_2> spatialSubdivision(vector<Point_2>, int , int , int ,int);
vector<Spal> getSpalsFromPointSet(vector<Point_2>, int);
bool rightCriterion(vector<Point_2> );
bool leftCriterion(vector<Point_2> , double , int );
void createOptimalSpals(vector<Spal>, int, int, int);
vector<pair<Segment_2,Point_2>> SubdivisionGetPairOfClosestPointToSegments(vector<Point_2>, vector<Point_2>, vector<Segment_2>);
vector<Point_2> subdivisionConvexHullAlgo(Spal , vector<Point_2> , int );
vector<Point_2> subdivisionSimulatedAnnealing(Spal ,vector<Point_2> , int , int , int );
bool markedSegmentChosen(Spal , StepResult );
void addPointsLeftmostTillRightmost(vector<Point_2>&, Spal );
void addPointsRightmostTillLeftMost(vector<Point_2>& , Spal );
vector<Point_2> mergeSpals(vector<Spal> );
#endif