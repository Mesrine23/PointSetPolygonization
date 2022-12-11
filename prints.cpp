#include "prints.h"

void printSegList(vector<Point_2> points){
    int size = points.size();
    cout<<"segList={";
    for(int i=0; i<(points.size()-1); i++){
        cout<<"segment[("<<int(points[i].x())<<","<<int(points[i].y())<<"),("<<int(points[i+1].x())<<","<<int(points[i+1].y())<<")],";
    }
    cout<<"segment[("<<int(points.back().x())<<","<<int(points.back().y())<<"),("<<int(points[0].x())<<","<<int(points[0].y())<<")]}";
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

void printResults(vector<Point_2> points, int minmax, string algorithm, unsigned long milliseconds, vector<Point_2> newPolygonPoints){
    for(int i=0 ; i<points.size() ; ++i) {
        int nextIndex = i==(points.size()-1) ? 0 : (i+1);
        cout << points[i].x() << " " << points[i].y() << " " << points[nextIndex].x() << " " << points[nextIndex].y() << endl;
    }
    cout << "Algorithm: " << algorithm << "_" << (minmax==1 ? "min" : "max") << endl;
    Polygon_2 polygon = getSimplePolygonFromPoints(points);
    Polygon_2 newPolygon = getSimplePolygonFromPoints(newPolygonPoints);
    long double polygonArea = abs(polygon.area());
    long double newPolygonArea = abs(newPolygon.area());
    long double convexHullArea = abs(getSimplePolygonFromPoints(createConvexHull(points)).area());
    cout << "area_initial: " << polygonArea << endl;
    cout << "area: " << newPolygonArea << endl;
    cout << "ratio_initial: " << (polygonArea/convexHullArea) << endl;
    cout << "ratio: " << (newPolygonArea/convexHullArea) << endl;
    cout<< "construcution time: " << milliseconds << endl;
//    if (newPolygonArea > polygonArea)
//        cout << endl << "ΜΕΓΑΛΥΤΕΡΟ" << endl << endl;
//    else if (newPolygonArea < polygonArea)
//        cout << endl << "ΜΙΚΡΟΤΕΡΟ" << endl << endl;
//    else
//        cout << endl << "ΙΣΟ" << endl << endl;
//    if (!isPolygonSimple(points)) {
//        cout << "Not simple!" << endl;
//        exit(-1);
//    }
}
