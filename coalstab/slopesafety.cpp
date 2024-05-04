#include "slopesafety.h"
#include "mainheader.h"
slopesafety::slopesafety(double c_val, double A_val, double sigma_val, double N_val, double y_val, double V_val){
    c = c_val;
    A = A_val;
    sigma = sigma_val;
    N = N_val;
    y = y_val;
    V = V_val;
}
double slopesafety::calculateResistance(){
    R = c * A * sigma * N;
    return R;
}
double slopesafety::calculateHorizontalForce(){
    W = y * V;
    return W;
}
double slopesafety::calculationresult(){
    if (W != 0){
        return R/W;
    }
    else {
        return 0;
    }
};
int main(){
    slopesafety obj(60.0, 90.0, 130.0, 240.0, 50.0, 60.0);
    obj.calculateResistance();
    obj.calculateHorizontalForce();
    double ratio = obj.calculationresult();
    
    std::cout << "Resistance R: " << obj.R << std::endl;
    std::cout << "Horizontal Force W: " << obj.W << std::endl;
    std::cout << "Ratio R/W: " << ratio << std::endl;

    return 0;
}