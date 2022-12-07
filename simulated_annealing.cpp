#include "simulated_annealing.h"

vector<Point_2> simulatedAnnealing(vector <Point_2> pointset, int L, int edgeSelection, int stepType, int initialPolygonCalculationMethod){
    srand(time(0));
    vector<Point_2> currentPolygon;
    switch(initialPolygonCalculationMethod){
        case 1:
            currentPolygon = IncrementalAlg(pointset, edgeSelection ,"1a");
            break;
        case 2:
            currentPolygon = ConvexHullAlg(pointset, edgeSelection);
            break;
        default:
            cout<<"Invalid initialPolygonCalculationMethod given at simulatedAnnealing!"<<endl;
            exit(-1);
    }

    StepResult (*step) (vector<Point_2>);
    bool (*transitionValid) (vector<Point_2>, StepResult);
    void (*applyTransition) (vector<Point_2>&, StepResult);
    switch(stepType){
        case 1:
            step = (&localStep);
            transitionValid = (&localTransitionValid);
            applyTransition = (&applyLocalTransition);
            break;
        case 2:
            step = (&globalStep);
            transitionValid = (&globalTransitionValid);
            applyTransition = (&applyGlobalTransition);
            break;
        default:
            cout<<"Invalid stepType given at simulatedAnnealing!"<<endl;
            exit(1);
    }

    double currentEnergy, polygonArea, convexHullArea;
    int pointsetSize = pointset.size();
    Polygon_2 polygonLine = getSimplePolygonFromPoints(pointset);
    polygonArea = abs(polygonLine.area());
    convexHullArea = abs(getSimplePolygonFromPoints(createConvexHull(pointset)).area());
    currentEnergy = calculateEnergy(polygonArea, convexHullArea, pointsetSize, edgeSelection);
    double T = 1.0;
    StepResult stepResult;
    while(T>=0){
        do {
            stepResult = step(currentPolygon);
        }
        while(!transitionValid(currentPolygon, stepResult));
        double tempArea = calculateNewArea(pointset, polygonArea, stepResult, stepType);
        double tempEnergy = calculateEnergy(tempArea, convexHullArea, pointsetSize, edgeSelection);
        double energyDifference = tempEnergy-currentEnergy;
        if(energyDifference<0 or metropolis(energyDifference, T)){
            currentEnergy = tempEnergy;
            polygonArea = tempArea;
            applyTransition(pointset, stepResult);
        }
        T-= 1.0/L;
    }

}

bool metropolis(double energyDifference, double T){
    double R = ((double)rand()/(double)RAND_MAX);
    return exp(-energyDifference/T)>=R;
}

double calculateEnergy(double polygonArea, double convexHullArea, int pointsetSize, int edgeSelection){
    double energy;
    switch(edgeSelection){
        case 1:
            energy = pointsetSize * (polygonArea/convexHullArea);
            break;
        case 2:
            energy = pointsetSize * (1-polygonArea/convexHullArea);
            break;
        default:
            cout<<"Invalid edgeSelection given at calculateEnergy!"<<endl;
            exit(5);
    }
    return energy;
}

double calculateNewArea(vector<Point_2> pointset, double oldArea, StepResult stepResult, int stepType){
    Point_2 q,r,p,s,t;
    double newArea;
    int qIndex, rIndex;
    switch(stepType){
        case 1:
            q=stepResult.local.first;
            r=stepResult.local.second;
            qIndex = findIndexOfPointInPointSet(pointset, q);
            rIndex = findIndexOfPointInPointSet(pointset, r);
            (rIndex == (pointset.size()-1)) ? (s = pointset[0]) : (s = pointset[rIndex+1]);
            (qIndex == 0) ? (s = pointset[pointset.size()-1]) : (s = pointset[qIndex-1]);
            newArea=oldArea-abs(area(p,r,q))+abs(area(q,r,s));
            break;
        case 2:
            p=stepResult.global[0];
            q=stepResult.global[1];
            r=stepResult.global[2];
            s=stepResult.global[3];
            t=stepResult.global[4];
            newArea=oldArea+abs(area(q,r,p))-abs(area(s,q,t));
            break;
        default:
            cout<<"Invalid stepType given at calculateNewArea!"<<endl;
            exit(3);
    }
    return newArea;
}

StepResult localStep(vector<Point_2> pointset){
    StepResult res;
    int randomIndex = rand()%pointset.size();

    if(randomIndex == (pointset.size()-1)){
        res.local=make_pair(pointset[randomIndex], pointset.front());
        return res;
    }
    else{
        res.local=make_pair(pointset[randomIndex], pointset[randomIndex+1]);
        return res;
    }
}

StepResult globalStep(vector<Point_2> pointset){
    StepResult res;

    int randomIndex1, randomIndex2;
    do{
        randomIndex1 = rand()%pointset.size();
        randomIndex2 = rand()%pointset.size();
    }while(abs(randomIndex1-randomIndex2)>2);

    Point_2 q,s;
    q = pointset[randomIndex1];
    s = pointset[randomIndex2];
    Point_2 p, r;
    (randomIndex1 == (pointset.size()-1)) ? (r = pointset[0]) : (r = pointset[randomIndex1+1]);
    (randomIndex1 == 0) ? (p = pointset[pointset.size()-1]) : (p = pointset[randomIndex1-1]);

    Point_2 t;
    (randomIndex2 == (pointset.size()-1)) ? (t = pointset[0]) : (t = pointset[randomIndex2+1]);

    res.global = {p,q,r,s,t};
    return res;
}

