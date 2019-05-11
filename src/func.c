
#include "func.h"

float getLanePos(int lane) {
    switch(lane) {
        case LEFT_LANE:
            return LEFT_LANE_POS;
        case RIGHT_LANE:
            return RIGHT_LANE_POS;
        case MID_LANE:
            return MID_LANE_POS;
        default:
            printf("unkown lane position given to get land pos\n");
            break;
    }

    return 0;
}
