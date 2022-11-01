#ifndef POINTSETPOLYGONIZATION_PRINTS_H
#define POINTSETPOLYGONIZATION_PRINTS_H

#include "includes.h"
#include "cgalStructsFunctions.h"

void printPointSet(vector<Point_2>);
void printUserInput(string, string, string, string, string);
void printErrorPointNotFound(Point_2, vector<Point_2>);
void printUserInput(string, string, string, string, string);
void printResults(vector<Point_2>, int , string , string , unsigned long );
void printSegList(vector<Point_2> );

#endif
