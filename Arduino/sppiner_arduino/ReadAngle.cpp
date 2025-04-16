#include "ReadAngle.h"

extern ReadAngle angle_var;
extern vector<int> mapping;
vector<int> howManyTimesEachAngleWasMeasured(360, 0);
extern MPU6050 mpu;


// returns true if new distance sample is available
void Wrapping_OffsetCorrection()
{

    
    angle_var.currentNormalizedAngleZ = fmod(angle_var.currentNormalizedAngleZ, 360.0);
    if (angle_var.currentNormalizedAngleZ < 0)
    {
        angle_var.currentNormalizedAngleZ += 360.0;
    }

    angle_var.currentNormalizedAngleZWithOffset = angle_var.currentNormalizedAngleZ - angle_var.currentAngleZOffset;
    angle_var.currentNormalizedAngleZWithOffset = fmod(angle_var.currentNormalizedAngleZWithOffset, 360.0);
    if (angle_var.currentNormalizedAngleZWithOffset < 0)
    {
        angle_var.currentNormalizedAngleZWithOffset += 360.0;
    }
  
}
