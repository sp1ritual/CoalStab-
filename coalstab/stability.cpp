#include "mainheader.h"

class Stability {
public:
    Stability(double tension_val = 0, double resistance_val = 0) : tension(tension_val), resistance(resistance_val) {}

    double calculateStability(double tension_val, double resistance_val) {
        tension = tension_val;
        resistance = resistance_val;

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
