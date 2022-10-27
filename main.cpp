#include "includes.h"
#include "incremental.h"
#include "prints.h"

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

    auto started = std::chrono::high_resolution_clock::now();
    vector<Point_2> result = IncrementalAlg(test, stoi(edgeSelection), incrementalInit);
    auto done = std::chrono::high_resolution_clock::now();

    cout<<"Testing file: "<<inputFileName<<"  Selection: "<<stoi(edgeSelection)<<" Initialization: "<< incrementalInit<<endl;
    printResults(result, stoi(edgeSelection), algorithmName, incrementalInit, std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count());
    cout<<endl<<endl;
    return 0;
}

// ./main -i ./tests/euro-night-0000010.instance -o outputFile.txt -algorithm incremental -edge_selection 1 -initialization 2b
