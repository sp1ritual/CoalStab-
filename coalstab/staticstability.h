#ifndef STATICSTABILITY_H
#define STATICSTABILITY_H
class staticstability{
public:
// F = R/F
// R = c * A * N = c * y * A^2
// N = y * A
// F = H
double R,F,c,A,N,y,H;
staticstability(double c_val, double A_val,double N_val,double y_val, double H_val);
double delaminationresistance();//R
double horizonforce();//F
double calculationresult();
};
#endif