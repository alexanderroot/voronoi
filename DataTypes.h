#ifndef TYPES_VOR_H
#define TYPES_VOR_H


#include <vector>
#include <queue>

struct Point;

struct Arc;

struct Segment;

struct Event;

struct GT;

//keep track of segments
extern std::vector<Segment*> output;

// Keep track of linked list of arcs
extern Arc *root;

// Bounding box coordinates.
extern double X_BEGINS, X_ENDS, Y_BEGINS, Y_ENDS;


struct Point {
    double xcor;
    double ycor;

    Point(double x, double y) 
        : xcor(x), ycor(y) {}
    
    Point() 
        : xcor(0), ycor(0) {}
};


struct Event {
    double e_xcor;
    Point e_point;
    Arc *e_arc;
    bool valid;

    Event(double x, Point p, Arc *a) 
        : e_xcor(x), e_point(p), e_arc(a), valid(true) {}

    // ~Event() {delete e_arc;}
};


struct Arc {
    Point a_point;
    Arc *prev, *next;
    Event *a_event;
    Segment *seg_start, *seg_end;

    Arc(Point p, Arc *a=0, Arc *b=0) 
        : a_point(p), prev(a), next(b), seg_start(0), seg_end(0), a_event(0) {}
    
    // ~Arc() {
    //     delete prev; 
    //     delete next;
    //     delete a_event;
    //     delete seg_start;
    //     delete seg_end;
    // }
};


struct Segment {
    Point s_start, s_end;
    bool done;


    Segment(Point p) 
        : s_start(p), s_end( Point(0, 0) ), done(false) 
        { output.push_back(this); }

    void finish(Point p) { if (done) return; s_end = p; done = true; }
};


//greater than comparison for priority queue
struct GT {
    //if the have the same x-coordinate, return True if a's y is greater, else return if a's x is greater
    bool operator()(Point a, Point b) {return a.xcor == b.xcor ? a.ycor > b.ycor : a.xcor > b.xcor;}
    
    // return True if a has a higher xcoordinate
    bool operator()(Event *a, Event *b) {return a->e_xcor > b->e_xcor;}
};

#endif