#ifndef UNITCONVERSION_H
#define UNITCONVERSION_H
#endif

#include "Camera.h"
using namespace std;

class UnitConversion
{
    public:
        float getPixToCm(int Y_ICCS, const Camera& CAM);
        float getDistance(int Y_ICCS, const Camera& CAM);
        UnitConversion(){};
        ~UnitConversion(){};

};

float UnitConversion::getPixToCm(int Y_ICCS, const Camera& CAM){
    float Z, W;
    Z = getDistance(Y_ICCS,CAM);
    W = 2 * Z * tan(CAM.FOV_VH(1) / 2 * M_PI / 180);

    return W/CAM.RES_VH(1);
}

float UnitConversion::getDistance(int Y_ICCS, const Camera& CAM){
    float thetaY, ty;
    ty = CAM.MATRIX_EXTRINSIC.at<float>(1,3);
    thetaY = CAM.FOV_HORIZON * (Y_ICCS / CAM.RES_HORIZON);
    return ty * 100 / tan(thetaY * M_PI / 180);
}