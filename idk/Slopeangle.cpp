#include <iostream>
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
extern "C"{
    SlopeAngleCalculator* createSlopeAngle(){
       return new SlopeAngleCalculator;
    }
    void destroySlopeAngle(SlopeAngleCalculator* obj){
        delete obj;
    }
    double calculateAngle(SlopeAngleCalculator* obj,double height, double length){
        return obj->calculateSlopeAngle(height,length);
    }

}