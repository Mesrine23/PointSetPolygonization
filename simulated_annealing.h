#ifndef MAIN_SIMULATED_ANNEALING_H
#define MAIN_SIMULATED_ANNEALING_H

#include "includes.h"
#include "convex_hull.h"
#include "incremental.h"
#include "cgalStructsFunctions.h"
#include "prints.h"
#include <CGAL/Kd_tree.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/Search_traits_2.h>

typedef CGAL::Search_traits_2<K> Traits;
typedef CGAL::Kd_tree<Traits> Tree;
typedef CGAL::Fuzzy_iso_box<Traits> Fuzzy_iso_box;

struct StepResult{
    pair<Point_2, Point_2> local;
    vector<Point_2> global;
};

vector<Point_2> SimulatedAnnealing(vector <Point_2> , int , int , int );
StepResult localStep(vector<Point_2>);
StepResult globalStep(vector<Point_2>);
bool localTransitionValid(vector<Point_2>, StepResult );
bool globalTransitionValid(vector<Point_2>, StepResult );
void applyLocalTransition(vector<Point_2>& , StepResult );
void applyGlobalTransition(vector<Point_2>&, StepResult );
double calculateNewArea(vector<Point_2> pointset, double, StepResult, int);
double calculateEnergy(double, double, int, int);
bool metropolis(double, double);
#endif