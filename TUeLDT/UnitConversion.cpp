#include "UnitConversion.h"


float UnitConversion::getPixToCm(int Y_ICCS, const Camera& CAM){
    float Z, W;
    Z = getDistance(Y_ICCS,CAM);
    W = 2 * Z * tan(CAM.FOV_VH(1)/2 * M_PI / 180);
    return W/CAM.RES_VH(1);
}

float UnitConversion::getDistance(int Y_ICCS, const Camera& CAM){
    float ty;
    ty = CAM.MATRIX_EXTRINSIC.at<float>(1,3);

    int dy = CAM.RES_VH(0) * (CAM.FOV_HORIZON / CAM.FOV_VH(0) - 0.5) ; // Calculate pixels added/removed due to pitch angle.
    float x = (float)CAM.RES_HORIZON  / tan(((float)CAM.FOV_HORIZON + (float)CAM.PITCH_ANGLE)* M_PI / 180);
    float Z = ty * 100 / (((float)Y_ICCS - (float)dy) / x);
    return Z;
}