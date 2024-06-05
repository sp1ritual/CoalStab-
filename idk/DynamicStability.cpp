#include <iostream>
class DynamicStability {
public:
    // Constructor for initializing values
    DynamicStability(double cohesion, double area, double normalStress, double velocity)
        : resistance(cohesion), pressure(area), stressCoefficient(normalStress), velocity(velocity) {}

    double calculateStability() {
        double temporaryResistance = stressCoefficient * velocity;
        double stabilityFactor = temporaryResistance / pressure;
        return stabilityFactor;
    }

private:
    double resistance, pressure, stressCoefficient, velocity;
};

extern "C" DynamicStability* create(double cohesion, double area, double normalStress, double velocity) {
    return new DynamicStability(cohesion, area, normalStress, velocity);
}

extern "C" void destroy(DynamicStability* obj) {
    delete obj;
}

extern "C" double calculate(DynamicStability* obj) {
    return obj->calculateStability();
}
