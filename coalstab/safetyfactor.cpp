#include "mainheader.h"
#include <iostream>

class SafetyFactor {
public:

    double calculateSafetyFactor(double cohesion, double force) {
        this->cohesion = cohesion;
        this->force = force;
        double safetyFactor = cohesion / force;
        if (force != 0) {
            return safetyFactor;
        } else {
            return 0;
        }
    }

private:
    double cohesion, force;
};