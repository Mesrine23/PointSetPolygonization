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
//    int index = 0;
//    for(int polygonIndex=0 ; polygonIndex < polygon.size() ; polygonIndex++) {
//        for(int pointSetIndex=0 ; pointSetIndex < pointSet.size() ; pointSetIndex++) {
//            if(polygon[polygonIndex]==pointSet[pointSetIndex]) {
//                pointSet.erase(pointSet.begin() + pointSetIndex);
//                break;
//            }
//        }
//    }
    for(Point_2 & polygonPoint : polygon){
        if(int index = find(pointSet.begin(), pointSet.end(), polygonPoint)!=pointSet.end())
            pointSet.erase(next(pointSet.begin(),index));
    }

}

void addUntrackedCollinearPointsInPolygonalLine(vector<Point_2>& polygon, vector<Point_2>& pointSet) {
    int size = pointSet.size()-1;
    for(int pointSetIndex=size ; pointSetIndex>=0 ; --pointSetIndex) {
        for(int polygonIndex=0 ; polygonIndex<polygon.size() ; ++polygonIndex) {
            int nextElementIndex = polygonIndex==(polygon.size()-1) ? 0 : (polygonIndex+1);
            //cout << "Testing: " << polygon[polygonIndex] << " , " << polygon[nextElementIndex] << " , " << pointSet[pointSetIndex] << endl;
            const auto doIntersect = CGAL::intersection(Segment_2(polygon[polygonIndex],polygon[nextElementIndex]),pointSet[pointSetIndex]);
            if (doIntersect) {
                polygon = insertPointToPolygonPointSet(pointSet[pointSetIndex],
                                                       Segment_2(polygon[polygonIndex], polygon[nextElementIndex]),
                                                       polygon);
                pointSet.erase(next(pointSet.begin(),pointSetIndex));
                break;
            }

        }
    }
}

Point_2 findClosestVisiblePointToSegment(Segment_2 seg, vector<Point_2> points, vector<Point_2> polygon){
    pair<Point_2, int> minDistance(points[0], -1);
    vector<Segment_2> polygonEdges = getPolygonEdgesFromPoints(polygon);
    for(Point_2 &point : points){
        if(isEdgeVisibleFromPoint(point, seg, polygonEdges)){
            if(int dist = abs(squared_distance(point, seg))<minDistance.second){
                minDistance.first = point;
                minDistance.second = dist;
            }
        }
    }
    if(minDistance.second == -1){
        cout<<"Visible points from Segment!"<<endl;
        exit(1);
    }
    return minDistance.first;
}