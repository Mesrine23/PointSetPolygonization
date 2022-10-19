#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>

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

void printPointSet(vector<Point_2> pointSet) {
    for(int i=0 ; i < pointSet.size() ; ++i) {
        cout << to_string(pointSet[i].x()) + " " + to_string(pointSet[i].y()) << endl;
    }
}

vector<Point_2> ProcessInputFile(string file_name){
    ifstream in_file;
    in_file.open(file_name);
    string line;
    vector<Point_2> pointset;
    while(getline(in_file, line)){
        int pnt_id, tmp_x, tmp_y;
        stringstream line_strm(line);
        string inter;

        getline(line_strm, inter, '\t');
        pnt_id = stoi(inter);

        getline(line_strm, inter, '\t');
        tmp_x = stoi(inter);

        getline(line_strm , inter ,'\t');
        tmp_y = stoi(inter);

        pointset.push_back(Point_2(tmp_x, tmp_y));
    }
    return pointset;
}

bool comparePointXdesc(Point_2 A, Point_2 B){
    return A.x()>B.x();
}
bool comparePointXasc(Point_2 A, Point_2 B){
    return A.x()<B.x();
}
bool comparePointYdesc(Point_2 A, Point_2 B){
    return A.y()>B.y();
}
bool comparePointYasc(Point_2 A, Point_2 B){
    return A.y()<B.y();
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

Polygon_2 ConvexHullPolygon(vector<Point_2> pointSet) {
    Polygon_2 convexHull;
    std::vector<std::size_t> indices(pointSet.size()), out;
    std::iota(indices.begin(), indices.end(),0);
    CGAL::convex_hull_2(indices.begin(), indices.end(), std::back_inserter(out),
                        Convex_hull_traits_2(CGAL::make_property_map(pointSet)));
    for( std::size_t i : out ) {
        convexHull.push_back(pointSet[i]);
        std::cout << "points[" << i << "] = " << pointSet[i] << std::endl;
    }
    return convexHull;
}

void IncrementalAlg(vector<Point_2> pointSet, int edgeSelection, string initMethod) {
    vector<Point_2> sortedPointSet, usedPoints;
    Polygon_2 polygon, convexHull;

    sortedPointSet = sortPointset(pointSet, initMethod);

    for(int i=0; i<3; i++) {
        polygon.push_back(sortedPointSet.front());
        usedPoints.push_back(sortedPointSet.front());
        sortedPointSet.erase(sortedPointSet.begin());
    }

    while(!sortedPointSet.empty()){
        Point_2 currentPoint = sortedPointSet.front();
        sortedPointSet.erase(sortedPointSet.begin());

    }
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
    cout << endl << endl;
    cout << "input file: " + inputFileName << endl;
    cout << "output file: " + outputFileName << endl;
    cout << "algorithm: " + algorithmName << endl;
    cout << "edge selection: " + edgeSelection << endl;
    cout << "incremental initialization: " + incrementalInit << endl;

    vector<Point_2> test = ProcessInputFile(inputFileName);
    ConvexHullPolygon(test);
    return 0;
    IncrementalAlg(test, stoi(edgeSelection), incrementalInit);

    return 0;
}

// ./to_polygon -i test.txt -o outputFile.txt -algorithm incremental -edge_selection 1 -initialization 2b
