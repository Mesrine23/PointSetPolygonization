#include "local_search.h"

vector<Point_2> LocalSearchAlg(vector<Point_2> polygon, int chainLength, double threshold, bool isMax, long & time) {
    auto started=chrono::high_resolution_clock::now();
    long double polygonArea = abs(getSimplePolygonFromPoints(polygon).area());
    long double newPolygonArea = 0;

    while(abs(polygonArea-newPolygonArea) >= threshold) {
        vector<vector<Point_2>> allPossiblePolygonChanges;
        for(int pointIndex=0 ; pointIndex < polygon.size() ; ++pointIndex) {
            vector<vector<Point_2>> possiblePolygonChangesForSinglePoint = getPossiblePolygonChangesForSinglePoint(polygon,pointIndex);
            addChangedPointSetsInPossibleChangesVector(allPossiblePolygonChanges, possiblePolygonChangesForSinglePoint);
            for(int length=2 ; length<=chainLength ; ++length) {
                vector<vector<Point_2>> possiblePolygonChangesForMultiplePoints = getPossiblePolygonChangesForMultiplePoints(polygon,pointIndex,length);
                addChangedPointSetsInPossibleChangesVector(allPossiblePolygonChanges, possiblePolygonChangesForMultiplePoints);
            }
        }
        vector<Point_2> newPolygon;
        if (isMax) {
            newPolygon = getChangeWithMaxArea(allPossiblePolygonChanges);
            if(newPolygon.empty())
                break;
            newPolygonArea = abs(getSimplePolygonFromPoints(newPolygon).area());
            polygonArea = abs(getSimplePolygonFromPoints(polygon).area());
            //cout << "newPolygonArea = " << newPolygonArea << " <~> currentPolygonArea = " << polygonArea << endl;
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
        auto done = chrono::high_resolution_clock::now();
        long passed_time = std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();
        if(time-passed_time<0){
            time=-1;
            return polygon;
        }
    }
    return polygon;
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
    vector<vector<Point_2>> possiblePolygonChanges;
    Point_2 nextPoint = getNextPointFromChainOfPoints(polygon,indexOfPoint,length);
    Point_2 previousPoint = getPreviousPointFromChainOfPoints(polygon,indexOfPoint);
    vector<Point_2> reversedChainOfPoints;
    for(int i=0 ; i<length ; i++) {
        if(indexOfPoint==polygon.size()-1) {
            reversedChainOfPoints.insert(reversedChainOfPoints.begin(),polygon[indexOfPoint]);
            polygon.erase(polygon.begin() + indexOfPoint);
            indexOfPoint=0;
            continue;
        }
        reversedChainOfPoints.insert(reversedChainOfPoints.begin(),polygon[indexOfPoint]);
        polygon.erase(polygon.begin() + indexOfPoint);
    }
    for(int i=0 ; i<polygon.size()-1 ; ++i) {
        if(polygon[i]==nextPoint || polygon[i]==previousPoint || polygon[i+1]==nextPoint || polygon[i+1]==previousPoint)
            continue;
        vector<Point_2> tempPolygon = polygon;
        tempPolygon.insert( (tempPolygon.begin()+i+1) , reversedChainOfPoints.begin() , reversedChainOfPoints.end() );
        if(isPolygonSimple(tempPolygon))
            possiblePolygonChanges.push_back(tempPolygon);
    }
    if(!(polygon[polygon.size()-1]==nextPoint || polygon[polygon.size()-1]==previousPoint || polygon[0]==nextPoint || polygon[0]==previousPoint)) {
        vector<Point_2> tempPolygon = polygon;
        tempPolygon.insert( (tempPolygon.end()) , reversedChainOfPoints.begin() , reversedChainOfPoints.end() );
        if(isPolygonSimple(tempPolygon))
            possiblePolygonChanges.push_back(tempPolygon);
    }
    return possiblePolygonChanges;
}

void addChangedPointSetsInPossibleChangesVector(vector<vector<Point_2>>& allPossibleChanges, vector<vector<Point_2>> changedPointSets) {
    for(int i=0 ; i<changedPointSets.size() ; ++i) {
        allPossibleChanges.push_back(changedPointSets[i]);
    }
}

vector<Point_2> getChangeWithMaxArea(vector<vector<Point_2>> allPossibleChanges) {
    vector<Point_2> newPolygon;
    if(allPossibleChanges.empty()) {
        //cout << "no possible changes found" << endl;
        return newPolygon;
    }
    newPolygon = allPossibleChanges[0];
    long double maxArea = abs(getSimplePolygonFromPoints(allPossibleChanges[0]).area());
    //cout << "current max area: " << maxArea << endl;
    for(int i=1 ; i<allPossibleChanges.size() ; ++i) {
        long double tempArea = abs(getSimplePolygonFromPoints(allPossibleChanges[i]).area());
        if( tempArea > maxArea ) {
            //cout << tempArea << " > " << maxArea << endl;
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
    long double minArea = abs(getSimplePolygonFromPoints(allPossibleChanges[0]).area());
    for(int i=1 ; i<allPossibleChanges.size() ; ++i) {
        long double tempArea = abs(getSimplePolygonFromPoints(allPossibleChanges[i]).area());
        if( tempArea < minArea ) {
            minArea = tempArea;
            newPolygon = allPossibleChanges[i];
        }
    }
    return newPolygon;
}

Point_2 getPreviousPointFromChainOfPoints(vector<Point_2> polygon, int indexOfStartingPoint) {
    if (indexOfStartingPoint==0)
        return polygon[polygon.size()-1];
    else
        return polygon[indexOfStartingPoint-1];
}

Point_2 getNextPointFromChainOfPoints(vector<Point_2> polygon, int indexOfStartingPoint, int length) {
    int indexOfNextItem = indexOfStartingPoint;
    for(int i=0 ; i<length ; ++i) {
        (indexOfNextItem==polygon.size()-1) ? (indexOfNextItem=0) : (indexOfNextItem++);
    }
    return polygon[indexOfNextItem];
}

