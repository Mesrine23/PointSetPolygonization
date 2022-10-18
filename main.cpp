#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2      Point_2;

using namespace std;

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
    for(int i=0 ; i < test.size() ; ++i) {
        cout << to_string(test[i].x()) << endl;
        cout << to_string(test[i].y()) << endl;
    }
        

    return 0;
}
