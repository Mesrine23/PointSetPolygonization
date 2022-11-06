#include "incremental.h"

vector<Point_2> IncrementalAlg(vector<Point_2> pointSet, int edgeSelectionMethod, string initMethod) {
    vector<Point_2> sortedPointSet, usedPoints, convexHull, polygon;


    sortedPointSet = sortPointset(pointSet, initMethod);


    for(int i=0; i<3; i++) {
        polygon.push_back(sortedPointSet.front());
        usedPoints.push_back(sortedPointSet.front());
        sortedPointSet.erase(sortedPointSet.begin());
    }

    int collinearIndex = 2;
    while(CGAL::collinear(polygon[collinearIndex-2],polygon[collinearIndex-1],polygon[collinearIndex])) {
        polygon.push_back(sortedPointSet.front());
        usedPoints.push_back(sortedPointSet.front());
        sortedPointSet.erase(sortedPointSet.begin());
        ++collinearIndex;
    }

    srand(time(NULL));
    while(!sortedPointSet.empty()){
        convexHull = createConvexHull(usedPoints);

        Point_2 currentPoint = sortedPointSet.front();
        sortedPointSet.erase(sortedPointSet.begin());
        usedPoints.push_back(currentPoint);

        vector<Point_2> newConvexHull = createConvexHull(usedPoints);
        vector<Segment_2> redEdges = getRedEdges(convexHull, newConvexHull,currentPoint);
        vector<Segment_2> replaceableEdges = calculateVisibleEdges(redEdges,polygon,currentPoint,convexHull);
        Segment_2 edgeToReplace = getPolygonEdgeToReplace(replaceableEdges,currentPoint,polygon,edgeSelectionMethod);
        polygon = insertPointToPolygonPointSet(currentPoint,edgeToReplace,polygon);
    }
    return polygon;
}

vector<Segment_2> getRedEdges(vector<Point_2> oldConvexHull, vector<Point_2> newConvexHull, Point_2 newPoint){
    vector<Segment_2> replacedEdges;
    int positionOfNewPoint = findIndexOfPointInPointSet(newConvexHull,newPoint);
    Point_2 upperPoint, lowerPoint;

    (positionOfNewPoint == (newConvexHull.size()-1)) ? (lowerPoint = newConvexHull[0]) : (lowerPoint = newConvexHull[positionOfNewPoint+1]);
    (positionOfNewPoint == 0) ? (upperPoint = newConvexHull[newConvexHull.size()-1]) : (upperPoint = newConvexHull[1]);

    vector<Segment_2> oldConvexHullPolygon = getPolygonEdgesFromPoints(oldConvexHull);
    vector<Segment_2> newConvexHullPolygon = getPolygonEdgesFromPoints(newConvexHull);
    for(Segment_2 oldConvexEdge: oldConvexHullPolygon) {
        bool foundEdge = false;
        for(Segment_2 newConvexEdge: newConvexHullPolygon) {
            if (segmentsEquivalent(oldConvexEdge,newConvexEdge)) {
                foundEdge = true;
                break;
            }
        }
        if(!foundEdge) {
            replacedEdges.push_back(oldConvexEdge);
        }
    }
    return replacedEdges;
}

Segment_2 randomEdgeSelection(vector<Segment_2> replaceableEdges) {
    if(replaceableEdges.size()==1)
        return replaceableEdges[0];
    int randomIndex = (std::rand() % (replaceableEdges.size()));
    return replaceableEdges[randomIndex];
}

Segment_2 minAreaEdgeSelection(vector<Segment_2> replaceableEdges, Point_2 currentPoint, vector<Point_2> polygonPointSet) {
    Segment_2 edgeToReturn = replaceableEdges[0];
    vector<Point_2> currentPolygonPointSet = insertPointToPolygonPointSet(currentPoint,edgeToReturn,polygonPointSet);
    long double minArea = abs(getSimplePolygonFromPoints(currentPolygonPointSet).area());
    for(int i=1 ; i<replaceableEdges.size() ; ++i) {
        Segment_2 tempEdgeToReturn = replaceableEdges[i];
        vector<Point_2> tempPolygonPointSet = insertPointToPolygonPointSet(currentPoint,tempEdgeToReturn,polygonPointSet);
        long double tempMinArea = abs(getSimplePolygonFromPoints(tempPolygonPointSet).area());
        if (tempMinArea < minArea) {
            edgeToReturn = replaceableEdges[i];
            minArea = tempMinArea;
        }
    }
    return edgeToReturn;
}

Segment_2 maxAreaEdgeSelection(vector<Segment_2> replaceableEdges, Point_2 currentPoint, vector<Point_2> polygonPointSet) {
    Segment_2 edgeToReturn = replaceableEdges[0];
    vector<Point_2> currentPolygonPointSet = insertPointToPolygonPointSet(currentPoint,edgeToReturn,polygonPointSet);
    long double maxArea = abs(getSimplePolygonFromPoints(currentPolygonPointSet).area());
    for(int i=1 ; i<replaceableEdges.size() ; ++i) {
        Segment_2 tempEdgeToReturn = replaceableEdges[i];
        vector<Point_2> tempPolygonPointSet = insertPointToPolygonPointSet(currentPoint,tempEdgeToReturn,polygonPointSet);
        long double tempMaxArea = abs(getSimplePolygonFromPoints(tempPolygonPointSet).area());
        if (tempMaxArea > maxArea) {
            edgeToReturn = replaceableEdges[i];
            maxArea = tempMaxArea;
        }
    }
    return edgeToReturn;
}

Segment_2 getPolygonEdgeToReplace(vector<Segment_2> replaceableEdges, Point_2 currentPoint, vector<Point_2> polygonPointSet, int edgeSelectionMethod) {
    switch (edgeSelectionMethod) {
        case 1:
            return randomEdgeSelection(replaceableEdges);
        case 2:
            return minAreaEdgeSelection(replaceableEdges,currentPoint,polygonPointSet);
        case 3:
            return maxAreaEdgeSelection(replaceableEdges,currentPoint,polygonPointSet);
        default:
            exit(-1);
    }
}

vector<Segment_2> calculateVisibleEdges(vector<Segment_2> redEdges, vector<Point_2> polygonPointSet, Point_2 added_point, vector<Point_2> convexHull){
    vector<Segment_2> visibleEdges;
    vector<Segment_2> polygonEdges = getPolygonEdgesFromPoints(polygonPointSet);
    for(Segment_2 redEdge : redEdges){
        bool foundEdge=false;
        for(Segment_2 polygonEdge : polygonEdges){
            if(CGAL::collinear(added_point, polygonEdge[0], polygonEdge[1]))continue;
            if(segmentsEquivalent(redEdge, polygonEdge)) {
                visibleEdges.push_back(redEdge);
                foundEdge = true;
                break;
            }
        }
        if(!foundEdge) {
            vector<Segment_2> chainedEdges = findChainOfEdges(redEdge.start(),redEdge.end(),polygonPointSet, sameOrientation(polygonPointSet, convexHull));
            for(Segment_2 chainedEdge: chainedEdges){
                if(isEdgeVisibleFromPoint(added_point,chainedEdge,polygonEdges)) {
                    visibleEdges.push_back(chainedEdge);
                }
            }
        }
    }
    return visibleEdges;
}