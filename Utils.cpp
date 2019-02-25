#include "Utils.h"


double X_BEGINS = 0.0;
double X_ENDS = 0.0;
double Y_BEGINS = 0.0;
double Y_ENDS = 0.0;

std::vector<Segment*> output;
std::priority_queue<Point,  std::vector<Point>,  GT> points; // site events
std::priority_queue<Event*, std::vector<Event*>, GT> events; // circle events
Arc *root = 0;
double MAX_VAL = 100;

void process_point()
{
   // Get the next point from the queue.
   Point p = points.top();
   points.pop();

   // Add a new arc to the parabolic front.
   front_insert(p);
}


void process_event() 
{
    Event *e = events.top();
    events.pop(); 

    if (e->valid) {
        //make a new edge starting at the point of the circle event
        Segment *s = new Segment(e->e_point);

        //Remove the associated arc from the front
        Arc *a = e->e_arc;

        //if there is a previous arc
        if (a->prev) {
            a->prev->next = a->next;
            a->prev->seg_end = s;
        }
        if (a->next) {
            a->next->prev = a->prev;
            a->next->seg_start = s;
        }

        //Finish the edges before and after a
        if (a->seg_start) {
            a->seg_start->finish(e->e_point);
        }
        if (a->seg_end) {
            a->seg_end->finish(e->e_point);
        }


        //recheck for circle events on either side of a
        if (a->prev) {
            check_circle_event(a->prev, e->e_xcor);
        }
        if (a->next) {
            check_circle_event(a->next, e->e_xcor);
        }
    }

    delete e;
}


void front_insert(Point p) 
{

    if (!root) {
        //if there is no front so far
        root = new Arc(p);
        return;
    }

    //go through the whole arc front
    for (Arc *a = root; a; a = a->next) {
        Point intersect_p, intersect_p2;

        if (intersect(p, a, &intersect_p)) {
            // New parabola intersects arc i.  If necessary, duplicate i.
            if (a->next && !intersect(p, a->next, &intersect_p2)) {
                //create a new arc and insert it 
                a->next->prev = new Arc(a->a_point, a, a->next);
                a->next = a->next->prev;
            }
            else {
                a->next = new Arc(a->a_point, a);
                //does this work?? a little confused here
            }
            a->next->seg_end = a->seg_end;


            //now we need to add the point p between a and a->next (and it's associated arc)
            a->next->prev = new Arc(p, a, a->next);
            a->next = a->next->prev;


            //move to the next arc (the one that includes p)
            a = a->next;

            //must create new half edges
            a->prev->seg_end = a->seg_end = new Segment(intersect_p);
            a->next->seg_start = a->seg_end = new Segment(intersect_p);

            //check for new circle events around the new arc
            check_circle_event(a, p.xcor);
            check_circle_event(a->prev, p.xcor);
            check_circle_event(a->next, p.xcor);

            return;
        }
    }

    //If there is never an intersect between the point and an arc
    Arc *a;

    //get the last part arc
    for(a = root; a->next; a=a->next) {
        continue;
    }

    a->next = new Arc(p, a);

    //create segment between p and a
    Point start;
    start.xcor = X_BEGINS;
    start.ycor = (p.ycor + a->a_point.ycor) / 2;

    a->seg_end = a->next->seg_start = new Segment(start);
}


void check_circle_event(Arc *a, double xcor) 
{

    //if event is old, invalidate it
    if (a->a_event && a->a_event->e_xcor != xcor) {
        a->a_event->valid = false;
    }
    a->a_event = NULL;

    if (!a->prev || !a->next) {
        //if arc has been removed?
        return;
    }

    double circle_r;
    Point circle_c;

    //if cri
    if (circle(a->prev->a_point, a->a_point, a->next->a_point, &circle_r, &circle_c) && circle_r > xcor) {
        //must create a new event
        a->a_event = new Event(circle_r, circle_c, a);
        events.push(a->a_event);
    }

}


double eucDistance(Point p1, Point p2) {
    double dist = pow(pow(p1.xcor - p2.xcor, 2.0) + pow(p1.ycor - p2.ycor, 2.0), 0.5);
    return dist;
}

//extract radius and center from a circle
bool circle(Point a, Point b, Point c, double *radius, Point *center) 
{
    // Check that bc is a "right turn" from ab.
    if ((b.xcor - a.xcor) * (c.ycor - a.ycor) - (c.xcor - a.xcor) * (b.ycor - a.ycor) > 0) {
        return false;
    }

    double bax_dif = b.xcor - a.xcor,  bay_dif = b.ycor - a.ycor,
        cax_dif = c.xcor - a.xcor, cay_dif = c.ycor - a.ycor,
        bax_sq = bax_dif * (a.xcor+b.xcor) + bay_dif * (a.ycor+b.ycor),
        cax_sq = cax_dif * (a.xcor+c.xcor) + cay_dif * (a.ycor+c.ycor),
        bottom = 2 * (bax_dif * (c.ycor-b.ycor) - bay_dif * (c.xcor-b.xcor));

    if (bottom == 0) {
        return false;
    }

    center->xcor = (cay_dif * bax_sq - bay_dif * cax_sq) / bottom;
    center->ycor = (bax_dif * cax_sq - cax_dif * bax_sq) / bottom;

    *radius = eucDistance(*center, a) ;
    return true;
}

