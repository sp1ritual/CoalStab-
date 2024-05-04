#include "mainheader.h"
class slopeangle {
public:
    double Calslopeangle(double h_val,double L_val){
        h = h_val;
        L = L_val;
        double O = atan(h/L);
        if (L !=0){
            return O;
        } else {
            return 0;
        }
    }

private:
double h,L;
};
int main(){
    slopeangle obj;
    double result = obj.Calslopeangle(30.0,45.5);
    std::cout<<"Equals : "<<result<<std::endl;
    return 0;
}