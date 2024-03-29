/**
 * Author: Pedro Henrique Rosso
 */

#include <ap_int.h>
#include <hls_stream.h>

#include "matutils.h"

void matadd_hw(hls::stream<wide_ty> &A, hls::stream<wide_ty> &B,
               hls::stream<wide_ty> &C, size_t size) {
#pragma HLS INTERFACE axis register_mode = both register port = A
#pragma HLS INTERFACE axis register_mode = both register port = B
#pragma HLS INTERFACE axis register_mode = both register port = C
#pragma HLS INTERFACE s_axilite port = size
#pragma HLS INTERFACE s_axilite port = return

  // Original definitions
  const int bt_bits = sizeof(buffer_ty) * 8;
  const int bt_bytes = sizeof(buffer_ty);

  // New definitiones
  const int wide_bits = sizeof(wide_ty) * 8;
  const int wide_bytes = sizeof(wide_ty);

  // How many of our elements are inside the new representation
  const int num_ele = wide_bytes / bt_bytes;

  // Redefine size
  int wide_size = (size * bt_bytes) / wide_bytes;

  // Do the addition
  for (size_t i = 0; i < wide_size; i++) {
#pragma HLS pipeline II = 1
    wide_ty A_wv = A.read();
    wide_ty B_wv = B.read();
    wide_ty C_wv = 0;

    // Unroll the addition of the read memory
    for (int j = 0; j < num_ele; j++) {
#pragma HLS unroll factor = num_ele
      int idx = i * num_ele + j;
      int end = (j + 1) * bt_bits - 1;
      int start = j * bt_bits;

      // Do the sum
      C_wv.range(end, start) = A_wv.range(end, start) + B_wv.range(end, start);
    }
    C.write(C_wv);
  }
}
