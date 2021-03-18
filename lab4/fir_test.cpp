#include "fir.h"

void fir_sw(hls::stream<axis_t> &input_val, hls::stream<axis_t> &output_val)
{
	int i;
	static data_t shift_reg[TAPS] = {0};
	const data_t coeff[TAPS] = {6,0,-4,-3,5,6,-6,-13,7,44,64,44,7,-13,
                                 -6,6,5,-3,-4,0,6};
	axis_t result;
	for(i=0; i < RUN_LENGTH; i++){
		data_t sample;
		sample = input_val.read().data;

		//Shift Register
		for(int j=0; j < TAPS-1; j++){
			shift_reg[j] = shift_reg[j+1];
		}
		shift_reg[TAPS-1] = sample;

		//Filter Operation
		acc_t acc = 0;
		for(int k=0; k < TAPS; k++){
			acc += shift_reg[k] * coeff[k];
		}

		result.data = (data_t)acc;
		result.last = (i == RUN_LENGTH-1)? 1:0;
		output_val.write(result);
	}
}

int main()
{
	hls::stream<axis_t> input_sw;
	hls::stream<axis_t> input_hw;
	hls::stream<axis_t> output_hw;
	hls::stream<axis_t> output_sw;

	//Write the input values
	for(int i = 0; i < RUN_LENGTH; i++){
		axis_t temp;
		temp.data = (data_t) 1;
		temp.last = (i == RUN_LENGTH-1)?1:0;
		input_sw.write(temp);
		input_hw.write(temp);
	}

	//Call to software model of fir
  	fir_sw(input_sw, output_sw);

	//Call to hardware model of fir
	fir_hw(input_hw, output_hw);

	for(int k=0; k < RUN_LENGTH; k++){
		data_t sw, hw;
		sw = output_sw.read().data;
		hw = output_hw.read().data;
		std::cout<< hw << std::endl;
		if(sw != hw){
			printf("ERROR: k = %d sw = %d hw = %d\n",k,sw,hw);
		      return 1;
		}
	}
	printf("Success! both SW and HW models match.\n");
	return 0;
}

