#ifndef STATICSTABILITY_H
#define STATICSTABILITY_H

class StaticStability {
public:
    // F = R/F
    // R = c * A * N = c * y * A^2
    // N = y * A
    // F = H
    double resistance, horizontalForce, cohesion, area, normalStress, unitWeight, height;

    StaticStability(double cohesion_val, double area_val, double normalStress_val, double unitWeight_val, double height_val);

    double calculateDelaminationResistance(); // R
    double calculateHorizontalForce(); // F
    double calculateStabilityFactor(); // R/F
};

#endif
