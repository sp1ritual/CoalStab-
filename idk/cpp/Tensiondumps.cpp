#include <iostream>

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

extern "C" {
    TensionDumps* createTension() {
        return new TensionDumps();
    }

    void destroyTension(TensionDumps* obj) {
        delete obj;
    }

    double calculateTension(TensionDumps* obj, double area_val, double force_val) {
        return obj->calculateStress(area_val, force_val);
    }
}
