#ifndef UTILS_VOR_H
#define UTILS_VOR_H


#include "DataTypes.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

extern std::priority_queue<Point,  std::vector<Point>,  GT> points; // site events
extern std::priority_queue<Event*, std::vector<Event*>, GT> events; // circle events
extern double MAX_POINT;

void process_point();

void process_event();

void front_insert(Point p);

bool circle(Point a, Point b, Point c, double *x, Point *o);

void check_circle_event(Arc *a, double xcor);

bool intersect(Point p, Arc *a, Point *result = 0);

Point intersection(Point p1, Point p2, double len);

void finish_edges();

void print_output();

double eucDistance(Point p1, Point p2);

void print_to_file(std::string edges_file);

void read_data(std::string points_fn);

void generate_rand_points(int numbPoints, std::string points_fn);

#endif