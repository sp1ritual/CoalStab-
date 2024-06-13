#include <iostream>

class Stability {
public:
    Stability(double tension_val = 0, double resistance_val = 0) 
        : tension(tension_val), resistance(resistance_val) {}

    double calculateStability(double tension_val, double resistance_val) {
        this->tension = tension_val;
        this->resistance = resistance_val;
        if (resistance != 0) {
            return tension / resistance;
        } else {
            return 0;
        }
    }

private:
    double tension, resistance;
};
extern "C" {
    Stability* createStability() {
        return new (std::nothrow) Stability();  
    }

    void destroyStability(Stability* obj) {
        delete obj;
    }

    double calcStability(Stability* obj, double tension_val, double resistance_val) {
        if (obj != nullptr) {  // Ensure object is not null
            return obj->calculateStability(tension_val, resistance_val);
        } else {
            return 0;
        }
    }
}
