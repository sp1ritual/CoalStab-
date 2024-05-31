#include <iostream>
#include <cmath>

class SlopeSafety {
public:
    SlopeSafety(double cohesion, double area, double stress, double normalStress, double unitWeight, double volume);

    double calculateResistance();
    double calculateHorizontalForce();
    double calculateSafetyFactor();

private:
    double cohesion, area, stress, normalStress, unitWeight, volume;
    double resistance, horizontalForce;
};

SlopeSafety::SlopeSafety(double cohesion, double area, double stress, double normalStress, double unitWeight, double volume) {
    this->cohesion = cohesion;
    this->area = area;
    this->stress = stress;
    this->normalStress = normalStress;
    this->unitWeight = unitWeight;
    this->volume = volume;
    this->resistance = 0;
    this->horizontalForce = 0;
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
extern "C" {
    SlopeSafety* createSlopeSafety(double cohesion, double area, double stress, double normalStress, double unitWeight, double volume) {
        return new SlopeSafety(cohesion, area, stress, normalStress, unitWeight, volume);
    }

    void destroySlopeSafety(SlopeSafety* obj) {
        delete obj;
    }

    double calculateResistance(SlopeSafety* obj) {
        return obj->calculateResistance();
    }

    double calculateHorizontalForce(SlopeSafety* obj) {
        return obj->calculateHorizontalForce();
    }

    double calculateSafetyFactor(SlopeSafety* obj) {
        return obj->calculateSafetyFactor();
    }
}
