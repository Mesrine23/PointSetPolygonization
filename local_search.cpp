#include "local_search.h"

vector<Point_2> LocalSearchAlg(vector<Point_2> polygon, int chainLength, double threshold, bool isMax) {
    long double polygonArea = abs(getSimplePolygonFromPoints(polygon).area());
    long double newPolygonArea = 0;

    while(abs(polygonArea-newPolygonArea) >= threshold) {
        vector<vector<Point_2>> allPossiblePolygonChanges;
        for(int pointIndex=0 ; pointIndex < polygon.size() ; ++pointIndex) {
            vector<vector<Point_2>> possiblePolygonChangesForSinglePoint = getPossiblePolygonChangesForSinglePoint(polygon,pointIndex);
            addChangedPointSetsInPossibleChangesVector(allPossiblePolygonChanges,possiblePolygonChangesForSinglePoint);
        }
        vector<Point_2> newPolygon;
        if (isMax) {
            newPolygon = getChangeWithMaxArea(allPossiblePolygonChanges);
            if(newPolygon.empty())
                break;
            newPolygonArea = abs(getSimplePolygonFromPoints(newPolygon).area());
            polygonArea = abs(getSimplePolygonFromPoints(polygon).area());
            if(newPolygonArea<=polygonArea)
                break;
            polygon = newPolygon;
        } else {
            newPolygon = getChangeWithMinArea(allPossiblePolygonChanges);
            if(newPolygon.empty())
                break;
            newPolygonArea = abs(getSimplePolygonFromPoints(newPolygon).area());
            polygonArea = abs(getSimplePolygonFromPoints(polygon).area());
            if(newPolygonArea>=polygonArea)
                break;
            polygon = newPolygon;
        }
    }
}

vector<vector<Point_2>> getPossiblePolygonChangesForSinglePoint(vector<Point_2> polygon, int indexOfPoint) {
    vector<vector<Point_2>> possiblePolygonChanges;
    Point_2 nextPoint, previousPoint;
    Point_2 currentPoint = polygon[indexOfPoint];
    if (indexOfPoint==0) {
        nextPoint = polygon[indexOfPoint+1];
        previousPoint = polygon[polygon.size()-1];
    }
    else if (indexOfPoint==polygon.size()-1) {
        nextPoint = polygon[0];
        previousPoint = polygon[indexOfPoint-1];
    }
    else {
        nextPoint = polygon[indexOfPoint+1];
        previousPoint = polygon[indexOfPoint-1];
    }
    polygon.erase(polygon.begin() + indexOfPoint);
    for(int i=0 ; i<polygon.size()-1 ; ++i) {
        if(polygon[i]==nextPoint || polygon[i]==previousPoint || polygon[i+1]==nextPoint || polygon[i+1]==previousPoint)
            continue;
        vector<Point_2> tempPolygon = polygon;
        tempPolygon.insert( (tempPolygon.begin()+i+1) , currentPoint );
        if(isPolygonSimple(tempPolygon))
            possiblePolygonChanges.push_back(tempPolygon);
    }
    if(!(polygon[polygon.size()-1]==nextPoint || polygon[polygon.size()-1]==previousPoint || polygon[0]==nextPoint || polygon[0]==previousPoint)) {
        vector<Point_2> tempPolygon = polygon;
        tempPolygon.push_back(currentPoint);
        if(isPolygonSimple(tempPolygon))
            possiblePolygonChanges.push_back(tempPolygon);
    }
    return possiblePolygonChanges;
}

vector<vector<Point_2>> getPossiblePolygonChangesForMultiplePoints(vector<Point_2> polygon, int indexOfPoint, int length) {

}

void addChangedPointSetsInPossibleChangesVector(vector<vector<Point_2>>& allPossibleChanges, vector<vector<Point_2>> changedPointSets) {
    for(int i=0 ; i<changedPointSets.size() ; ++i) {
        allPossibleChanges.push_back(changedPointSets[i]);
    }
}

vector<Point_2> getChangeWithMaxArea(vector<vector<Point_2>> allPossibleChanges) {
    vector<Point_2> newPolygon;
    if(allPossibleChanges.empty())
        return newPolygon;
    newPolygon = allPossibleChanges[0];
    int maxArea = abs(getSimplePolygonFromPoints(allPossibleChanges[0]).area());
    for(int i=1 ; i<allPossibleChanges.size() ; ++i) {
        int tempArea = abs(getSimplePolygonFromPoints(allPossibleChanges[i]).area());
        if( tempArea > maxArea ) {
            maxArea = tempArea;
            newPolygon = allPossibleChanges[i];
        }
    }
    return newPolygon;
}

vector<Point_2> getChangeWithMinArea(vector<vector<Point_2>> allPossibleChanges) {
    vector<Point_2> newPolygon;
    if(allPossibleChanges.empty())
        return newPolygon;
    newPolygon = allPossibleChanges[0];
    int minArea = abs(getSimplePolygonFromPoints(allPossibleChanges[0]).area());
    for(int i=1 ; i<allPossibleChanges.size() ; ++i) {
        int tempArea = abs(getSimplePolygonFromPoints(allPossibleChanges[i]).area());
        if( tempArea < minArea ) {
            maxArea = tempArea;
            newPolygon = allPossibleChanges[i];
        }
    }
    return newPolygon;
}

