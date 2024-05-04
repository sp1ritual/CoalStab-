#include "mainheader.h"
class stability{
public:
    double calculatiob(double T_val, double R_val){
        T = T_val;
        R = R_val;
        double F = T/R;
        if (R != 0){
        return F;
        }
        else {
            return 0;
        }
    }
private:
    double T,R;
};
int main(){
    stability obj;
    double F = obj.calculatiob(40.8,54.7);
    std::cout<<"Equals: "<<F<<std::endl;
    return 0;
}