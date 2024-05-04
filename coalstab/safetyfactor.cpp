#include "mainheader.h"
class safetyfactor {
public:

double Calsafetyfactor(double C_val,double F_val){
    C = C_val;
    F = F_val;
    double Fac = C/F;
    if (F != 0){
        return Fac;
    } else {
        return 0;
    }
}

private:
double C,F;
};
int main(){
    safetyfactor obj;
    double result = obj.Calsafetyfactor(43.77,97.5);
    std::cout<<"Equals: "<<result<<std::endl;
    return 0;
}