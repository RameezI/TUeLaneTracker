#ifndef UNITCONVERSION_H
#define UNITCONVERSION_H

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

#endif