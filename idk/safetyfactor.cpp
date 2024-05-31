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

extern "C" {
    SafetyFactor* createSafetyFactor() {
        return new SafetyFactor();
    }

    void destroySafetyFactor(SafetyFactor* obj) {
        delete obj;
    }

    double calculateSafety(SafetyFactor* obj, double cohesion, double force) {
        return obj->calculateSafetyFactor(cohesion, force);
    }
}
