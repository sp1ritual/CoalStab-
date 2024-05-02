#include "staticstability.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
staticstability::staticstability(double c_val, double A_val,double N_val,double y_val, double H_val){
    c = c_val;
    A = A_val;
    N = N_val;
    y = y_val;
    H = H_val;
}
double staticstability::delaminationresistance(){
    R = c * y * pow(A,2);
    return R;
};
double staticstability::horizonforce(){
    F = H;
    return F;
};
double staticstability::calculationresult(){
    if (F != 0){
        return R/F;
    }
    else {
        return 0;
    }
};
int main() {
    staticstability obj(40.0, 63.0, 54.0, 65.0, 77.0);
    obj.delaminationresistance();
    obj.horizonforce();
    double result = obj.calculationresult();
    std::cout << "Delamination Resistance: " << obj.R << std::endl;
    std::cout << "Horizon Force: " << obj.F << std::endl;
    std::cout << "Ratio R/F: " << result << std::endl;
    return 0;
}
