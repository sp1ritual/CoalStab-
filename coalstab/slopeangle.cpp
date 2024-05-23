#include "mainheader.h"
#include <cmath>  // Для использования функции atan

class SlopeAngleCalculator {
public:
    double calculateSlopeAngle(double height, double length) {
        this->height = height;
        this->length = length;
        if (length != 0) {
            double angle = atan(height / length);
            return angle;
        } else {
            return 0;
        }
    }

private:
    double height, length;
};
