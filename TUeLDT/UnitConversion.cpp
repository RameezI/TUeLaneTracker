#include "UnitConversion.h"


float UnitConversion::getPixToCm(int Y_ICCS, const Camera& CAM){
    float Z, W;
    Z = getDistance(Y_ICCS,CAM);
    W = 2 * Z * tan(CAM.FOV_VH(1)/2 * M_PI / 180);
    //std::cout << CAM.FOV_VH(1) << "\t" << CAM.FOV_VH(0) << "\t";
    //std::cout<<"W: " << W << "\tZ: " << Z << "\t";
    return W/CAM.RES_VH(1);
}

float UnitConversion::getDistance(int Y_ICCS, const Camera& CAM){
    float thetaY, ty;
    ty = CAM.MATRIX_EXTRINSIC.at<float>(1,3);

    int dy = CAM.RES_VH(0) * (CAM.FOV_HORIZON / CAM.FOV_VH(0) - 0.5 ); // Calculate pixels added/removed due to pitch angle.
    //thetaY = CAM.FOV_HORIZON * ((Y_ICCS - CAM.RES_VH(0))/CAM.RES_HORIZON + 1);

    thetaY = CAM.FOV_HORIZON * (((float)Y_ICCS + (float)dy) / (float)CAM.RES_HORIZON);
    float W = ty * 100 / tan(thetaY * M_PI / 180);
    //std::cout << "\t" << CAM.FOV_HORIZON << "\t" << thetaY << "\t" << Y_ICCS << "\t" << W << "\t" << CAM.RES_VH(0) <<endl;
    //std::cout<< "theyaY: " << thetaY << "\tdy: " << dy << "\tRes_Horizon: "<< CAM.RES_HORIZON << endl;
    return W;
}