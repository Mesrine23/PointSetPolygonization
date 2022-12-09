#include "spatial_subdivision.h"

vector<Point_2> spatialSubdivision(vector<Point_2> pointset, int M, int L, int minmax, int initializationAlgo){
    vector<Spal> spals;

    spals = getSpalsFromPointSet(pointset, M);
    createOptimalSpals(spals, minmax, L, initializationAlgo);
    return mergeSpals(spals);
}

vector<Spal> getSpalsFromPointSet(vector<Point_2> pointset, int M){
    vector<Spal> result;
    pointset = sortPointset(pointset, "1b");
    Spal currentSpal;
    int counter = 0;
    while(!pointset.empty()){
        if(counter==0){
            std::copy(pointset.begin(), pointset.begin() + M, std::back_inserter(currentSpal.pointset));
            pointset.erase(std::next(pointset.begin(), 0), std::next(pointset.begin(), M-1));

            //AMA KATI PAEI LATHOS EDW EXEI GINEI MALAKIA 100%
            while(!(rightCriterion(currentSpal.pointset) and leftCriterion(pointset, currentSpal.pointset.back().y(), M)) and !pointset.empty()){
                currentSpal.pointset.push_back(pointset.front());
                pointset.erase(pointset.begin());
            }
            counter++;
        }
        else{
            currentSpal.pointset[0]=result.back().pointset.back();
            std::copy(pointset.begin(), pointset.begin() + (M-1), std::back_inserter(currentSpal.pointset));
            pointset.erase(std::next(pointset.begin(), 0), std::next(pointset.begin(), M-2));

            //AMA KATI PAEI LATHOS EDW EXEI GINEI MALAKIA 100%
            while(!(rightCriterion(currentSpal.pointset) and leftCriterion(pointset, currentSpal.pointset.back().y(), M)) and !pointset.empty()){
                currentSpal.pointset.push_back(pointset.front());
                pointset.erase(pointset.begin());
            }
            counter++;

        }

        if(pointset.size()<M-1) {
            currentSpal.pointset.insert(currentSpal.pointset.end(), pointset.begin(), pointset.end());
            pointset.clear();
        }
        result.push_back(currentSpal);
        currentSpal.pointset.clear();
    }

    return result;
}

bool rightCriterion(vector<Point_2> pointset){
    Point_2 lastPoint = pointset.back();
    for(Point_2& point : pointset){
        if(point.y()<lastPoint.y())
            return true;
    }
    return false;
}

bool leftCriterion(vector<Point_2> pointset, double connectionPointHeight, int M) {
    for ( int i = 0; i < (M - 1); i++ ) {
        if ( pointset[i].y() < connectionPointHeight )
            return true;
    }
    return false;
}

void createOptimalSpals(vector<Spal>& spals, int minmax,int L, int initalizationAlgorithm){
    //assign leftmost and rightmost to spals
    for(int i=0; i<=spals.size()-1; i++){
        if(i==0) {
            spals[i].isFirst=true;
            spals[i].isLast=false;
            spals[i].rightmost=spals[i].pointset[spals[i].pointset.size()-1];
        }
        else if (i==spals.size()-1) {
            spals[i].isFirst=false;
            spals[i].isLast=true;
            spals[i].leftmost=spals[i].pointset[0];
        }
        else{
            spals[i].isFirst=false;
            spals[i].isLast=false;
            spals[i].rightmost=spals[i].pointset[spals[i].pointset.size()-1];
            spals[i].leftmost=spals[i].pointset[0];
        }
    }

    //find convex_hull for each spal and assign marked segments
    vector<vector<Point_2>> convexHulls;
    for(Spal& spal : spals){
        vector<Point_2> convexHull = createConvexHull(spal.pointset);
        Polygon_2 temp = getSimplePolygonFromPoints(convexHull);

        if(!temp.is_clockwise_oriented()) temp.reverse_orientation();

        convexHull.clear();
        for(const Point_2& p : temp.vertices())
            convexHull.push_back(p);

        int size = convexHull.size();
        Point_2 leftmostPrevious, rightmostNext;
        for(int i=0; i<=size-1; i++){
            if(!spal.isFirst){
                //has leftmost
                if(convexHull[i]==spal.leftmost)
                    (i == 0) ? (leftmostPrevious = convexHull[size-1]) : (leftmostPrevious = convexHull[i-1]);
                spal.leftmostSeg={leftmostPrevious, convexHull[i]};
            }
            if(!spal.isLast){
                //has rightmost
                if(convexHull[i]==spal.rightmost)
                    (i == size-1) ? (rightmostNext = convexHull[0]) : (rightmostNext = convexHull[i+1]);
                spal.rightmostSeg={convexHull[i], rightmostNext};
            }
        }
        convexHulls.push_back(convexHull);
    }


    for(int i=0; i<=spals.size(); i++)
        spals[i].pointset = subdivisionSimulatedAnnealing(spals[i], convexHulls[i],L, minmax,initalizationAlgorithm);
}