bool intersect(Point p, Arc *a, Point *result) 
{
    if (a->a_point.xcor == p.xcor) {
        return false;
    }

    double first = 0.0, second = 0.0;

    if (a->prev) {
        first = intersection(a->prev->a_point, a->a_point, p.xcor).ycor;
    }
    if (a->next) {
        second = intersection(a->a_point, a->next->a_point, p.xcor).ycor;
    }

    if ( (!a->prev || first <= p.ycor) && (!a->next || p.ycor <= second) ) {
        result->ycor = p.ycor;
        result->xcor = ( pow(a->a_point.xcor, 2.0) + pow((a->a_point.ycor - result->ycor), 2.0) - pow(p.xcor, 2.0)) / (2*a->a_point.xcor - 2*p.xcor);
        return true;
    }
    return false;
}


//find the point of intersection for two parabolas
Point intersection(Point p1, Point p2, double len) 
{
    Point result, temp = p1;

    if (p1.xcor == p2.xcor) {
        result.ycor = (p1.ycor + p2.ycor) / 2.0;
    }
    else if (p2.xcor == len) {
        result.ycor = p2.ycor;
    }
    else if (p1.xcor == len) {
        result.ycor = p1.ycor;
        temp = p2;
    } 
    else {
        //quadratic formula

        double x1 = 2 * (p1.xcor - len);
        double x2 = 2 * (p2.xcor - len);

        double a_diff = 1/x1 - 1/x2;
        double b_diff = -2 * (p1.ycor / x1 - p2.ycor / x2);
        double c_diff = (pow(p1.ycor, 2.0) + pow(p1.xcor, 2.0) - pow(len, 2.0))/x1 - 
        (pow(p2.ycor, 2.0) + pow(p2.xcor, 2.0) - pow(len, 2.0))/x2;

        result.ycor = (-b_diff - sqrt( pow(b_diff, 2.0) - 4*a_diff*c_diff) ) / (2 * a_diff);
    }

    result.xcor = (pow(temp.xcor, 2.0) + pow(temp.ycor-result.ycor, 2.0) - pow(len, 2.0)) / (2 * (temp.xcor - len) );
    return result;
}


void finish_edges()
{
    // Advance the sweep line so no parabolas can cross the bounding box.
    double len = X_ENDS + (X_ENDS-X_BEGINS) / 2.0 + (Y_ENDS-Y_BEGINS) / 2.0;

    std::cout << "Accounted for length " << len << std::endl; 
    // Extend each remaining segment to the new parabola intersections.
    for (Arc *a = root; a->next; a = a->next) {
        if (a->seg_end) {
            a->seg_end->finish(intersection(a->a_point, a->next->a_point, 2*len));
        }
    }
}


void print_to_file(std::string edges_file)
{

    std::ofstream efile;
    efile.open(edges_file);
    std::cout << "Bounding box:" << std::endl;
    std::cout << "Domain: " << X_BEGINS << ", " << X_ENDS << std::endl;
    std::cout << "Range: " << Y_BEGINS << ", " << Y_ENDS << std::endl;

    std::vector<Segment*>::iterator i;
    Point start, end;
    for (i = output.begin(); i != output.end(); i++) {
        start = (*i)->s_start;
        end = (*i)->s_end;
        efile << start.xcor << "," << start.ycor << " " << end.xcor << "," << end.ycor << std::endl;
        std::cout << "Segment from (" << start.xcor << ", " << start.ycor << ") to (" << end.xcor << ", " << end.ycor << ")" << std::endl;
    }

    efile.close();
}


void read_data(std::string points_fn) {
    std::ifstream pfile(points_fn);

    double a, b;

    while (pfile >> a >> b)
    {
        points.push(Point(a, b));

        if (a < X_BEGINS) {
            X_BEGINS = a;
        }
        if (a > X_ENDS) {
            X_ENDS = a;
        }
        if (b < Y_BEGINS) {
            Y_BEGINS = b;
        }
        if (a > Y_ENDS) {
            Y_ENDS = b;
        }
        // cout << a << endl;
        // cout << b << endl;
    }
    pfile.close();
}


void generate_rand_points(int numbPoints, std::string points_fn) {
    
    assert(numbPoints > 0);

    std::ofstream pfile;
    pfile.open(points_fn);

    double a, b;

    for (int i = 0; i < numbPoints; i++) {
        a = MAX_VAL * (double)rand()/(double)RAND_MAX;
        b = MAX_VAL * (double)rand()/(double)RAND_MAX;
        points.push(Point(a, b));
        pfile << a << " " << b << std::endl;
        if (a < X_BEGINS) {
            X_BEGINS = a;
        }
        if (a > X_ENDS) {
            X_ENDS = a;
        }
        if (b < Y_BEGINS) {
            Y_BEGINS = b;
        }
        if (a > Y_ENDS) {
            Y_ENDS = b;
        }
    }
    pfile.close();
}
