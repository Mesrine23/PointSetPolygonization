#include "prints.h"

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

void printResults(vector<Point_2> points, int edgeSelection, string algorithm, string incrementalInit, unsigned long milliseconds){
    for(int i=0 ; i<points.size() ; ++i) {
        int nextIndex = i==(points.size()-1) ? 0 : (i+1);
        cout << points[i].x() << " " << points[i].y() << " " << points[nextIndex].x() << " " << points[nextIndex].y() << endl;
    }
    cout << "Algorithm: " << algorithm << "_edge_selection" << edgeSelection;
    incrementalInit.empty() ? (cout << endl) : (cout << "_initialization" << incrementalInit << endl);
    Polygon_2 polygon = getSimplePolygonFromPoints(points);
    long double polygonArea = abs(polygon.area());
    long double convexHullArea = abs(getSimplePolygonFromPoints(createConvexHull(points)).area());
    cout<< "area: " <<polygonArea << endl;
    cout<< "ratio: " << (polygonArea/convexHullArea) << endl;
    cout<< "construcution time: " << milliseconds <<endl;
}
