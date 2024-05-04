#include "mainheader.h"
class tensiondumps {
public:
    double calucaltion(double A_val,double F_val){
        A = A_val;
        F = F_val;
        double sigma = F/A;
        if (A != 0){
            return sigma;
        } else {
            return 0;
        }
    }

private:
double A,F;
};
int main(){
    tensiondumps obj;
    double result = obj.calucaltion(35.2,21.4);
    std::cout<<"Result: "<<result<<std::endl;
    
    return 0;
}