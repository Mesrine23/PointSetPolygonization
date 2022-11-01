#include "convex_hull.h"

vector<Point_2> ConvexHullAlg(vector<Point_2> pointSet, int edgeSelectionMethod) {
    vector<Point_2> polygon;
    pointSet = sortPointset(pointSet,"1b");
    polygon = createConvexHull(pointSet);
    printPointSet(polygon);
    removeUsedPoints(polygon,pointSet);
    addUntrackedCollinearPointsInPolygonalLine(polygon,pointSet);
    printPointSet(polygon);
    removeUsedPoints(polygon,pointSet);
    while(!pointSet.empty()) {
        vector<pair<Segment_2, Point_2>> pairOfSegmentAndClosestPoint = getPairOfClosestPointToSegments(pointSet,polygon);
        pair<Segment_2, Point_2> pairToReplace;
        switch (edgeSelectionMethod) {
            case 1:
                pairToReplace = randomPairSelection(pairOfSegmentAndClosestPoint, polygon);
                break;
            case 2:
                pairToReplace = minAreaPairSelection(pairOfSegmentAndClosestPoint, polygon);
                break;
            case 3:
                pairToReplace = maxAreaPairSelection(pairOfSegmentAndClosestPoint, polygon);
                break;
            default:
                cout << "Wrong edge selection method" << endl;
                exit(-1);
        }
        polygon = insertPointToPolygonPointSet(pairToReplace.second, pairToReplace.first, polygon);
    }
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

vector<pair<Segment_2,Point_2>> getPairOfClosestPointToSegments(vector<Point_2> internalPoints, vector<Point_2> polygon) {
    vector<pair<Segment_2,Point_2>> pairs;
    vector<Segment_2> polygonEdges = getPolygonEdgesFromPoints(polygon);
    for(Segment_2 edge : polygonEdges) {
        Point_2 closestPoint = findClosestPointToSegment(edge,internalPoints);
        pairs.push_back(make_pair(edge,closestPoint));
    }
    return pairs;
};

Point_2 findClosestPointToSegment(Segment_2 seg, vector<Point_2> internalPoints){
    pair<Point_2, int> minDistance(points[0], -1);
    //vector<Segment_2> polygonEdges = getPolygonEdgesFromPoints(polygon);
    for(Point_2 &point : internalPoints){
        //if(isEdgeVisibleFromPoint(point, seg, polygonEdges)){
        if(int dist = abs(squared_distance(point, seg))<minDistance.second){
            minDistance.first = point;
            minDistance.second = dist;
        }
        //}
    }
    /*if(minDistance.second == -1){
        cout<<"Visible points from Segment!"<<endl;
        exit(1);
    }*/
    return minDistance.first;
}

pair<Segment_2,Point_2> randomPairSelection(vector<pair<Segment_2,Point_2>> pair, vector<Point_2> polygon) {
    vector<int> randomInts = createVectorOfRandomInts();
    vector<Segment_2> polygonEdges = getPolygonEdgesFromPoints(polygon);
    for(int i=0 ; i<pair.size() ; ++i) {
        if( isEdgeVisibleFromPoint( pair[randomInts[i]].second, pair[randomInts[i]].first, polygonEdges) )
            return pair[randomInts[i]];
    }
    cout << "Couldn't find a visible point!" << endl << "Aborting..." << endl;
    exit(-1);
}

pair<Segment_2,Point_2> minAreaEdgeSelection() {
    cout << "Couldn't find a visible point!" << endl << "Aborting..." << endl;
    exit(-1);
}

pair<Segment_2,Point_2> maxAreaEdgeSelection() {
    cout << "Couldn't find a visible point!" << endl << "Aborting..." << endl;
    exit(-1);
}