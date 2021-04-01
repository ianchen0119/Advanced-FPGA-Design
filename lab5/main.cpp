#include "cordic.h"
#define iteration_num 7
THETA_TYPE cordic_phase[iteration_num] = {45.0, 26.565, 14.036, 7.125, 3.576, 1.790, 0.895};
void cordic(THETA_TYPE theta, COS_SIN_TYPE &s, COS_SIN_TYPE &c){
	COS_SIN_TYPE current_cos = 0.60735;
	COS_SIN_TYPE current_sin = 0.0;
	COS_SIN_TYPE factor = 1.0;
    int j = 0;
    for(; j<iteration_num; j++){
        int sigma = (theta>0)?1:-1;
        COS_SIN_TYPE temp_cos = current_cos;
        current_cos = current_cos - current_sin * sigma * factor;
        current_sin = temp_cos * sigma * factor + current_sin;
        theta = theta - sigma * cordic_phase[j];
        factor = factor >> 1;
    }
    s = current_sin;
    c = current_cos;
}