bool localTransitionValid(vector<Point_2>polygon, StepResult stepResult){
    Point_2 q = stepResult.local.first;
    Point_2 r = stepResult.local.second;

    Point_2 p, s;
    Tree tree;
    int size = polygon.size();
    for(int i=0; i<size; i++) {
        tree.insert(polygon[i]);
        if ( polygon[i] == q ) {
            if ( i == 0 ) p = polygon.back();
            else p = polygon[i - 1];
        }
        if ( polygon[i] == r ) {
            if ( i == (size - 1)) s = polygon.front();
            else s = polygon[i + 1];
        }
    }

    Segment_2 pr={p,r}, qs{q,s};
    if(segmentIntersectsSegment(pr, qs)) return false;

    vector<Point_2> temp = {r,p,s};
    int min_x=q.x(), min_y = q.y(), max_x = q.x(), max_y = q.y();
    for(Point_2 point : temp){
        if(point.x()>max_x) max_x=point.x();
        if(point.x()<min_x) min_x=point.x();

        if(point.y()>max_y) max_y=point.y();
        if(point.y()<min_y) min_y=point.y();
    }
    Point_2 min={min_x, min_y}, max={max_x, max_y};

    std::list<Point_2> result;
    Fuzzy_iso_box exact_range(min, max);
    tree.search(std::back_inserter(result), exact_range);
    for(auto const& listPoint : result){
        if(listPoint==p or listPoint==q or listPoint==s or listPoint==r) continue;

        int positionOfNewPoint = findIndexOfPointInPointSet(polygon,listPoint);
        Point_2 lowerPoint, upperPoint;
        (positionOfNewPoint == (polygon.size()-1)) ? (lowerPoint = polygon[0]) : (lowerPoint = polygon[positionOfNewPoint+1]);
        (positionOfNewPoint == 0) ? (upperPoint = polygon[polygon.size()-1]) : (upperPoint = polygon[positionOfNewPoint-1]);

        Segment_2 seg1={lowerPoint, listPoint}, seg2={upperPoint,listPoint};
        if(segmentIntersectsSegment(pr, seg1) or segmentIntersectsSegment(qs, seg1)) return false;
        if(segmentIntersectsSegment(pr, seg2) or segmentIntersectsSegment(qs, seg2)) return false;
    }
    return true;
}

void applyLocalTransition(vector<Point_2>& polygon, StepResult stepResult){
    Point_2 q=stepResult.local.first, r=stepResult.local.second;

    int qIndex = findIndexOfPointInPointSet(polygon, q);
    int rIndex = findIndexOfPointInPointSet(polygon, r);

    polygon[qIndex]=r;
    polygon[rIndex]=q;
}

bool globalTransitionValid(vector<Point_2> polygon, StepResult stepResult){
    Point_2 p=stepResult.global[0],q=stepResult.global[1],r=stepResult.global[2];
    Point_2 s=stepResult.global[3],t=stepResult.global[4];

    //int pIndex= findIndexOfPointInPointSet(polygon, p), qIndex= findIndexOfPointInPointSet(polygon, p), rIndex= findIndexOfPointInPointSet(polygon, p), sIndex= findIndexOfPointInPointSet(polygon, p), tIndex= findIndexOfPointInPointSet(polygon, p);

    Segment_2 pr={p,r},sq={s,q}, qt={q,t};
    if(segmentIntersectsSegment(pr,sq) or segmentIntersectsSegment(pr,qt)) return false;

    vector<Segment_2> edges = getPolygonEdgesFromPoints(polygon);
    for(Segment_2 edge : edges){
        if(!(segmentsEquivalent(edge,{s,t}) or segmentsEquivalent(edge, {r,q}) or segmentsEquivalent(edge, {q,p}))){
            if(segmentIntersectsSegment(sq, edge) and !edgesAreAdjacent(sq, edge)) return false;
            if(segmentIntersectsSegment(qt, edge) and !edgesAreAdjacent(qt, edge)) return false;
            if(segmentIntersectsSegment(pr, edge) and !edgesAreAdjacent(pr, edge)) return false;
        }
    }
    return true;
}

template <typename t> void move(std::vector<t>& v, size_t oldIndex, size_t newIndex)
{
    if (oldIndex > newIndex)
        std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
    else
        std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
}

void applyGlobalTransition(vector<Point_2>& polygon, StepResult stepResult){
    Point_2 p=stepResult.global[0],q=stepResult.global[1],r=stepResult.global[2];
    Point_2 s=stepResult.global[3],t=stepResult.global[4];

    int pIndex= findIndexOfPointInPointSet(polygon, p), qIndex= findIndexOfPointInPointSet(polygon, q);
    int rIndex= findIndexOfPointInPointSet(polygon, r), sIndex= findIndexOfPointInPointSet(polygon, s), tIndex= findIndexOfPointInPointSet(polygon, t);

    if(sIndex>tIndex)
        move(polygon, qIndex, tIndex);
    else
        move(polygon, qIndex, sIndex);

}
