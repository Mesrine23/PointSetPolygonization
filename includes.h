#ifndef POINTSETPOLYGONIZATION_INCLUDES_H
#define POINTSETPOLYGONIZATION_INCLUDES_H

#include <string.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>
#include <fstream>
#include <random>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/intersections.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2      Point_2;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef K::Segment_2    Segment_2;
typedef CGAL::Convex_hull_traits_adapter_2<K,CGAL::Pointer_property_map<Point_2>::type > Convex_hull_traits_2;

using namespace std;

#endif
