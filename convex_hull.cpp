#include "convex_hull.h"

vector<Point_2> ConvexHullAlg(vector<Point_2> pointSet, int edgeSelectionMethod, string initMethod) {
    vector<Point_2> sortedPointSet, polygon;
    sortedPointSet = sortPointset(pointSet,"1b");
    polygon = createConvexHull(pointSet);
    printPointSet(polygon);
    removeUsedPoints(polygon,pointSet);
    addUntrackedCollinearPointsInPolygonalLine(polygon,pointSet);
    printPointSet(polygon);
    removeUsedPoints(polygon,pointSet);
    return polygon;
}

void removeUsedPoints(vector<Point_2> polygon, vector<Point_2>& pointSet) {
    int index = 0;
    for(int polygonIndex=0 ; polygonIndex < polygon.size() ; polygonIndex++) {
        for(int pointSetIndex=0 ; pointSetIndex < pointSet.size() ; pointSetIndex++) {
            if(polygon[polygonIndex]==pointSet[pointSetIndex]) {
                pointSet.erase(pointSet.begin() + pointSetIndex);
                break;
            }
        }
    }
}

void addUntrackedCollinearPointsInPolygonalLine(vector<Point_2>& polygon, vector<Point_2> pointSet) {
    for(int pointSetIndex=0 ; pointSetIndex<pointSet.size() ; ++pointSetIndex) {
        for(int polygonIndex=0 ; polygonIndex<polygon.size() ; ++polygonIndex) {
            int nextElementIndex = polygonIndex==(polygon.size()-1) ? 0 : (polygonIndex+1);
            //cout << "Testing: " << polygon[polygonIndex] << " , " << polygon[nextElementIndex] << " , " << pointSet[pointSetIndex] << endl;
            const auto doIntersect = CGAL::intersection(Segment_2(polygon[polygonIndex],polygon[nextElementIndex]),pointSet[pointSetIndex]);
            if (doIntersect) {
                polygon = insertPointToPolygonPointSet(pointSet[pointSetIndex],
                                                       Segment_2(polygon[polygonIndex], polygon[nextElementIndex]),
                                                       polygon);
                break;
            }

        }
    }
}