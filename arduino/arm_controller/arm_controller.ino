#include <cmath>

//Defining Arm Values
int lengthAB = 19, lengthBC = 18;
int minAlpha = 0, minBeta = 35, minGamma = 32;
int maxAlpha = 270, maxBeta = 100, maxGamma = 127;

//Clamping Function
double clamp(double val, double minVal, double maxVal) {
    return fmax(minVal, fmin(maxVal, val));
}

//Proccess D2 Functions generating angles from postion (copied from simulations.py)
//Generating Alpha angle
double generateAlphaAngle(int x = 0, int y = 0) {
    try {

        double alphaRadians = atan2(y, x);
        double alpha = alphaRadians * (180.0 / M_PI);

        if(alpha < minAlpha && alpha > maxAlpha) {
            throw 205;
        }

        return alpha;
    }
    catch (int errorCode) {
        return NAN;
    }
}

//Generating Beta Angle
double generateBetaAngle(int x = 0, int y = 0, int z = 0) {
    try {
        double r = sqrt(x * x + y * y);
        double rho = sqrt(x * x + y * y + z * z);

        double cosPhi = (pow(lengthAB, 2) + pow(rho, 2) - pow(lengthBC, 2)) / (2 * lengthAB * rho);
        cosPhi = clamp(cosPhi, -1, 1);
        double phi = acos(cosPhi);
        double omega = atan2(z, r);

        double betaRadians = phi + omega;

        double beta = betaRadians * (180.0 / M_PI);

        if(beta < minBeta && beta > maxBeta) {
            throw 205;
        }

        return beta;
    }
    catch (int errorCode) {
        return NAN;
    }
}

//Generate Gamma angle
double generateAngle(int x = 0, int y = 0, int z = 0) {
    try {
        double rho = sqrt(x * x + y * y + z * z);

        double cosGamma = (pow(lengthAB, 2) + pow(lengthBC, 2) - pow(rho, 2)) / (2 * lengthAB * lengthBC);
        cosGamma = clamp(cosGamma, -1, 1);
        double gammaRadians = acos(cosGamma);

        double gamma = gammaRadians * (180.0 / M_PI);

        if(gamma < minGamma && gamma > maxGamma) {
            throw 205;
        }

        return gamma;
    }
    catch (int errorCode) {
        return NAN;
    }
}

void setup() {
    //Setup Function
    //Get Starting 
}

void loo() {
    //Main function
}