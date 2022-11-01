#include "cgalStructsFunctions.h"

vector<Segment_2> getPolygonEdgesFromPoints(vector<Point_2> pointSet) {
    vector<Segment_2> polygon;
    for(int i=0 ; i<pointSet.size() ; ++i){
        (i != (pointSet.size()-1)) ? polygon.push_back(Segment_2(pointSet[i],pointSet[i+1])) : polygon.push_back(Segment_2(pointSet[i],pointSet[0]));
    }
    return polygon;
}

Polygon_2 getSimplePolygonFromPoints(vector<Point_2> pointSet) {
    Polygon_2 polygon;
    for(int i=0 ; i<pointSet.size() ; ++i)
        polygon.push_back(pointSet[i]);
    if (polygon.is_simple()) {
        return polygon;
    } else {
        cout << endl << "Not a simple polygon!" << endl;
        //*((char*)0) = 0;
        exit(-1);
    }
}

vector<Point_2> sortPointset(vector<Point_2> pointSet, string sortMethod) {
    if (sortMethod == "1a")
        std::sort(pointSet.begin(), pointSet.end(), comparePointXdesc);
    else if (sortMethod == "1b")
        std::sort(pointSet.begin(), pointSet.end(), comparePointXasc);
    else if (sortMethod == "2a")
        std::sort(pointSet.begin(), pointSet.end(), comparePointYdesc);
    else if (sortMethod == "2b")
        std::sort(pointSet.begin(), pointSet.end(), comparePointYasc);
    else {
        cout << "Invalid sort method." << endl;
        exit(-1);
    }
    return pointSet;
}

bool comparePointXdesc(Point_2 A, Point_2 B) {
    if (A.x()==B.x()) return A.y()<B.y();
    return A.x()>B.x();
}
bool comparePointXasc(Point_2 A, Point_2 B) {
    if (A.x()==B.x()) return A.y()<B.y();
    return A.x()<B.x();
}
bool comparePointYdesc(Point_2 A, Point_2 B) {
    if (A.y()==B.y()) return A.x()<B.x();
    return A.y()>B.y();
}
bool comparePointYasc(Point_2 A, Point_2 B) {
    if (A.y()==B.y()) return A.x()<B.x();
    return A.y()<B.y();
}

vector<Segment_2> findChainOfEdges(Point_2 pointA, Point_2 pointB, vector<Point_2> polygon, bool sameOrientation) {
    //cout << "I will try to find chain of edges!" << endl;
    //cout << "[" << pointA.x() << "," << pointA.y() << "][" << pointB.x() << "," << pointB.y() << "] <- trying to find its chain in..." << endl;
    //printPointSet(polygon);
    Point_2 startingPoint, endingPoint;
    if(sameOrientation){
        //cout<<"Same orientation!"<<endl;
        startingPoint = pointA;
        endingPoint = pointB;
    }
    else{
        //cout<<"Different orientation!"<<endl;
        startingPoint = pointB;
        endingPoint = pointA;
    }
    vector<Segment_2> chainedEdges;
    auto indexOfStartingPoint = find(polygon.begin(), polygon.end(), startingPoint);
    if (indexOfStartingPoint==polygon.end()) {
        printErrorPointNotFound(pointA, polygon);
    }
    int index = indexOfStartingPoint - polygon.begin();
    bool foundEndOfChain = false;
    while(!foundEndOfChain) {
        if (index!=(polygon.size()-1)) {
            //cout << "Pushing: [" << polygon[index].x() << "," << polygon[index].y() << "][" << polygon[index+1].x() << "," << polygon[index+1].y() << "]" << endl;
            chainedEdges.push_back(Segment_2(polygon[index],polygon[index+1]));
            ++index;
            if (polygon[index]==endingPoint) { foundEndOfChain = true; }
        }
        else {
            //cout << "Pushing: [" << polygon[index].x() << "," << polygon[index].y() << "][" << polygon[0].x() << "," << polygon[0].y() << "]" << endl;
            chainedEdges.push_back(Segment_2(polygon[index],polygon[0]));
            index = 0;
            if (polygon[0]==endingPoint) { foundEndOfChain = true; }
        }
    }
    //cout << endl;
    return chainedEdges;
}

