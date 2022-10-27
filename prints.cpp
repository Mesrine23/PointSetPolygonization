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
