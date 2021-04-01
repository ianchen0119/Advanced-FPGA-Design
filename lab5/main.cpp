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


COS_SIN_TYPE matrixmul(COS_SIN_TYPE A[2][2], COS_SIN_TYPE B[2][1]) {
  #pragma HLS ARRAY_RESHAPE variable=A complete dim=2
  #pragma HLS ARRAY_RESHAPE variable=B complete dim=1
  /* for each row and column of AB */
  COS_SIN_TYPE AB[2][1];
  row: for(int i = 0; i < 2; ++i) {
    col: for(int j = 0; j < 1; ++j) {
         #pragma HLS PIPELINE II=1
         /* compute (AB)i,j */
         int ABij = 0;
         product: for(int k = 0; k < 2; ++k) {
             ABij += A[i][k] * B[k][j];
      }
      AB[i][j] = ABij;
    }
  }
  return AB;
}

COS_SIN_TYPE rotate(THETA_TYPE theta, COS_SIN_TYPE sc_mat[2][1]){
	COS_SIN_TYPE s = 0.01745;
	COS_SIN_TYPE c = 0.99985;
	COS_SIN_TYPE one_degree[2][2] = {{c, -s}, {s, c}};
	return matrixmul(one_degree, sc_mat);
}

COS_SIN_TYPE sincos(THETA_TYPE theta){
	COS_SIN_TYPE sc_mat[2][1] = {{1},{0}};
	for(int i = 0; i < theta; ++i) {
		sc_mat = rotate(1, sc_mat);
	}
	return sc_mat;
}
