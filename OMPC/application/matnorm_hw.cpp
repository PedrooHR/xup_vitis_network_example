/**
 * Author: Pedro Henrique Rosso
 */

#include <ap_int.h>
#include <hls_stream.h>

#include "matutils.h"

void matnorm_hw(hls::stream<wide_ty> &A, buffer_ty *A_out, size_t size,
                size_t factor) {
#pragma HLS INTERFACE axis register_mode = both register port = A
#pragma HLS INTERFACE m_axi depth = DEPTH_ port = A_out bundle = gmem max_read_burst_length = 64 max_write_burst_length = 64
#pragma HLS INTERFACE s_axilite port = size
#pragma HLS INTERFACE s_axilite port = factor
#pragma HLS INTERFACE s_axilite port = return

  // Interpret A_out as wide array
  wide_ty *wA_out = reinterpret_cast<wide_ty *>(A_out);

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

  // Do the normalization
  for (size_t i = 0; i < wide_size; i++) {
#pragma HLS pipeline II = 1
    wide_ty A_wv = A.read();
    wide_ty Out_wv = 0;

    // Unroll the normalization of the read memory
    for (int j = 0; j < num_ele; j++) {
#pragma HLS unroll factor = num_ele
      int idx = i * num_ele + j;
      int end = (j + 1) * bt_bits - 1;
      int start = j * bt_bits;

      // Do the sum
      Out_wv.range(end, start) = A_wv.range(end, start) / factor;
    }
    A_out[i] = Out_wv;
  }
}
