#include "includes.h"
#include "incremental.h"
#include "convex_hull.h"
#include "local_search.h"
#include "simulated_annealing.h"
#include "prints.h"
#include "spatial_subdivision.h"
#include <experimental/filesystem>
#include <iomanip>

using namespace std;

struct instanceSet{
    vector<string> instances;
    int size;
};

struct resultHolder{
    vector<double> local_search_inc_max, local_search_inc_min, local_search_ch_min, local_search_ch_max;
    vector<double> local_step_inc_max, local_step_inc_min, local_step_ch_min, local_step_ch_max;
    vector<double> global_step_inc_max, global_step_inc_min, global_step_ch_min, global_step_ch_max;
    vector<double> subdivision_min, subdivision_max;
};

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

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

int getSizeOfInstance(string instance){
    if(!ends_with(instance, ".instance"))
        return -1;
    else{
        int pos = instance.find_last_of('-');
        if(pos!=string::npos)
            return stoi(instance.substr(pos+1, 7));
        else
            return -1;
    }
}

bool sortInstanceBySize(string instance1, string instance2){
    return getSizeOfInstance(instance1)<getSizeOfInstance(instance2);
}

vector<instanceSet> makeInstanceSets(vector<string> instances){
    vector<instanceSet> result;
    instanceSet temp;

    sort(instances.begin(),instances.end(), sortInstanceBySize);

    int old_size=-2;
    for(int i=0; i<instances.size(); i++){
        int size = getSizeOfInstance(instances[i]);
        if(size==-1) continue;
        if(old_size!=size){
            if(old_size!=-2)
                result.push_back(temp);

            old_size=size;
            temp.size=size;
            temp.instances.clear();
        }
        temp.instances.push_back(instances[i]);
    }

    result.push_back(temp);
    return result;
}

template<typename T> void printElement(T t, const int& width)
{
    cout << left << setw(width) << t;
}

void printSingleScore(double d, const int& width){
    cout << left <<" "<< setw(width) << fixed<< setprecision(4)<<d;
}

double getScore(vector<Point_2> points, int minmax, bool cutoff){
    if(cutoff){
        if(minmax==1)
            return 1.0;
        else
            return 0.0;
    }
    double convex_hull_area = abs(getSimplePolygonFromPoints(createConvexHull(points)).area());
    double polygon_area = abs(getSimplePolygonFromPoints(points).area());
    return polygon_area/convex_hull_area;
}

void printScores(vector<double>& minVec, vector<double>&  maxVec, bool flag){
    if(flag){
        printElement(" ---", 11);
        cout<<"|";
        printElement(" ---", 11);
        cout<<"|";
        printElement(" ---", 11);
        cout<<"|";
        printElement(" ---", 11);
        return;
    }

    double min_score=0.0, max_score=0.0, min_bound=0.0, max_bound=1.0;
    for(int i=0; i<minVec.size(); i++){
        min_score+=minVec[i];
        max_score+=maxVec[i];
        if(minVec[i]>min_bound) min_bound=minVec[i];
        if(maxVec[i]<max_bound) max_bound=maxVec[i];
    }
    min_score=min_score/minVec.size();
    max_score=max_score/maxVec.size();

    printSingleScore(min_score, 10);
    cout<<"|";
    printSingleScore(max_score, 10);
    cout<<"|";
    printSingleScore(min_bound, 10);
    cout<<"|";
    printSingleScore(max_bound, 10);
}

