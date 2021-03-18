#include "fir.h"

/*
 * Interface specification
 */
void fir_hw(hls::stream<axis_t>&input_val, hls::stream<axis_t>&output_val)
{
	/*
	 * HLS Stream interface pragma
	 */
#pragma HLS INTERFACE axis port=input_val bundle=INPUT_STREAM
#pragma HLS INTERFACE axis port=output_val bundle=OUTPUT_STREAM
	/*
	 * HLS Stream interface pragma end
	 */
#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

	int i;
	static data_t shift_reg[TAPS] = {0};
	const data_t coeff[TAPS] = {6,0,-4,-3,5,6,-6,-13,7,44,64,44,7,-13,
                                 -6,6,5,-3,-4,0,6};

	axis_t result;
	data_t sample;

	RUN_LENGTH_LOOP: for(i=0; i < RUN_LENGTH; i++){
#pragma HLS PIPELINE

		/*
		 * AXIS read data
		 */
		sample = input_val.read().data;

		/*
		 * AXIS read data end
		 */

		//Shift Register
		for(int j=0; j < TAPS-1; j++){
#pragma HLS UNROLL
			shift_reg[j] = shift_reg[j+1];
		}
		shift_reg[TAPS-1] = sample;

		//Filter Operation
		acc_t acc = 0;
		for(int k=0; k < TAPS; k++){
#pragma HLS UNROLL
			acc += shift_reg[k] * coeff[k];
		}

		/*
		 * AXIS write output here
		 */

		axis_t result;
		result.data = (data_t)acc;
		result.last = (i == RUN_LENGTH-1)?1:0;
		output_val.write(result);
		/*
		 * AXIS write output end
		 */
	}

	return;
}
