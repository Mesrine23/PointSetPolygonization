#include "includes.h"
#include "incremental.h"
#include "convex_hull.h"
#include "prints.h"
#include "simulated_annealing.h"

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
    string initializationAlgorithm;
    string annealing;
    string threshhold;
    string L;

    for(int i=1 ; i < argc ; i+=2) {
        if (strcmp(argv[i],"-i")==0) {
            inputFileName = argv[i+1];
        }
        else if (strcmp(argv[i],"-o")==0) {
            outputFileName = argv[i+1];
        }
        else if(strcmp(argv[i], "-initializationAlgorithm")==0){
            initializationAlgorithm == argv[i+1];
        }
        else if (strcmp(argv[i],"-algorithm")==0) {
            algorithmName = argv[i+1];
        }
        else if (strcmp(argv[i],"-annealing")==0) {
            annealing = argv[i+1];
        }
        else if(strcmp(argv[i], "-threshold")==0){
            threshhold = argv[i+1];
        }
        else if(strcmp(argv[i], "-L")==0){
            L = argv[i+1];
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
    cout << "Polygonization" << endl;
    for(int i=0 ; i<test.size() ; ++i) {
        cout << test[i].x() << " " << test[i].y() << endl;
    }
    vector<Point_2> result;
    auto started = std::chrono::high_resolution_clock::now();
    if (algorithmName=="local_search") {
        //algorithmos palamida
    } else if (algorithmName=="simulated_annealing") {
        if(initializationAlgorithm == "incremental"){
            if(annealing == "local")
                result = simulatedAnnealing(test, stoi(L), minmax, 1, 1);
            else if(annealing == "global")
                result = simulatedAnnealing(test, stoi(L), minmax, 2, 1);
            else if(annealing == "subdivision")
                result = simulatedAnnealing(test, stoi(L), minmax, 3, 1);
            else{
                cout<<"Wrong annealing parameter!"<<endl;
                exit(0);
            }
        }
        else if(initializationAlgorithm == "convex_hull"){
            if(annealing == "local")
                result = simulatedAnnealing(test, stoi(L), minmax, 1, 2);
            else if(annealing == "global")
                result = simulatedAnnealing(test, stoi(L), minmax, 2, 2);
            else if(annealing == "subdivision")
                result = simulatedAnnealing(test, stoi(L), minmax, 3, 2);
            else{
                cout<<"Wrong annealing parameter!"<<endl;
                exit(0);
            }
        }
        else{
            cout<<"Wrong initialization algorithm!"<<endl;
            exit(0);
        }
    } else {
        cout << "Wrong algorithm name!" << endl;
        exit(0);
    }
    auto done = std::chrono::high_resolution_clock::now();

    //printResults(result, stoi(edgeSelection), algorithmName, incrementalInit, std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count());
    return 0;
}