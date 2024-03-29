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
 * Stream to Memory recv
 *  m_in: output located on memory
 *  size: size of the input, in bytes
 *  orig: index of the udp socket to recv
 *  s_in: stream connected to the arbiter
 */
void s2m_recv(short_ty *m_out, unsigned int size, unsigned int orig,
              hls::stream<udp_pkt> &s_in) {
#pragma HLS INTERFACE m_axi port = m_out bundle = gmem max_read_burst_length = 64 max_write_burst_length = 64
#pragma HLS INTERFACE s_axilite port = size
#pragma HLS INTERFACE s_axilite port = orig
#pragma HLS INTERFACE axis port = s_in
#pragma HLS INTERFACE s_axilite port = return

  // Interpret m_out as large output
  large_ty *lm_out = reinterpret_cast<large_ty *>(m_out);
  unsigned int count = size / sizeof(large_ty);

  udp_pkt pkt;

  // Let's assume now that size is multiple of count. Do we need to check if
  // dest == orig?
  for (int i = 0; i < count; i++) {
#pragma HLS PIPELINE II = 1
    pkt = s_in.read();
    lm_out[i] = pkt.data;
  }
}