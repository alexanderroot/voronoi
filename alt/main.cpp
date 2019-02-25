#include <iostream>
#include <fstream>
#include "DataTypes.h"
#include "Utils.h"

using namespace std;



void read_data(string filename) {
    fstream myfile(filename);
    double a, b;

    while (myfile >> a >> b)
    {
        points.push(Point(a, b));
        cout << a << endl;
        cout << b << endl;
    }
}


// void print_output() 
// {
//     std::cout << "Bounding box:" << std::endl;
//     std::cout << "Domain: " << X_BEGINS << ", " << X_ENDS << std::endl;
//     std::cout << "Range: " << Y_BEGINS << ", " << Y_ENDS << std::endl;

//     std::vector<Segment*>::iterator i;
//     Point start, end;
//     for (i = output.begin(); i != output.end(); i++) {
//         start = (*i)->s_start;
//         end = (*i)->s_end;
//         std::cout << "Segment from (" << start.xcor << ", " << start.ycor << ") to (" << end.xcor << ", " << end.ycor << ")" << std::endl;
//     }
// }

int main(int argc, char* argv[])
{
    if (argc != 3) {
        cout << "Intended use case:" << endl;
        cout << argv[0] << "[points_fn].txt [edges_fn].txt" << endl;
    }
    else {
        string points_fn = argv[1];
        string edges_fn = argv[2];
        read_data(points_fn);

        std::cout << "Original Bounding box:" << std::endl;
        std::cout << "Domain: " << X_BEGINS << ", " << X_ENDS << std::endl;
        std::cout << "Range: " << Y_BEGINS << ", " << Y_ENDS << std::endl;

        double dx = (X_ENDS - X_BEGINS + 1) / 5.0, dy = (Y_ENDS - Y_BEGINS+1) / 5.0;
        X_ENDS += dx;
        X_BEGINS -= dx;
        Y_ENDS += dy;
        Y_BEGINS -= dy;

        while(!points.empty()) {
            if (!events.empty() && events.top()->e_xcor <= points.top().xcor) {
                process_event();
            }
            else {
                process_point();
            }
        }

        while (!events.empty()) {
            process_event();
        }

        finish_edges();
        print_to_file(edges_fn);
    }

}



