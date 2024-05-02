#ifndef SLOPESAFETY_H
#define SLOPESAFETY_H
class slopesafety{
public:
// F = R/W
// R = c * A * sigma * N
// W = y * V
double R,W,c,A,sigma,N,y,V;
 slopesafety(double c_val, double A_val, double sigma_val, double N_val, double y_val, double V_val);
  double calculateResistance(); //R
  double calculateHorizontalForce(); //W
  double calculationresult(); // R/W
};
#endif