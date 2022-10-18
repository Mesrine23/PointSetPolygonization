#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>

#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>


//typedef K::Point_2      Point_2;


using namespace std;

vector<Point_2> ProcessInputFile(string file_name){
    ifstream in_file;
    in_file.open(file_name);
    string line;
    vector<Point_2> pointset;
    while(getline(in_file, line){
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
    return points;
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
            inputFileName = argv[i+1];
        }
        else if (strcmp(argv[i],"-algorithm")==0) {
            inputFileName = argv[i+1];
        }
        else if (strcmp(argv[i],"-edge_selection")==0) {
            saveUserInputData(argv[i+1],edgeSelection);
        }
        else if (strcmp(argv[i],"-initialization")==0) {
            saveUserInputData(argv[i+1],incrementalInit);
        }
    }
    return 0;

    /*
     * ./PointSetPolygonization –i inputFile.txt –ο outputFile.txt –algorithm incremental -edge_selection 1 -initialization 1a -onion_initialization 5
     */


}
