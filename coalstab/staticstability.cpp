#include "staticstability.h"
#include "mainheader.h"

StaticStability::StaticStability(double cohesion_val, double area_val, double normalStress_val, double unitWeight_val, double height_val) {
    cohesion = cohesion_val;
    area = area_val;
    normalStress = normalStress_val;
    unitWeight = unitWeight_val;
    height = height_val;
}

double StaticStability::calculateDelaminationResistance() {
    resistance = cohesion * unitWeight * pow(area, 2);
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
