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
 * Stream to Stream send
 *  s_in: input stream connected to the user kernel
 *  size: size of the input, in bytes
 *  dest: index of the udp socket to send
 *  s_out: stream connected to the arbiter
 */
void s2s_send(hls::stream<stream_ty> &s_in, unsigned int size,
              unsigned int dest, hls::stream<udp_pkt> &s_out) {
#pragma HLS INTERFACE axis port = s_in
#pragma HLS INTERFACE s_axilite port = size
#pragma HLS INTERFACE s_axilite port = dest
#pragma HLS INTERFACE axis port = s_out
#pragma HLS INTERFACE s_axilite port = return

  // Interpret s_in as large input
  unsigned int count = (size / sizeof(stream_ty)) / sizeof(large_ty);
  unsigned int count_int = sizeof(large_ty) / sizeof(stream_ty);
  unsigned int b_sty = 8 * sizeof(stream_ty);

  udp_pkt pkt;
  pkt.keep = -1;
  pkt.dest = dest;

  // Let's assume now that size is multiple of count
  for (int i = 0; i < count; i++) {
#pragma HLS PIPELINE II = 1
    ap_uint<512> value;
    for (int j = 0; j < count_int; j++) {
      value.range(b_sty * (i + 1) - 1, b_sty * i) = s_in.read();
    }
    pkt.data = value;
    pkt.last = 0;

    // determine if the packet is completed
    if (i == (count - 1) || ((i * sizeof(large_ty)) % pkt_size) == 0)
      pkt.last = 1;

    s_out.write(pkt);
  }
}