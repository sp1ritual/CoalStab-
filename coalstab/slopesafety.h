#ifndef SLOPESAFETY_H
#define SLOPESAFETY_H

class SlopeSafety {
public:
    // F = R/W
    // R = c * A * sigma * N
    // W = y * V
    double resistance, horizontalForce, cohesion, area, stress, normalStress, unitWeight, volume;

    SlopeSafety(double cohesion_val, double area_val, double stress_val, double normalStress_val, double unitWeight_val, double volume_val);

    double calculateResistance(); // R
    double calculateHorizontalForce(); // W
    double calculateSafetyFactor(); // R/W
};

#endif
