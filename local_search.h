#ifndef POINTSETPOLYGONIZATION_LOCAL_SEARCH_H
#define POINTSETPOLYGONIZATION_LOCAL_SEARCH_H

#include "includes.h"
#include "cgalStructsFunctions.h"
#include "prints.h"

vector<Point_2> LocalSearchAlg(vector<Point_2>, int, double, bool);
vector<vector<Point_2>> getPossiblePolygonChangesForSinglePoint(vector<Point_2>);
vector<vector<Point_2>> getPossiblePolygonChangesForMultiplePoints(vector<Point_2>, int, int);
void addChangedPointSetsInPossibleChangesVector(vector<vector<Point_2>>&, vector<vector<Point_2>>);
vector<Point_2> getChangeWithMaxArea(vector<vector<Point_2>>);
vector<Point_2> getChangeWithMinArea(vector<vector<Point_2>>);