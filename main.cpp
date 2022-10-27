#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <fstream>
#include <random>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/intersections.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2      Point_2;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef K::Segment_2    Segment_2;
typedef CGAL::Convex_hull_traits_adapter_2<K,CGAL::Pointer_property_map<Point_2>::type > Convex_hull_traits_2;

using namespace std;
/*--------------------------------------------------------------------------------------------------------------------*/
void printPointSet(vector<Point_2>);
void printUserInput(string, string, string, string, string);
void printErrorPointNotFound(Point_2, vector<Point_2>);
vector<Segment_2> getPolygonEdgesFromPoints(vector<Point_2>);
Polygon_2 getSimplePolygonFromPoints(vector<Point_2>);
vector<Point_2> ProcessInputFile(string);
bool comparePointXdesc(Point_2, Point_2);
bool comparePointXasc(Point_2, Point_2);
bool comparePointYdesc(Point_2, Point_2);
bool comparePointYasc(Point_2, Point_2);
int findIndexOfPointInPointSet(vector<Point_2>, Point_2);
vector<Point_2> sortPointset(vector<Point_2>, string);
vector<Point_2> createConvexHull(vector<Point_2>);
vector<Segment_2> getRedEdges(vector<Point_2>, vector<Point_2>, Point_2);
vector<Segment_2> findChainOfEdges(Point_2, Point_2, vector<Point_2>, bool);
bool isEdgeVisibleFromPoint(Point_2, Segment_2, vector<Segment_2>);
bool segmentsEquivalent(Segment_2, Segment_2);
vector<Segment_2> calculateVisibleEdges(vector<Segment_2>, vector<Point_2>, Point_2, vector<Point_2>);
Segment_2 randomEdgeSelection(vector<Segment_2>);
Segment_2 minAreaEdgeSelection(vector<Segment_2>, Point_2, vector<Point_2>);
Segment_2 maxAreaEdgeSelection(vector<Segment_2>, Point_2, vector<Point_2>);
Segment_2 getPolygonEdgeToReplace(vector<Segment_2>, Point_2, vector<Point_2>, int);
vector<Point_2> insertPointToPolygonPointSet(Point_2, Segment_2, vector<Point_2>);
vector<Point_2> IncrementalAlg(vector<Point_2>, int, string);
/*--------------------------------------------------------------------------------------------------------------------*/
//segList={segment[(1,2),(3,4)],segment[(5,2),(-1,-3)]}
void printSegList(vector<Point_2> points){
    int size = points.size();
    cout<<"segList={";
    for(int i=0; i<(points.size()-1); i++){
        cout<<"segment[("<<points[i].x()<<","<<points[i].y()<<"),("<<points[i+1].x()<<","<<points[i+1].y()<<")],";
    }
    cout<<"segment[("<<points.back().x()<<","<<points.back().y()<<"),("<<points[0].x()<<","<<points[0].y()<<")]}";
}

void printPointSet(vector<Point_2> pointSet) {
    cout << "PointList({";
    for(int i=0 ; i < pointSet.size()-1 ; ++i) {
        cout << "{" << to_string(pointSet[i].x()) + "," + to_string(pointSet[i].y()) << "},";
    }
    cout << "{" << to_string(pointSet[pointSet.size()-1].x()) + "," + to_string(pointSet[pointSet.size()-1].y()) << "}";
    cout << "})" << endl;
}

void printUserInput(string inputFileName, string outputFileName, string algorithmName, string edgeSelection, string incrementalInit) {
    cout << endl << "User input:" << endl;
    cout << "input file: " + inputFileName << endl;
    cout << "output file: " + outputFileName << endl;
    cout << "algorithm: " + algorithmName << endl;
    cout << "edge selection: " + edgeSelection << endl;
    cout << "incremental initialization: " + incrementalInit << endl << endl;
}

void printErrorPointNotFound(Point_2 point, vector<Point_2> polygon) {
    cout << endl;
    cout << point.x() << " " << point.y() << endl;
    cout << "^This *Point* was not found in vector!" << endl;
    printPointSet(polygon);
    exit(-1);
}

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

vector<Point_2> ProcessInputFile(string file_name) {
    ifstream file(file_name);
    vector<Point_2> pointSet;
    int lineIterator, xCordinate, yCordinate;
    string garbageInfo;
    getline(file, garbageInfo);
    getline(file, garbageInfo);
    while (file >> lineIterator >> xCordinate >> yCordinate)
        //cout << "[" << xCordinate << "," << yCordinate << "]" << endl;
        pointSet.push_back(Point_2(xCordinate, yCordinate));
    return pointSet;
}

