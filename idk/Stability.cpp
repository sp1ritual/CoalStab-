#include <iostream>

class Stability {
public:
    Stability(double tension_val = 0, double resistance_val = 0) : tension(tension_val), resistance(resistance_val) {}

    double calculateStability(double tension_val, double resistance_val) {
        this->tension = tension_val;
        this->resistance = resistance_val;
        if (resistance != 0) {
            double stabilityFactor = tension / resistance;
            return stabilityFactor;
        } else {
            return 0;  
        }
    }

private:
    double tension, resistance;  
};
//Не показывает значения в  main.cpp, но тут все работает :/
int main(){
    Stability stability;
    double tension_val = 10.0;
    double resistance_val = 5.0;
    double result = stability.calculateStability(tension_val,resistance_val);
    std::cout<<"Result: "<<result<<std::endl;
    return 0;
}
extern "C" {
    Stability* createStability() {
        return new Stability();
    }

    void destroyStability(Stability* obj) {
        delete obj;
    }

    double calcStability(Stability* obj, double tension_val, double resistance_val) {
        return obj->calculateStability(tension_val, resistance_val);
    }
}
