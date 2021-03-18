#include <stdio.h>
#include <stdlib.h>
#include <ap_int.h>

/*
 * Import HLS streaming library
 */
#include<hls_stream.h>

#define DATA_WIDTH 16
#define TAPS 21
#define RUN_LENGTH 100000

typedef ap_int<DATA_WIDTH> data_t;
typedef ap_int<32> acc_t;


/*
 * AXIS package
 */

struct axis_t
{
    data_t data;
    bool last;
};


/*
 * Interface specification
 */
void fir_hw(hls::stream<axis_t>&input_val, hls::stream<axis_t>&output_val);

