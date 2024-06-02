#include <iostream>
#include <cmath>

class StaticStability {
public:
    StaticStability(double cohesion_val, double area_val, double normalStress_val, double unitWeight_val, double height_val);

    double calculateDelaminationResistance();
    double calculateHorizontalForce();
    double calculateStabilityFactor();

private:
    double cohesion, area, normalStress, unitWeight, height;
    double resistance, horizontalForce;
};

StaticStability::StaticStability(double cohesion_val, double area_val, double normalStress_val, double unitWeight_val, double height_val) {
    cohesion = cohesion_val;
    area = area_val;
    normalStress = normalStress_val;
    unitWeight = unitWeight_val;
    height = height_val;
    resistance = 0;
    horizontalForce = 0;
}

double StaticStability::calculateDelaminationResistance() {
    resistance = cohesion * unitWeight * std::pow(area, 2);
    return resistance;
}

double StaticStability::calculateHorizontalForce() {
    horizontalForce = height;
    return horizontalForce;
}

double StaticStability::calculateStabilityFactor() {
    if (horizontalForce != 0) {
        return resistance / horizontalForce;
    } else {
        return 0;
    }
}

extern "C" {
    StaticStability* createStaticStability(double cohesion, double area, double normalStress, double unitWeight, double height) {
        return new StaticStability(cohesion, area, normalStress, unitWeight, height);
    }

    void destroyStaticStability(StaticStability* obj) {
        delete obj;
    }

    double calculateDelaminationResistance(StaticStability* obj) {
        return obj->calculateDelaminationResistance();
    }

    double calculateHorizontalForce(StaticStability* obj) {
        return obj->calculateHorizontalForce();
    }

    double calculateStabilityFactor(StaticStability* obj) {
        return obj->calculateStabilityFactor();
    }
}