bool comparePointXdesc(Point_2 A, Point_2 B) {
    if (A.x()==B.x()) return A.y()>B.y();
    return A.x()>B.x();
}
bool comparePointXasc(Point_2 A, Point_2 B) {
    if (A.x()==B.x()) return A.y()>B.y();
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

bool isPointOnCHPerimeter(Point_2 point, vector<Point_2> convex_hull, vector<Segment_2>& replaceableEdges){
    for(int i=0; i<(convex_hull.size()-2); i++){
        Segment_2 currentEdge(convex_hull[i], convex_hull[i+1]);
        if(do_intersect(currentEdge,point)){
            replaceableEdges.push_back(currentEdge);
            //cout<< "Found edge on CH perimeter ["<<currentEdge.start().x()<<" ,"<<currentEdge.start().y()<<"]["<<currentEdge.end().x()<<" ,"<<currentEdge.end().y()<<"]"<<endl;
            return true;
        }
    }

    Segment_2 lastSegment(convex_hull.back(), convex_hull.front());
    if(do_intersect(lastSegment, point)){
        replaceableEdges.push_back(lastSegment);
        //cout<< "Found edge on CH perimeter ["<<lastSegment.start().x()<<" ,"<<lastSegment.start().y()<<"]["<<lastSegment.end().x()<<" ,"<<lastSegment.end().y()<<"]"<<endl;
        return true;
    }
    return false;
}

vector<Segment_2> getRedEdges(vector<Point_2> oldConvexHull, vector<Point_2> newConvexHull, Point_2 newPoint){
    vector<Segment_2> replacedEdges;
    int positionOfNewPoint = findIndexOfPointInPointSet(newConvexHull,newPoint);
    Point_2 upperPoint, lowerPoint;

    (positionOfNewPoint == (newConvexHull.size()-1)) ? (lowerPoint = newConvexHull[0]) : (lowerPoint = newConvexHull[positionOfNewPoint+1]);
    (positionOfNewPoint == 0) ? (upperPoint = newConvexHull[newConvexHull.size()-1]) : (upperPoint = newConvexHull[1]);

    //if(isPointOnCHPerimeter(newPoint, oldConvexHull, replacedEdges)) return replacedEdges;

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
        if (CGAL::collinear(seg[0],polygonEdge[1],polygonEdge[0]))/*const Segment_2* s = boost::get<Segment_2>(&*result)*/ {
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

vector<Point_2> IncrementalAlg(vector<Point_2> pointSet, int edgeSelectionMethod, string initMethod) {
    vector<Point_2> sortedPointSet, usedPoints, convexHull, polygon;

    //cout << "Size of input file: " << pointSet.size() << endl;

    sortedPointSet = sortPointset(pointSet, initMethod);

    //cout << "Size of sortedPointSet: " << sortedPointSet.size() << endl;

    //cout<<"Starting triangle is:\n";
    for(int i=0; i<3; i++) {
        polygon.push_back(sortedPointSet.front());
        //cout<<"["<<sortedPointSet.front().x()<<" ]["<<sortedPointSet.front().y()<<"]"<<endl;
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

/*--------------------------------------------------------------------------------------------------------------------*/

void printResults(vector<Point_2> points, int edgeSelection, string algorithm, string incrementalInit, unsigned long milliseconds){
//    cout<<"Polygonization"<<endl;
//    for(Point_2 point : points)
//        cout<<point.x()<<" "<<point.y()<<endl;
//
//    for(int i=0; i<(points.size()-1); i++)
//        cout<<points[i].x()<<" "<<points[i].y()<<" "<<points[i+1].x()<<" "<<points[i+1].y()<<endl;
//
//    int size = points.size();
//    cout<<points[size-1].x()<<" "<<points[size-1].y()<<" "<<points[0].x()<<" "<<points[0].y()<<endl;

//    if(algorithm=="incremental")
//        cout<<"Algorithm: incremental"<<endl;
//    else if(algorithm=="convex_hull")
//        cout<<"Algorithm: convex_hull"<<endl;

    Polygon_2 polygon = getSimplePolygonFromPoints(points);
    long double polygonArea = abs(polygon.area());
    long double convexHullArea = abs(getSimplePolygonFromPoints(createConvexHull(points)).area());
    cout<<"area: "<<polygonArea<<endl;
    cout<<"ratio: "<<(polygonArea/convexHullArea)<<endl;
    cout<<"construcution time:"<<milliseconds<<"ms"<<endl;
    cout<<"convex hull area: "<<convexHullArea<<endl;
}

int main(int argc, char* argv[]) {
    string inputFileName;
    string outputFileName;
    string algorithmName;
    string edgeSelection;
    string incrementalInit;
    for(int i=1 ; i < argc ; i+=2) {
        if (strcmp(argv[i],"-i")==0) {
            inputFileName = argv[i+1];
        }
        else if (strcmp(argv[i],"-o")==0) {
            outputFileName = argv[i+1];
        }
        else if (strcmp(argv[i],"-algorithm")==0) {
            algorithmName = argv[i+1];
        }
        else if (strcmp(argv[i],"-edge_selection")==0) {
            edgeSelection = argv[i+1];
        }
        else if (strcmp(argv[i],"-initialization")==0) {
            incrementalInit = argv[i+1];
        }
    }
    //printUserInput(inputFileName,outputFileName,algorithmName,edgeSelection,incrementalInit);
    vector<Point_2> test = ProcessInputFile(inputFileName);

    auto started = std::chrono::high_resolution_clock::now();
    vector<Point_2> result = IncrementalAlg(test, stoi(edgeSelection), incrementalInit);
    auto done = std::chrono::high_resolution_clock::now();

    cout<<"Testing file: "<<inputFileName<<"  Selection: "<<stoi(edgeSelection)<<" Initialization: "<< incrementalInit<<endl;
    printResults(result, stoi(edgeSelection), algorithmName, incrementalInit, std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count());
    cout<<endl<<endl;
    return 0;
}

// ./main -i ./tests/euro-night-0000010.instance -o outputFile.txt -algorithm incremental -edge_selection 1 -initialization 2b
