
/**
 * Designs built for testing OMPC Task Streaming. This designs only represent
 * operations to demonstrate Design 1:
 *  - 'matmul_hw_0': multiplies two memory matrices and put results in the first
 * stream of vadd_hw_0
 *  - 'matmul_hw_0': multiplies two memory matrices and put results in the
 * second stream of vadd_hw_0
 *  - 'matadd_hw_0': sums two stream matrices and put results in a out stream,
 * which is linked to network kernels
 *
 * Design 2:
 *  - 'matnorm_hw_0': normalizes a matrix given a factor and put the result in a
 * memory buffer
 */

#include <ap_int.h>
#include <stddef.h>

#include <hls_stream.h>

using buffer_ty = unsigned int;
using wide_ty = ap_uint<512>;

constexpr unsigned int DEPTH_ = 256;
constexpr unsigned int SQDEPTH_ = 16;
constexpr size_t types_r = sizeof(wide_ty) / sizeof(buffer_ty);

void matmul_hw(buffer_ty *A, buffer_ty *B, hls::stream<wide_ty> &C, size_t size,
               size_t size_sq);

void matadd_hw(hls::stream<wide_ty> &A, hls::stream<wide_ty> &B,
               hls::stream<wide_ty> &C, size_t size);

void matnorm_hw(hls::stream<wide_ty> &A, buffer_ty *A_out, size_t size,
                float factor);