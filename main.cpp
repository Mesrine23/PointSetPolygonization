#include "includes.h"
#include "incremental.h"
#include "convex_hull.h"
#include "prints.h"

vector<Point_2> ProcessInputFile(string file_name) {
    ifstream file(file_name);
    vector<Point_2> pointSet;
    int lineIterator, xCordinate, yCordinate;
    string garbageInfo;
    getline(file, garbageInfo);
    getline(file, garbageInfo);
    while (file >> lineIterator >> xCordinate >> yCordinate)
        pointSet.push_back(Point_2(xCordinate, yCordinate));
    return pointSet;
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
    std::ofstream out(outputFileName.c_str());
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    vector<Point_2> test = ProcessInputFile(inputFileName);
    cout << "Polygonization" << endl;
    for(int i=0 ; i<test.size() ; ++i) {
        cout << test[i].x() << " " << test[i].y() << endl;
    }
    vector<Point_2> result;
    auto started = std::chrono::high_resolution_clock::now();
    if (algorithmName=="incremental") {
        result = IncrementalAlg(test, stoi(edgeSelection), incrementalInit);
    } else if (algorithmName=="convex_hull") {
        result = ConvexHullAlg(test, stoi(edgeSelection));
    } else {
        cout << "Wrong algorithm name!" << endl;
        exit(0);
    }
    auto done = std::chrono::high_resolution_clock::now();

    printResults(result, stoi(edgeSelection), algorithmName, incrementalInit, std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count());
    return 0;
}