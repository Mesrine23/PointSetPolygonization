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
vector<Segment_2> findChainOfEdges(Point_2, Point_2, vector<Point_2>);
bool isEdgeVisibleFromPoint(Point_2, Segment_2, vector<Segment_2>);
vector<Segment_2> calculateVisibleEdges(vector<Segment_2>, vector<Point_2>, Point_2, vector<Point_2>);
Segment_2 randomEdgeSelection(vector<Segment_2>);
Segment_2 minAreaEdgeSelection(vector<Segment_2>, Point_2, vector<Point_2>);
Segment_2 maxAreaEdgeSelection(vector<Segment_2>, Point_2, vector<Point_2>);
Segment_2 getPolygonEdgeToReplace(vector<Segment_2>, Point_2, vector<Point_2>, int);
vector<Point_2> insertPointToPolygonPointSet(Point_2, Segment_2, vector<Point_2>);
void IncrementalAlg(vector<Point_2>, int, string);
/*--------------------------------------------------------------------------------------------------------------------*/

void printPointSet(vector<Point_2> pointSet) {
    for(int i=0 ; i < pointSet.size() ; ++i) {
        cout << to_string(pointSet[i].x()) + " " + to_string(pointSet[i].y()) << endl;
    }
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
        exit(-1);
    }
    return polygon;
}

vector<Point_2> ProcessInputFile(string file_name) {
    ifstream file(file_name);
    vector<Point_2> pointSet;
    int lineIterator, xCordinate, yCordinate;
    while (file >> lineIterator >> xCordinate >> yCordinate)
        pointSet.push_back(Point_2(xCordinate, yCordinate));
    return pointSet;
}

