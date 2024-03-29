#include "ap_int.h"
#include "hls_stream.h"

#define pkt_size 1472

struct udp_pkt {
  ap_uint<512> data;
  ap_uint<64> keep;
  ap_uint<16> dest;
  ap_uint<1> last;
};

typedef ap_uint<8> short_ty; // Short type, equivalent to char (1 byte)
typedef ap_uint<512> large_ty; // Large type (64 bytes)
typedef ap_uint<512> stream_ty; // Definition for application stream size

/**
 * Stream to Stream recv
 *  s_out: output stream connected to the user kernel
 *  size: size of the input, in bytes
 *  orig: index of the udp socket to recv
 *  s_in: stream connected to the arbiter
 */
void s2s_recv(hls::stream<stream_ty> &s_out, unsigned int size,
              unsigned int orig, hls::stream<udp_pkt> &s_in) {
#pragma HLS INTERFACE axis port = s_in
#pragma HLS INTERFACE s_axilite port = size
#pragma HLS INTERFACE s_axilite port = orig
#pragma HLS INTERFACE axis port = s_out
#pragma HLS INTERFACE s_axilite port = return

  hls::stream<ap_uint<512>> s_int; 

  unsigned int count = (size / sizeof(stream_ty)) / sizeof(large_ty);
  unsigned int count_int = sizeof(large_ty) / sizeof(stream_ty);
  unsigned int b_sty = 8 * sizeof(stream_ty);

  udp_pkt pkt;

  // Let's assume now that size is multiple of count. Do we need to check if
  // dest == orig?
  for (int i = 0; i < count; i++) {
#pragma HLS PIPELINE II = 1
    pkt = s_in.read();

    for (int j = 0; j < count_int; j++) {
      s_out.write(pkt.data.range(b_sty * (i + 1) - 1, b_sty * i));
    }
  }
}
