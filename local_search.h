#ifndef POINTSETPOLYGONIZATION_LOCAL_SEARCH_H
#define POINTSETPOLYGONIZATION_LOCAL_SEARCH_H

#include "includes.h"
#include "cgalStructsFunctions.h"
#include "prints.h"

vector<Point_2> LocalSearchAlg(vector<Point_2>, int, double, bool, long&);
vector<vector<Point_2>> getPossiblePolygonChangesForSinglePoint(vector<Point_2>, int);
vector<vector<Point_2>> getPossiblePolygonChangesForMultiplePoints(vector<Point_2>, int, int);
void addChangedPointSetsInPossibleChangesVector(vector<vector<Point_2>>&, vector<vector<Point_2>>);
vector<Point_2> getChangeWithMaxArea(vector<vector<Point_2>>);
vector<Point_2> getChangeWithMinArea(vector<vector<Point_2>>);
Point_2 getPreviousPointFromChainOfPoints(vector<Point_2>, int);
Point_2 getNextPointFromChainOfPoints(vector<Point_2>, int, int);
int maxChangesToFind(vector<Point_2>);
int getRandomPolygonIndex(int,int);

#endif