/*--------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char* argv[]){
    string inputDirectory;
    string outputFileName;

    for(int i=1; i<argc; i+=2){
        if(strcmp(argv[i], "-i")==0)
            inputDirectory = argv[i+1];

        if(strcmp(argv[i],"-o")==0)
            outputFileName = argv[i+1];
    }

    vector<string> instances;
    error_code ec;
    for(const auto & entry : experimental::filesystem::recursive_directory_iterator(inputDirectory)){
        if(experimental::filesystem::is_regular_file(entry.path(), ec))
             instances.push_back(entry.path().string());
    }

    std::ofstream out(outputFileName.c_str());
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    //vector<Point_2> test = ProcessInputFile(inputFileName);

    size_t scores = strlen(" min score | max score | min bound | max bound ");

    printElement(" ", 8);
    cout<<"||";
    printElement(" Local Search (INC) ", scores);
    cout<<"||";
    printElement(" Local Search (CH) ", scores);
    cout<<"||";
    printElement(" Local Step (INC) ", scores);
    cout<<"||";
    printElement(" Local Step (CH) ", scores);
    cout<<"||";
    printElement(" Global Step (INC) ", scores);
    cout<<"||";
    printElement(" Global Step (CH) ", scores);
    cout<<"||";
    printElement(" Subdivision ", scores);
    cout<<"||"<<endl;

    printElement("Size", 8);
    cout<<"||";
    for(int i=0; i<7; i++){
        cout<<" min score | max score | min bound | max bound ||";
    }
    cout<<endl;


    vector<instanceSet> orderedInstances = makeInstanceSets(instances);
    for(instanceSet & inst : orderedInstances){
        resultHolder resHolder;
        long time = 500*inst.size;
        for(string & file : inst.instances){
            cerr<<"FOR FILE: "<<file<<endl;
            vector<Point_2> points = ProcessInputFile(file);

            cerr<<"Calculating Incremental Min..."<<endl;
            auto started=chrono::high_resolution_clock::now();
            vector<Point_2> incrementalInitMin= IncrementalAlg(points, 2, "1a");
            auto done = chrono::high_resolution_clock::now();
            long incrementalMinInitTime = std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();

            cerr<<"Calculating Incremental Max..."<<endl;
            started=chrono::high_resolution_clock::now();
            vector<Point_2> incrementalInitMax= IncrementalAlg(points, 3, "1a");
            done = chrono::high_resolution_clock::now();
            long incrementalMaxInitTime = std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();

            cerr<<"Calculating Convex Hull Min..."<<endl;
            started=chrono::high_resolution_clock::now();
            vector<Point_2> convexHullInitMin= ConvexHullAlg(points, 2);
            done = chrono::high_resolution_clock::now();
            long convexHullMinInitTime = std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();

            cerr<<"Calculating Convex Hull Max..."<<endl;
            started=chrono::high_resolution_clock::now();
            vector<Point_2> convexHullInitMax= ConvexHullAlg(points, 3);
            done = chrono::high_resolution_clock::now();
            long convexHullMaxInitTime = std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();

            vector<Point_2> result;

            cerr<<"Calculating Local Search Incremental Min..."<<endl;
            long time_till_cutoff=time-incrementalMinInitTime;
            result = LocalSearchAlg(incrementalInitMin, 4, 0.05, false, time_till_cutoff);
            resHolder.local_search_inc_min.push_back(getScore(result, 1, (time_till_cutoff==-1)));
            result.clear();

            cerr<<"Calculating Local Search Incremental Max..."<<endl;
            time_till_cutoff=time-incrementalMaxInitTime;
            result = LocalSearchAlg(incrementalInitMax, 4, 0.05, true, time_till_cutoff);
            resHolder.local_search_inc_max.push_back(getScore(result, 2, (time_till_cutoff==-1)));
            result.clear();

            cerr<<"Calculating Local Search Convex Hull Min..."<<endl;
            time_till_cutoff=time-convexHullMinInitTime;
            result = LocalSearchAlg(convexHullInitMin, 4, 0.05, false, time_till_cutoff);
            resHolder.local_search_ch_min.push_back(getScore(result, 1, (time_till_cutoff==-1)));
            result.clear();

            cerr<<"Calculating Local Search Convex Hull Max..."<<endl;
            time_till_cutoff=time-convexHullMaxInitTime;
            result = LocalSearchAlg(convexHullInitMax, 4, 0.05, true, time_till_cutoff);
            resHolder.local_search_ch_max.push_back(getScore(result, 2, (time_till_cutoff==-1)));
            result.clear();


            cerr<<"Calculating Local Step Incremental Min..."<<endl;
            time_till_cutoff=time-incrementalMinInitTime;
            result = SimulatedAnnealing(incrementalInitMin, 1500, 1, 1, time_till_cutoff);
            resHolder.local_step_inc_min.push_back(getScore(result, 1, (time_till_cutoff==-1)));
            result.clear();

            cerr<<"Calculating Local Step Incremental Max..."<<endl;
            time_till_cutoff=time-incrementalMaxInitTime;
            result = SimulatedAnnealing(incrementalInitMax, 1500, 2, 1, time_till_cutoff);
            resHolder.local_step_inc_max.push_back(getScore(result, 2, (time_till_cutoff==-1)));
            result.clear();

            cerr<<"Calculating Local Step Convex Hull Min..."<<endl;
            time_till_cutoff=time-convexHullMinInitTime;
            result = SimulatedAnnealing(convexHullInitMin, 1500, 1, 1, time_till_cutoff);
            resHolder.local_step_ch_min.push_back(getScore(result, 1, (time_till_cutoff==-1)));
            result.clear();

            cerr<<"Calculating Local Step Convex Hull Max..."<<endl;
            time_till_cutoff=time-convexHullMaxInitTime;
            result = SimulatedAnnealing(convexHullInitMax, 1500, 2, 1, time_till_cutoff);
            resHolder.local_step_ch_max.push_back(getScore(result, 2, (time_till_cutoff==-1)));
            result.clear();



            cerr<<"Calculating Global Step Incremental Min..."<<endl;
            time_till_cutoff=time-incrementalMinInitTime;
            result = SimulatedAnnealing(incrementalInitMin, 1500, 1, 2, time_till_cutoff);
            resHolder.global_step_inc_min.push_back(getScore(result, 1, (time_till_cutoff==-1)));
            result.clear();

            cerr<<"Calculating Global Step Incremental Max..."<<endl;
            time_till_cutoff=time-incrementalMaxInitTime;
            result = SimulatedAnnealing(incrementalInitMax, 1500, 2, 2, time_till_cutoff);
            resHolder.global_step_inc_max.push_back(getScore(result, 2, (time_till_cutoff==-1)));
            result.clear();

            cerr<<"Calculating Global Step Convex Hull Min..."<<endl;
            time_till_cutoff=time-convexHullMinInitTime;
            result = SimulatedAnnealing(convexHullInitMin, 1500, 1, 2, time_till_cutoff);
            resHolder.global_step_ch_min.push_back(getScore(result, 1, (time_till_cutoff==-1)));
            result.clear();

            cerr<<"Calculating Global Step Convex Hull Max..."<<endl;
            time_till_cutoff=time-convexHullMaxInitTime;
            result = SimulatedAnnealing(convexHullInitMax, 1500, 2, 2, time_till_cutoff);
            resHolder.global_step_ch_max.push_back(getScore(result, 2, (time_till_cutoff==-1)));
            result.clear();

            if(inst.size>=1000) {
                cerr << "Calculating Subdivision Min" << endl;
                time_till_cutoff = time;
                result = spatialSubdivision(points, 20, 1500, 1, 1, time_till_cutoff);
                resHolder.subdivision_min.push_back(getScore(result, 1, (time_till_cutoff == -1)));
                result.clear();

                cerr << "Calculating Subdivision Max" << endl;
                time_till_cutoff = time;
                result = spatialSubdivision(points, 20, 1500, 1, 1, time_till_cutoff);
                resHolder.subdivision_max.push_back(getScore(result, 2, (time_till_cutoff == -1)));
                result.clear();
            }
            cerr<<endl<<endl;
        }
        printElement(inst.size, 8);
        cout<<"||";
        printScores(resHolder.local_search_inc_min, resHolder.local_search_inc_max, false);
        cout<<"||";
        printScores(resHolder.local_search_ch_min, resHolder.local_search_ch_max, false);
        cout<<"||";
        printScores(resHolder.local_step_inc_min, resHolder.local_step_inc_max, false);
        cout<<"||";
        printScores(resHolder.local_step_ch_min, resHolder.local_step_ch_max, false);
        cout<<"||";
        printScores(resHolder.global_step_inc_min, resHolder.global_step_inc_max, false);
        cout<<"||";
        printScores(resHolder.global_step_ch_min, resHolder.global_step_ch_max, false);
        cout<<"||";
        if(inst.size<1000)
            printScores(resHolder.subdivision_min, resHolder.subdivision_min, true);
        else
            printScores(resHolder.subdivision_min, resHolder.subdivision_min, false);
        cout<<"||"<<endl;
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
//int main(int argc, char* argv[]) {
//    string inputFileName;
//    string outputFileName;
//    string algorithmName;
//    string edgeSelection;
//    string initAlgorithm;
//    string L;
//    string threshold;
//    string annealing;
//
//    cerr<<"hello"<<endl;
//
//    for(int i=1 ; i < argc ; i+=2) {
//        if (strcmp(argv[i],"-i")==0) {
//            inputFileName = argv[i+1];
//        }
//        else if (strcmp(argv[i],"-o")==0) {
//            outputFileName = argv[i+1];
//        }
//        else if (strcmp(argv[i],"-algorithm")==0) {
//            algorithmName = argv[i+1];
//        }
//        else if(strcmp(argv[i], "-L")==0){
//            L = argv[i+1];
//        }
//        else if (strcmp(argv[i],"-annealing")==0) {
//            annealing = argv[i+1];
//        }
//        else if(strcmp(argv[i], "-threshold")==0){
//            threshold = argv[i+1];
//        }
//        else if (strcmp(argv[i],"-initAlgorithm")==0) {
//            initAlgorithm = argv[i+1];
//        }
//        else if(strcmp(argv[i],"-min")==0 or strcmp(argv[i], "-max")==0){
//            edgeSelection = argv[i];
//            i-=1;
//        }
//    }
//
//    cerr<<"hello2"<<endl;
//
//
//    int minmax;
//    if(edgeSelection=="-min")
//        minmax=1;
//    else if(edgeSelection == "-max")
//        minmax=2;
//    else {
//        cout<<"Wrong edgeSelection!"<<endl;
//        exit(0);
//    }
//
//    cerr<<"main 1"<<endl;
//
//    std::ofstream out(outputFileName.c_str());
//    std::streambuf *coutbuf = std::cout.rdbuf();
//    std::cout.rdbuf(out.rdbuf());
//    vector<Point_2> test = ProcessInputFile(inputFileName);
//
//    cerr<<"main 2"<<endl;
//
////    cout << "Optimal Area Polygonization" << endl;
////    for(int i=0 ; i<test.size() ; ++i) {
////        cout << test[i].x() << " " << test[i].y() << endl;
////    }
//
//    auto started = std::chrono::high_resolution_clock::now();
//
//    vector<Point_2> result;
//    if (initAlgorithm=="incremental") {
//        result = IncrementalAlg(test, minmax==1 ? 2 : 3, "1a");
//    } else if (initAlgorithm=="convex_hull") {
//        result = ConvexHullAlg(test, minmax==1 ? 2 : 3);
//    } else {
//        cout << "Wrong initialisation algorithm name!" << endl;
//        exit(0);
//    }
//
//    vector<Point_2> newPolygon;
//    if (algorithmName=="local_search") {
//        newPolygon = LocalSearchAlg(result, stoi(L), stod(threshold), (minmax==1 ? false : true));
//    } else if (algorithmName=="simulated_annealing") {
//        if(annealing == "local")
//            newPolygon = SimulatedAnnealing(result, stoi(L), minmax, 1);
//        else if(annealing == "global")
//            newPolygon = SimulatedAnnealing(result, stoi(L), minmax, 2);
//        else if(annealing == "subdivision")
//            if(initAlgorithm=="incremental")
//                newPolygon = spatialSubdivision(test, 15, stoi(L), minmax, 1);
//            else if(initAlgorithm=="convex_hull")
//                newPolygon = spatialSubdivision(test, 15, stoi(L), minmax, 2);
//            else{
//                cout<<"Wrong annealing parameter!"<<endl;
//                exit(0);
//            }
//    }
//
//    auto done = std::chrono::high_resolution_clock::now();
//
//    printResults(result, minmax, algorithmName, std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count(), newPolygon);
//
//    return 0;
//}