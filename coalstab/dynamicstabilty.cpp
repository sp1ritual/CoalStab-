#include "mainheader.h"
class dynamicstability {
    public: 
    //F = Rd/Pd
    //Rd = k * V
    double calculation(double Rd_val,double Pd_val,double k_val,double V_val){
        Rd = Rd_val;
        Pd = Pd_val;
        k = k_val;
        V = V_val;
        double Rd = k * V;
        double F = Rd/Pd;
        return F;
    }
    private:
    double Rd,Pd,k,V;
};
int main(){
    dynamicstability obj;
    double result = obj.calculation(40.0,34.6,76.3,14.89);
    std::cout<<"Result : "<<result<<std::endl;
    return 0;
}