bool comparePointXdesc(Point_2 A, Point_2 B) {
    return A.x()>B.x();
}
bool comparePointXasc(Point_2 A, Point_2 B) {
    return A.x()<B.x();
}
bool comparePointYdesc(Point_2 A, Point_2 B) {
    return A.y()>B.y();
}
bool comparePointYasc(Point_2 A, Point_2 B) {
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

vector<Segment_2> getRedEdges(vector<Point_2> oldConvexHull, vector<Point_2> newConvexHull, Point_2 newPoint){
    vector<Segment_2> replacedEdges;
    int positionOfNewPoint = findIndexOfPointInPointSet(newConvexHull,newPoint);
    Point_2 upperPoint, lowerPoint;

    (positionOfNewPoint == (newConvexHull.size()-1)) ? (lowerPoint = newConvexHull[0]) : (lowerPoint = newConvexHull[positionOfNewPoint+1]);
    (positionOfNewPoint == 0) ? (upperPoint = newConvexHull[newConvexHull.size()-1]) : (upperPoint = newConvexHull[1]);

    int positionOfUpperElement = findIndexOfPointInPointSet(oldConvexHull, upperPoint);

    bool isLowerElement = false;
    int index = positionOfUpperElement;
    while(!isLowerElement){
        Point_2 segmentPointA, segmentPointB;
        segmentPointA = oldConvexHull[index];
        (index == (oldConvexHull.size()-1)) ? (index = 0) : (++index);
        segmentPointB = oldConvexHull[index];
        replacedEdges.push_back(Segment_2(segmentPointA, segmentPointB));
        if (segmentPointB==lowerPoint)
            isLowerElement=true;
    }
    return replacedEdges;
}
vector<Segment_2> findChainOfEdges(Point_2 pointA, Point_2 pointB, vector<Point_2> polygon) {
    vector<Segment_2> chainedEdges;
    auto indexOfPointA = find(polygon.begin(), polygon.end(), pointA);
    if (indexOfPointA==polygon.end()) {
        printErrorPointNotFound(pointA, polygon);
    }
    auto indexOfPointB = find(polygon.begin(), polygon.end(), pointB);
    if (indexOfPointB==polygon.end()) {
        printErrorPointNotFound(pointB, polygon);
    }
    int indexA = indexOfPointA - polygon.begin();
    int indexB = indexOfPointB - polygon.begin();
    int startingIndex, endingIndex;
    if(indexA>indexB) {
        startingIndex = indexB;
        endingIndex = indexA;
    } else {
        startingIndex = indexA;
        endingIndex = indexB;
    }
    for(int i=startingIndex ; i<endingIndex ; ++i){
        chainedEdges.push_back(Segment_2(polygon[i],polygon[i+1]));
    }
    return chainedEdges;
}

bool isEdgeVisibleFromPoint(Point_2 point, Segment_2 edge, vector<Segment_2> polygon){
    Segment_2 edgeA(point, edge[0]), edgeB(point, edge[1]);
    for(Segment_2 polygonEdge: polygon) {
        if(do_intersect(edgeA,polygonEdge) || do_intersect(edgeB,polygonEdge)) return false;
    }
    return true;
}

vector<Segment_2> calculateVisibleEdges(vector<Segment_2> redEdges, vector<Point_2> polygonPointSet, Point_2 added_point, vector<Point_2> convexHull){
    vector<Segment_2> visibleEdges;
    vector<Segment_2> polygonEdges = getPolygonEdgesFromPoints(polygonPointSet);
    for(Segment_2 redEdge : redEdges){
        cout << "redEdgePointA: " << redEdge[0].x() << " " << redEdge[0].y() << endl;
        cout << "redEdgePointB: " << redEdge[1].x() << " " << redEdge[1].y() << endl;
        bool foundEdge=false;
        for(Segment_2 polygonEdge : polygonEdges){
            if(redEdge==polygonEdge) {
                visibleEdges.push_back(redEdge);
                foundEdge = true;
                break;
            }
        }
        if(!foundEdge) {
            vector<Segment_2> chainedEdges = findChainOfEdges(redEdge[0],redEdge[1],polygonPointSet);
            for(Segment_2 chainedEdge: chainedEdges){
                if(isEdgeVisibleFromPoint(added_point,chainedEdge,polygonEdges))
                    visibleEdges.push_back(chainedEdge);
            }
        }
    }
    return visibleEdges;
}

Segment_2 randomEdgeSelection(vector<Segment_2> replaceableEdges) {
    std::random_device randomDevice;
    std::mt19937 gen(randomDevice());
    std::uniform_int_distribution<> distr(0, replaceableEdges.size()-1);
    return replaceableEdges[distr(gen)];
}

Segment_2 minAreaEdgeSelection(vector<Segment_2> replaceableEdges, Point_2 currentPoint, vector<Point_2> polygonPointSet) {
    Segment_2 edgeToReturn = replaceableEdges[0];
    vector<Point_2> currentPolygonPointSet = insertPointToPolygonPointSet(currentPoint,edgeToReturn,polygonPointSet);
    double minArea = getSimplePolygonFromPoints(currentPolygonPointSet).area();
    for(int i=1 ; i<replaceableEdges.size() ; ++i) {
        Segment_2 tempEdgeToReturn = replaceableEdges[i];
        vector<Point_2> tempPolygonPointSet = insertPointToPolygonPointSet(currentPoint,tempEdgeToReturn,polygonPointSet);
        double tempMinArea = getSimplePolygonFromPoints(currentPolygonPointSet).area();
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
    double maxArea = getSimplePolygonFromPoints(currentPolygonPointSet).area();
    for(int i=1 ; i<replaceableEdges.size() ; ++i) {
        Segment_2 tempEdgeToReturn = replaceableEdges[i];
        vector<Point_2> tempPolygonPointSet = insertPointToPolygonPointSet(currentPoint,tempEdgeToReturn,polygonPointSet);
        double tempMaxArea = getSimplePolygonFromPoints(currentPolygonPointSet).area();
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
    int startingIndex;
    (index1 > index2) ? (startingIndex = index2) : (startingIndex = index1);
    auto insertPosition = polygon.begin() + startingIndex+1;
    polygon.insert(insertPosition,point);
    return polygon;
}

void IncrementalAlg(vector<Point_2> pointSet, int edgeSelectionMethod, string initMethod) {
    vector<Point_2> sortedPointSet, usedPoints, convexHull, polygon;

    sortedPointSet = sortPointset(pointSet, initMethod);

    for(int i=0; i<3; i++) {
        polygon.push_back(sortedPointSet.front());
        usedPoints.push_back(sortedPointSet.front());
        sortedPointSet.erase(sortedPointSet.begin());
    }

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
    printPointSet(polygon);
    Polygon_2 polygon2 = getSimplePolygonFromPoints(polygon);
    cout << "Polygon is " << (polygon2.is_simple() ? "" : "not") << " simple." << endl;
}

/*--------------------------------------------------------------------------------------------------------------------*/

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
    IncrementalAlg(test, stoi(edgeSelection), incrementalInit);

    return 0;
}

// ./main -i test.txt -o outputFile.txt -algorithm incremental -edge_selection 1 -initialization 2b
