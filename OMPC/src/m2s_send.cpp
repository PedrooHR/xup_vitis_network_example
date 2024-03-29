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
 * Memory to Stream send
 *  m_in: input located on memory
 *  size: size of the input, in bytes
 *  dest: index of the udp socket to send
 *  s_out: stream connected to the arbiter
 */
void m2s_send(short_ty *m_in, unsigned int size, unsigned int dest,
              hls::stream<udp_pkt> &s_out) {
#pragma HLS INTERFACE m_axi port = m_in bundle = gmem max_read_burst_length = 64 max_write_burst_length = 64
#pragma HLS INTERFACE s_axilite port = size
#pragma HLS INTERFACE s_axilite port = dest
#pragma HLS INTERFACE axis port = s_out
#pragma HLS INTERFACE s_axilite port = return

  // Interpret m_in as large input
  large_ty *lm_in = reinterpret_cast<large_ty *>(m_in);
  unsigned int count = size / sizeof(large_ty);

  udp_pkt pkt;
  pkt.keep = -1;
  pkt.dest = dest;

  // Let's assume now that size is multiple of count
  for (int i = 0; i < count; i++) {
#pragma HLS PIPELINE II = 1
    pkt.data = lm_in[i];
    pkt.last = 0;

    // determine if the packet is completed
    if (i == (count - 1) || ((i * sizeof(large_ty)) % pkt_size) == 0)
      pkt.last = 1;

    s_out.write(pkt);
  }
}