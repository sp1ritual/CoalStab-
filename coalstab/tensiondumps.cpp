#include "mainheader.h"

class TensionDumps {
public:
    TensionDumps(double area_val = 0, double force_val = 0) : area(area_val), force(force_val) {}

    double calculateStress(double area_val, double force_val) {
        area = area_val;
        force = force_val;

        if (area != 0) {
            double stress = force / area;
            return stress;
        } else {
            return 0;  
        }
    }

private:
    double area, force;  
};