vector<Point_2> mergeSpals(vector<Spal> spals){
    vector<Point_2> finalPolygon;
    for(Spal& spal : spals)
        addPointsLeftmostTillRightmost(finalPolygon, spal);
    for (vector<Spal>::reverse_iterator spal = spals.rbegin(); spal != spals.rend(); ++spal )
        addPointsRightmostTillLeftMost(finalPolygon, *spal);
    return finalPolygon;
}

void addPointsLeftmostTillRightmost(vector<Point_2>& finalResult, Spal spal){
    int rightMostIndex = findIndexOfPointInPointSet(spal.pointset, spal.rightmost);
    int leftMostIndex = findIndexOfPointInPointSet(spal.pointset, spal.leftmost);

    int index=leftMostIndex;
    int size=spal.pointset.size();
    while(true){
        if(index==rightMostIndex) break;

        finalResult.push_back(spal.pointset[index]);

        if(index==size-1) index=0;
        else index++;
    }
}

void addPointsRightmostTillLeftMost(vector<Point_2>& finalResult, Spal spal){
    int rightMostIndex = findIndexOfPointInPointSet(spal.pointset, spal.rightmost);
    int leftMostIndex = findIndexOfPointInPointSet(spal.pointset, spal.leftmost);

    int index=rightMostIndex;
    int size=spal.pointset.size();
    while(true){
        if(index==leftMostIndex) break;

        finalResult.push_back(spal.pointset[index]);

        if(index==size-1) index=0;
        else index++;
    }
}

vector<Point_2> subdivisionSimulatedAnnealing(Spal spal,vector<Point_2> convexHull, int L, int edgeSelection, int initialPolygonCalculationMethod){
    srand(time(0));
    vector<Point_2> currentPolygon;
    switch(initialPolygonCalculationMethod){
        case 1:
            currentPolygon = subdivisionConvexHullAlgo(spal,convexHull, edgeSelection);
            break;
        case 2:
            currentPolygon = subdivisionConvexHullAlgo(spal, convexHull, edgeSelection);
            break;
        default:
            cout<<"Invalid initialPolygonCalculationMethod given at simulatedAnnealing!"<<endl;
            exit(-1);
    }

    StepResult (*step) (vector<Point_2>);
    bool (*transitionValid) (vector<Point_2>, StepResult);
    void (*applyTransition) (vector<Point_2>&, StepResult);

    step = (&globalStep);
    transitionValid = (&globalTransitionValid);
    applyTransition = (&applyGlobalTransition);

    double currentEnergy, polygonArea, convexHullArea;
    int pointsetSize = currentPolygon.size();
    Polygon_2 polygonLine = getSimplePolygonFromPoints(currentPolygon);
    polygonArea = abs(polygonLine.area());
    convexHullArea = abs(getSimplePolygonFromPoints(createConvexHull(currentPolygon)).area());
    currentEnergy = calculateEnergy(polygonArea, convexHullArea, pointsetSize, edgeSelection);
    double T = 1.0;
    StepResult stepResult;
    while(T>=0){
        do {
            stepResult = step(currentPolygon);
        }
        while(!transitionValid(currentPolygon, stepResult) or markedSegmentChosen(spal, stepResult));
        double tempArea = calculateNewArea(currentPolygon, polygonArea, stepResult, 2);
        double tempEnergy = calculateEnergy(tempArea, convexHullArea, pointsetSize, edgeSelection);
        double energyDifference = tempEnergy-currentEnergy;
        if(energyDifference<0 or metropolis(energyDifference, T)){
            currentEnergy = tempEnergy;
            polygonArea = tempArea;
            applyTransition(currentPolygon, stepResult);
        }
        T-= 1.0/L;
    }
    return currentPolygon;
}