bool segmentIntersectsPolygonEdge(Segment_2 seg, Segment_2 polygonEdge){
    const auto result = intersection(seg, polygonEdge);
    if(result){
        if (const Segment_2* s = boost::get<Segment_2>(&*result))/*(CGAL::collinear(seg[0],polygonEdge[1],polygonEdge[0]))*/{
            //cout<<"COLLINEAR: ("<<seg[0].x()<<","<<seg[0].y()<<") ("<<polygonEdge[0].x()<<","<<polygonEdge[0].y()<<") ("<<polygonEdge[1].x()<<","<<polygonEdge[1].y()<<")"<<endl;
            return true;
        } else {
            const Point_2* p = boost::get<Point_2 >(&*result);
            if(!(*p==seg.end())) return true;
        }
    }
    return false;
}

bool isEdgeVisibleFromPoint(Point_2 point, Segment_2 edge, vector<Segment_2> polygon){
    Point_2 edgeMidpoint((edge.start().x()+edge.end().x())/2, (edge.start().y()+edge.end().y())/2);
    Segment_2 edgeA(point, edge[0]), edgeB(point, edge[1]), edgeMid(point, edgeMidpoint);
    for(Segment_2 polygonEdge : polygon){
        if(segmentIntersectsPolygonEdge(edgeA, polygonEdge) || segmentIntersectsPolygonEdge(edgeB, polygonEdge) ||
           segmentIntersectsPolygonEdge(edgeMid, polygonEdge)) return false;
    }
    return true;

}

bool segmentsEquivalent(Segment_2 A, Segment_2 B){
    return((A.start()==B.end() && A.end()==B.start())||A==B);
}

bool sameOrientation(vector<Point_2> polygonVector, vector<Point_2> convexHullVector){
    Polygon_2 polygon = getSimplePolygonFromPoints(polygonVector);
    Polygon_2 convexHull = getSimplePolygonFromPoints(convexHullVector);

    return (polygon.orientation()==convexHull.orientation());
}

vector<Point_2> insertPointToPolygonPointSet(Point_2 point, Segment_2 edgeToBreak, vector<Point_2> polygon) {
    auto indexOfEdge1 = find(polygon.begin(), polygon.end(), edgeToBreak[0]);
    auto indexOfEdge2 = find(polygon.begin(), polygon.end(), edgeToBreak[1]);
    int index1 = indexOfEdge1 - polygon.begin();
    int index2 = indexOfEdge2 - polygon.begin();
    if ((index1==0 || index1==polygon.size()-1) && (index2==0 || index2==polygon.size()-1)) {
        polygon.push_back(point);
        return polygon;
    }
    //cout << "Index of point: [" << edgeToBreak[0].x() << "," << edgeToBreak[0].y() << "] ~> " << index1 << endl;
    //cout << "Index of point: [" << edgeToBreak[1].x() << "," << edgeToBreak[1].y() << "] ~> " << index2 << endl;
    int startingIndex;
    (index1 > index2) ? (startingIndex = index2) : (startingIndex = index1);
    //cout << "Will place it in position ~> " << startingIndex+1 << endl;
    auto insertPosition = polygon.begin() + startingIndex+1;
    polygon.insert(insertPosition,point);
    return polygon;
}

int findIndexOfPointInPointSet(vector<Point_2> pointSet, Point_2 point) {
    int index = -1;
    for(int i=0 ; i<pointSet.size() ; ++i) {
        if( pointSet[i].x() == point.x() && pointSet[i].y() == point.y() ) {
            index = i;
            break;
        }
    }
    return index;
}

vector<Point_2> createConvexHull(vector<Point_2> pointSet) {
    vector<Point_2> convexHull;
    std::vector<std::size_t> indices(pointSet.size()), out;
    std::iota(indices.begin(), indices.end(),0);
    CGAL::convex_hull_2(indices.begin(), indices.end(), std::back_inserter(out),
                        Convex_hull_traits_2(CGAL::make_property_map(pointSet)));
    for( std::size_t i : out )
        convexHull.push_back(pointSet[i]);
    return convexHull;
}

void printErrorPointNotFound(Point_2 point, vector<Point_2> polygon) {
    cout << endl;
    cout << point.x() << " " << point.y() << endl;
    cout << "^This *Point* was not found in vector!" << endl;
    //printPointSet(polygon);
    exit(-1);
}

vector<int> createVectorOfRandomInts(int range) {
    constexpr int SIZE = range;
    std::vector<int> randomInts(SIZE);
    std::iota(begin(randomInts), end(randomInts), 0);
    std::random_shuffle(begin(randomInts), end(randomInts));
    randomInts.resize(SIZE);
//    for(int i=0 ; i<SIZE ; ++i) {
//        cout << values[i] << endl;
//    }
    return randomInts;
}
