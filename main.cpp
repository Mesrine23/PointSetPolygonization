#include "includes.h"
#include "incremental.h"
#include "convex_hull.h"
#include "local_search.h"
#include "simulated_annealing.h"
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
    string initAlgorithm;
    string L;
    string threshold;
    string annealing;

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
        else if(strcmp(argv[i], "-L")==0){
            L = argv[i+1];
        }
        else if (strcmp(argv[i],"-annealing")==0) {
            annealing = argv[i+1];
        }
        else if(strcmp(argv[i], "-threshold")==0){
            threshold = argv[i+1];
        }
        else if (strcmp(argv[i],"-initAlgorithm")==0) {
            initAlgorithm = argv[i+1];
        }
        else if(strcmp(argv[i],"-min")==0 or strcmp(argv[i], "-max")==0){
            edgeSelection = argv[i];
            i-=1;
        }
    }

    int minmax;
    if(edgeSelection=="-min")
        minmax=1;
    else if(edgeSelection == "-max")
        minmax=2;
    else {
        cout<<"Wrong edgeSelection!"<<endl;
        exit(0);
    }

    std::ofstream out(outputFileName.c_str());
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    vector<Point_2> test = ProcessInputFile(inputFileName);

    cout << "Optimal Area Polygonization" << endl;
    for(int i=0 ; i<test.size() ; ++i) {
        cout << test[i].x() << " " << test[i].y() << endl;
    }

    auto started = std::chrono::high_resolution_clock::now();

    vector<Point_2> result;
    if (initAlgorithm=="incremental") {
        result = IncrementalAlg(test, 1, "1a");
    } else if (initAlgorithm=="convex_hull") {
        result = ConvexHullAlg(test, minmax==1 ? 2 : 3);
    } else {
        cout << "Wrong initialisation algorithm name!" << endl;
        exit(0);
    }

    vector<Point_2> newPolygon;
    if (algorithmName=="local_search") {
        newPolygon = LocalSearchAlg(result, stoi(L), stod(threshold), (minmax==1 ? false : true));
    } else if (algorithmName=="simulated_annealing") {
        if(annealing == "local")
            newPolygon = SimulatedAnnealing(result, stoi(L), minmax, 1);
        else if(annealing == "global")
            newPolygon = SimulatedAnnealing(result, stoi(L), minmax, 2);
        else if(annealing == "subdivision")
            newPolygon = SimulatedAnnealing(result, stoi(L), minmax, 3);
        else{
            cout<<"Wrong annealing parameter!"<<endl;
            exit(0);
        }
    }

    auto done = std::chrono::high_resolution_clock::now();

    printResults(result, minmax, algorithmName, std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count(), newPolygon);

    return 0;
}