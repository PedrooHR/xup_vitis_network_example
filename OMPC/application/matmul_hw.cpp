/**
 * Author: Pedro Henrique Rosso
 */

#include <ap_int.h>
#include <hls_stream.h>

#include "matutils.h"

buffer_ty mul_pe(buffer_ty *A, buffer_ty *B, int i,  int idx,
                 size_t size_sq) {
#pragma HLS inline
  buffer_ty red = 0;
  if (types_r == 16) {
    red = A[i * size_sq +  0] * B[ 0 * size_sq + idx] + 
          A[i * size_sq +  1] * B[ 1 * size_sq + idx] + 
          A[i * size_sq +  2] * B[ 2 * size_sq + idx] + 
          A[i * size_sq +  3] * B[ 3 * size_sq + idx] + 
          A[i * size_sq +  4] * B[ 4 * size_sq + idx] + 
          A[i * size_sq +  5] * B[ 5 * size_sq + idx] + 
          A[i * size_sq +  6] * B[ 6 * size_sq + idx] + 
          A[i * size_sq +  7] * B[ 7 * size_sq + idx] + 
          A[i * size_sq +  8] * B[ 8 * size_sq + idx] + 
          A[i * size_sq +  9] * B[ 9 * size_sq + idx] + 
          A[i * size_sq + 10] * B[10 * size_sq + idx] + 
          A[i * size_sq + 11] * B[11 * size_sq + idx] + 
          A[i * size_sq + 12] * B[12 * size_sq + idx] + 
          A[i * size_sq + 13] * B[13 * size_sq + idx] + 
          A[i * size_sq + 14] * B[14 * size_sq + idx] + 
          A[i * size_sq + 15] * B[15 * size_sq + idx]; 
   } else if (types_r == 8) {
    red = A[i * size_sq +  0] * B[ 0 * size_sq + idx] + 
          A[i * size_sq +  1] * B[ 1 * size_sq + idx] + 
          A[i * size_sq +  2] * B[ 2 * size_sq + idx] + 
          A[i * size_sq +  3] * B[ 3 * size_sq + idx] + 
          A[i * size_sq +  4] * B[ 4 * size_sq + idx] + 
          A[i * size_sq +  5] * B[ 5 * size_sq + idx] + 
          A[i * size_sq +  6] * B[ 6 * size_sq + idx] + 
          A[i * size_sq +  7] * B[ 7 * size_sq + idx];
  } else {
    red = 0;
  }
  return red;
}

void matmul_hw(buffer_ty *A, buffer_ty *B, hls::stream<wide_ty> &C, size_t size,
               size_t size_sq) {
#pragma HLS INTERFACE m_axi depth = DEPTH_ port = A bundle =                   \
    gmem0 max_read_burst_length = 64 max_write_burst_length = 64
#pragma HLS INTERFACE m_axi depth = DEPTH_ port = B bundle =                   \
    gmem1 max_read_burst_length = 64 max_write_burst_length = 64
#pragma HLS INTERFACE axis register_mode = both register port = C
#pragma HLS INTERFACE s_axilite port = size
#pragma HLS INTERFACE s_axilite port = size_sq
#pragma HLS INTERFACE s_axilite port = return

  const int num_ele = sizeof(wide_ty) / sizeof(buffer_ty);
  const int bt_bits = sizeof(buffer_ty) * 8;
  const int parts = size_sq / num_ele;

  for (int i = 0; i < size_sq; i++) {
    for (int j = 0; j < parts; j++) {
      // This partition is to put result on stream
      wide_ty res = 0;
      for (int k = 0; k < num_ele; k++) {
#pragma HLS unroll factor = num_ele
        int idx = j * num_ele + k;
        int end = (k + 1) * bt_bits - 1;
        int start = k * bt_bits;

        res.range(end, start) = mul_pe(A, B, i, idx, size_sq);
      }
      C.write(res);
    }
  }
}
