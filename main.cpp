#include <iostream>
#include <fstream>
#include "DataTypes.h"
#include "Utils.h"

using namespace std;



int main(int argc, char* argv[])
{
    if (argc != 4) {
        cout << "Intended use case:" << endl;
        cout << argv[0] << " [numb_points] [points_fn].txt [edges_fn].txt" << endl;
    }
    else {
        int numbPoints = atoi(argv[1]);
        assert(numbPoints > 0);
        string points_fn = argv[2];
        string edges_fn = argv[3];
        generate_rand_points(numbPoints, points_fn);

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



