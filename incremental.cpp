#include "incremental.h"

vector<Point_2> IncrementalAlg(vector<Point_2> pointSet, int edgeSelectionMethod, string initMethod) {
    vector<Point_2> sortedPointSet, usedPoints, convexHull, polygon;

    //cout << "Size of input file: " << pointSet.size() << endl;

    sortedPointSet = sortPointset(pointSet, initMethod);

    //cout << "Size of sortedPointSet: " << sortedPointSet.size() << endl;

    //cout<<"Starting triangle is:\n";
    for(int i=0; i<3; i++) {
        polygon.push_back(sortedPointSet.front());
        //<<"["<<sortedPointSet.front().x()<<" ]["<<sortedPointSet.front().y()<<"]"<<endl;
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

    //exit(1);

    //cout<<endl;
    srand(time(NULL));
    while(!sortedPointSet.empty()){
        //printPointSet(polygon);
        //printSegList(polygon);
        //cout<<"Creating CH!"<<endl;
        convexHull = createConvexHull(usedPoints);
        //cout << "Old convex Hull:" << endl;
        //printPointSet(convexHull);

        Point_2 currentPoint = sortedPointSet.front();
        sortedPointSet.erase(sortedPointSet.begin());
        usedPoints.push_back(currentPoint);

        //cout<<"Creating the new CH!"<<endl;
        vector<Point_2> newConvexHull = createConvexHull(usedPoints);
        //cout << "New convex Hull:" << endl;
        //printPointSet(newConvexHull);
        //cout<<"Calculating Red Edges when adding Point ["<<currentPoint.x()<<" , "<<currentPoint.y()<<"]"<<endl;
        vector<Segment_2> redEdges = getRedEdges(convexHull, newConvexHull,currentPoint);
        //cout<<"Calculating Replaceable Edges!"<<endl;
        vector<Segment_2> replaceableEdges = calculateVisibleEdges(redEdges,polygon,currentPoint,convexHull);
        //cout<<"Calculating Edge to Replace!"<<endl;
        Segment_2 edgeToReplace = getPolygonEdgeToReplace(replaceableEdges,currentPoint,polygon,edgeSelectionMethod);
        //cout<<"Replacing Edge ["<<edgeToReplace.start().x()<<" ,"<<edgeToReplace.start().y()<<"]["<<edgeToReplace.end().x()<<" ,"<<edgeToReplace.end().y()<<"]"<<endl;
        polygon = insertPointToPolygonPointSet(currentPoint,edgeToReplace,polygon);
        //cout<<endl<<endl;
    }
    return polygon;
}

vector<Segment_2> getRedEdges(vector<Point_2> oldConvexHull, vector<Point_2> newConvexHull, Point_2 newPoint){
    vector<Segment_2> replacedEdges;
    int positionOfNewPoint = findIndexOfPointInPointSet(newConvexHull,newPoint);
    Point_2 upperPoint, lowerPoint;

    (positionOfNewPoint == (newConvexHull.size()-1)) ? (lowerPoint = newConvexHull[0]) : (lowerPoint = newConvexHull[positionOfNewPoint+1]);
    (positionOfNewPoint == 0) ? (upperPoint = newConvexHull[newConvexHull.size()-1]) : (upperPoint = newConvexHull[1]);

    //ιf(isPointOnCHPerimeter(newPoint, oldConvexHull, replacedEdges)) return replacedEdges;

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
            //cout<<"Pushed back Red Edge ["<<oldConvexEdge.start().x()<<" ,"<<oldConvexEdge.start().y()<<"]["<<oldConvexEdge.end().x()<<" ,"<<oldConvexEdge.end().y()<<"]"<<endl;
        }
    }
    return replacedEdges;
}

Segment_2 randomEdgeSelection(vector<Segment_2> replaceableEdges) {
    if(replaceableEdges.size()==1)
        return replaceableEdges[0];
    int randomIndex = (std::rand() % (replaceableEdges.size()));
    //cout << "Replaceable edges size: " << replaceableEdges.size()-1 << endl;
    //cout << "Index: " << randomIndex << endl;
    return replaceableEdges[randomIndex];
}

Segment_2 minAreaEdgeSelection(vector<Segment_2> replaceableEdges, Point_2 currentPoint, vector<Point_2> polygonPointSet) {
    Segment_2 edgeToReturn = replaceableEdges[0];
    vector<Point_2> currentPolygonPointSet = insertPointToPolygonPointSet(currentPoint,edgeToReturn,polygonPointSet);
    long double minArea = getSimplePolygonFromPoints(currentPolygonPointSet).area();
    for(int i=1 ; i<replaceableEdges.size() ; ++i) {
        Segment_2 tempEdgeToReturn = replaceableEdges[i];
        vector<Point_2> tempPolygonPointSet = insertPointToPolygonPointSet(currentPoint,tempEdgeToReturn,polygonPointSet);
        long double tempMinArea = getSimplePolygonFromPoints(currentPolygonPointSet).area();
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
    long double maxArea = getSimplePolygonFromPoints(currentPolygonPointSet).area();
    for(int i=1 ; i<replaceableEdges.size() ; ++i) {
        Segment_2 tempEdgeToReturn = replaceableEdges[i];
        vector<Point_2> tempPolygonPointSet = insertPointToPolygonPointSet(currentPoint,tempEdgeToReturn,polygonPointSet);
        long double tempMaxArea = getSimplePolygonFromPoints(currentPolygonPointSet).area();
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
                //cout<<"EQUIVALENT Found Visible Edge ["<<redEdge.start().x()<<" ,"<<redEdge.start().y()<<"]["<<redEdge.end().x()<<" ,"<<redEdge.end().y()<<"]"<<endl;
                foundEdge = true;
                break;
            }
        }
        if(!foundEdge) {
            vector<Segment_2> chainedEdges = findChainOfEdges(redEdge.start(),redEdge.end(),polygonPointSet, sameOrientation(polygonPointSet, convexHull));
            for(Segment_2 chainedEdge: chainedEdges){
                if(isEdgeVisibleFromPoint(added_point,chainedEdge,polygonEdges)) {
                    visibleEdges.push_back(chainedEdge);
                    //cout<<"Found Visible Edge ["<<chainedEdge.start().x()<<" ,"<<chainedEdge.start().y()<<"]["<<chainedEdge.end().x()<<" ,"<<chainedEdge.end().y()<<"]"<<endl;
                }
            }
        }
    }
    return visibleEdges;
}