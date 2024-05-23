#include "slopesafety.h"
#include "mainheader.h"

SlopeSafety::SlopeSafety(double cohesion, double area, double stress, double normalStress, double unitWeight, double volume) {
    this->cohesion = cohesion;
    this->area = area;
    this->stress = stress;
    this->normalStress = normalStress;
    this->unitWeight = unitWeight;
    this->volume = volume;
}

double SlopeSafety::calculateResistance() {
    resistance = cohesion * area * stress * normalStress;
    return resistance;
}

double SlopeSafety::calculateHorizontalForce() {
    horizontalForce = unitWeight * volume;
    return horizontalForce;
}

double SlopeSafety::calculateSafetyFactor() {
    if (horizontalForce != 0) {
        return resistance / horizontalForce;
    } else {
        return 0;
    }
}