bool markedSegmentChosen(Spal spal, StepResult stepResult){
    Point_2 p=stepResult.global[0],q=stepResult.global[1],r=stepResult.global[2];
    Point_2 s=stepResult.global[3],t=stepResult.global[4];

    if(!spal.isFirst){
        if(segmentsEquivalent(spal.leftmostSeg, {s,t})) return true;
        if(segmentsEquivalent(spal.leftmostSeg, {r,q})) return true;
        if(segmentsEquivalent(spal.leftmostSeg, {q,p})) return true;
    }
    if(!spal.isLast){
        if(segmentsEquivalent(spal.rightmostSeg, {s,t})) return true;
        if(segmentsEquivalent(spal.rightmostSeg, {r,q})) return true;
        if(segmentsEquivalent(spal.rightmostSeg, {q,p})) return true;
    }
    return false;
}

vector<Point_2> subdivisionConvexHullAlgo(Spal spal, vector<Point_2> convexHull, int minmax){
    vector<Segment_2> markedSegments;
    if(!spal.isLast) markedSegments.push_back(spal.leftmostSeg);
    if(!spal.isFirst) markedSegments.push_back(spal.rightmostSeg);

    vector<Point_2> polygon, pointSet=spal.pointset;
    pointSet = sortPointset(pointSet,"1b");
    polygon = convexHull;
    removeUsedPoints(polygon,pointSet);
    addUntrackedCollinearPointsInPolygonalLine(polygon,pointSet);
    removeUsedPoints(polygon,pointSet);
    while(!pointSet.empty()) {
        vector<pair<Segment_2, Point_2>> pairOfSegmentAndClosestPoint = SubdivisionGetPairOfClosestPointToSegments(pointSet,polygon, markedSegments);
        pair<Segment_2, Point_2> pairToReplace;
        switch (minmax) {
            case 1:
                pairToReplace = minAreaPairSelection(pairOfSegmentAndClosestPoint, polygon);
                break;
            case 2:
                pairToReplace = maxAreaPairSelection(pairOfSegmentAndClosestPoint, polygon);
                break;
            default:
                cout << "Wrong edge selection method" << endl;
                exit(-1);
        }
        polygon = insertPointToPolygonPointSet(pairToReplace.second, pairToReplace.first, polygon);
        pointSet.erase(std::remove(pointSet.begin(), pointSet.end(), pairToReplace.second), pointSet.end());

    }
    return polygon;
}

vector<pair<Segment_2,Point_2>> SubdivisionGetPairOfClosestPointToSegments(vector<Point_2> internalPoints, vector<Point_2> polygon, vector<Segment_2> markedSegments) {
    vector<pair<Segment_2,Point_2>> pairs;
    vector<Segment_2> polygonEdges = getPolygonEdgesFromPoints(polygon);
    for(Segment_2 edge : polygonEdges) {
        bool flag=false;
        for(Segment_2 markedSegment : markedSegments){
            if( segmentsEquivalent(edge, markedSegment)) flag = true;
        }
        if(flag) continue;
        Point_2 closestPoint = findClosestPointToSegment(edge,internalPoints);
        pairs.push_back(make_pair(edge,closestPoint));
    }
    return pairs;
};

