﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2020-2021 Intel Corporation
* Copyright 2020-2021 FUJITSU LIMITED
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef CPU_AARCH64_JIT_PRIMITIVE_CONF_HPP
#define CPU_AARCH64_JIT_PRIMITIVE_CONF_HPP

#include <stdint.h>

#include "common/primitive_attr.hpp"
#include "cpu/aarch64/cpu_isa_traits.hpp"

namespace zendnn {
namespace impl {
namespace cpu {
namespace aarch64 {

enum class jit_memory_tag_kind_t { ncsp, nspc, blocked, undef };

/* convolution */
enum conv_version_t {
    ver_unused,
    ver_fma,
    ver_sve_512,
};

enum conv_loop_order_t {
    loop_cgn,
    loop_gnc,
    loop_ngc,
    loop_gncw,
    loop_cwgn,
    loop_ngcw,
    loop_nhwcg,
    loop_nwcg
};

enum conv_kernel_kind_t { embd_bcast, expl_bcast };

enum conv_1x1_loop_order_t {
    loop_rbl,
    loop_rlb,
    loop_lbr,
    loop_lrb,
    loop_blr,
    loop_brl
};

enum conv_harness_t {
    harness_2d_reduction,
    harness_3d_reduction,
    harness_mb_reduction,
    harness_compute_full_spatial,
    harness_nxc
};

enum {
    FLAG_MB_FIRST = 1 << 0,
    FLAG_MB_LAST = 1 << 1,
    FLAG_OC_FIRST = 1 << 2,
    FLAG_OC_LAST = 1 << 3,
    FLAG_IC_FIRST = 1 << 4,
    FLAG_IC_LAST = 1 << 5,
    FLAG_SP_FIRST = 1 << 6,
    FLAG_SP_LAST = 1 << 7,
    FLAG_REDUCE_FIRST = 1 << 8,
    FLAG_REDUCE_LAST = 1 << 9,
    FLAG_ZERO_FILTER = 1 << 0, /* Controls whether the inner kernel skips
                                   loading weights-data from memory; this
                                   needs to happen on the first Group/16
                                   iteration. */
    FLAG_ZERO_BIAS = 1 << 1, /* Controls whether the inner kernel skip
                               loading bias data from memory */
    FLAG_COMPUTE_BIAS = 1 << 2, /* Controls bias computation during execution
                                    pass */
};

struct jit_conv_conf_t {
    prop_kind_t prop_kind;
	alg_kind_t alg_kind;
    conv_version_t ver;
    conv_loop_order_t loop_order;
    conv_harness_t harness;

    int simd_w;
    int ndims;
    int mb;
    int ngroups, ic, oc, oc_without_padding, ic_without_padding;
    int id, ih, iw, od, oh, ow;
    int f_pad, l_pad, t_pad;
    int back_pad, r_pad, b_pad;
    int kd, kh, kw;
    int stride_d, stride_h, stride_w;
    int dilate_d, dilate_h, dilate_w;
    format_tag_t src_tag, wei_tag, dst_tag; // temporary workaround
    bool with_bias;
    bool with_sum;
    bool with_eltwise;
    bool with_binary;

    bool is_fused_conv;
    int dw_conv_buffer_oc;

    post_ops_t::entry_t::eltwise_t eltwise;
    post_ops_t post_ops;

    int nthr, nthr_mb, nthr_g, nthr_oc_b, nthr_ic_b;

    int idp, ihp, iwp, ohp, owp;
    int nb_ic, ic_block;
    int nb_oc, oc_block;
    int nb_iw, iw_block;
    int nb_ow, ow_block;
    int nb_oc_blocking; /* used in jit kernels for nb_oc work blocking taking
                           into account vector registers distribution */
    int nb_oc_blocking_thr_chunk; /* used for distribution of nb_oc work
                                      within threads */
    int nb_ic_blocking, nb_ic_blocking_max; // blocking of nb_ic work
    int nb_ic_L2;
    int h_blocking;
    int nb_oc_L2;
    int ic_tail, oc_tail;
    int ur_h, ur_w;
    int ur_w_tail;
    int ur_ic, ur_kw;
    bool is_1stconv;
    int nonblk_group_off;
    /* fma sve512_core */
    conv_kernel_kind_t kernel_kind;

    int tr_iw, tr_ih;
    int tr_kw, tr_kh;
    int tr_src_num_guard_elems;

    // Transpose buffer management
    size_t tr_src_buf_size, tr_src_buf_count;
    size_t tr_diff_dst_buf_size, tr_diff_dst_buf_count;
    int nthr_mb_work;

    /* 1st conv */
    int tr_ld;
    int kh_step;
    /* sve_512_u8s8u8 */
    int typesize_in;
    int typesize_out;
    int typesize_bia;
    int typesize_acc;
    int ic_nb1, ic_nb2;
    int oc_nb1;
    int ur_ow_max, ur_ow, ur_ow_tail;
    int ur_ow_nsteps;
    data_type_t bia_dt;
    /* bf16 data-type for output */
    data_type_t dst_dt;
    data_type_t src_dt;
    /* bf16 weights update */
    data_type_t wei_dt;
    data_type_t dsrc_dt;
    data_type_t dwei_dt;
    bool expl_bcast;
    bool large_spatial, large_w_filter;
    int is_oc_scale;
    int max_regs_ur; // maximum accumulation registers
    // dw conv
    int nb_ch, ch_block, nb_ch_blocking;
    bool is_depthwise, is_fast_depthwise, is_resrc_depthwise;
    int aligned_threads;
    // large spatial
    int h_blk_size, oh_blk_size;
    // s8s8 convolution
    bool signed_input;
    bool need_saturation;
    float wei_adj_scale;
    // zero-point compensation
    bool src_zero_point;
    bool dst_zero_point;
    bool zp_src_is_common; // common, otherwise (TODO) per-channel

    bool uses_permw_transposition;
    bool transpose_src;
    bool transpose_dst;
    int ic_block_step;

    cpu_isa_t isa;

    bool is_hw_transp; // spatial dim height-width transposed
	//ReLU and BatchNorm fusion flags
	bool reluFused;
	bool batchNormFused;
};

// calculates filter size taking into account dilation
inline int calculate_extended_filter_size(int filter_size, int dilation) {
    return (filter_size - 1) * (dilation + 1) + 1;
}

inline int calculate_end_padding(int start_padding, int dst_size, int src_size,
        int spatial_stride, int dilated_filter_size) {
    return (dst_size - 1) * spatial_stride + dilated_filter_size
            - (src_size + start_padding);
}

inline status_t init_tag(format_tag_t &tag, const memory_desc_wrapper &mdw,
        const format_tag_t &tag_value) {
    if (mdw.format_kind() == format_kind::any) return status::unimplemented;

    tag = mdw.matches_one_of_tag(tag_value);
    return tag == tag_value ? status::success : status::unimplemented;
}

struct jit_conv_call_s {
    const void *src; /* hack, non-const for backward_data */
    const void *dst; /* hack, non-const for forward */
    const void *filt; /* hack, non-const for backward_weights */
    const void *bias; /* hack, non-const for backward_bias */
    const void *src_prf;
    const void *dst_prf;
    const void *filt_prf;
    const void *bias_prf;
    const void *scales;
    const void *acc_s32;
    const void *compensation;
    const int32_t *zp_compensation;
    const int32_t *src_zero_point;
    const int32_t *dst_zero_point;
    const void *tile_cfg;
    const void *tile_cfg_tail;

    // ptr to table of void * elements that are pointers to
    // post_op binary src1 tensors
    const void *post_ops_binary_rhs_arg_vec;
    // logical (# of elems) offset to the processed output channel
    // (for broadcasting [1,OC,1,1])
    size_t oc_l_off;
    const void *dst_orig; // pointer to dst memory (no offset)

    size_t oc_l_off_prf;
    const void *dst_orig_prf;

    size_t kd_offset;
    size_t kd_offset_prf;
    size_t kh_offset;
    size_t kh_offset_prf;
    size_t os_index_begin;
    size_t os_index_begin_prf;
    size_t os_index_end;
    size_t os_index_end_prf;
    size_t kd_padding;
    size_t kd_padding_prf;
    size_t kh_padding;
    size_t kh_padding_prf;
    size_t iwb;
    size_t iwb_prf;
    size_t owb;
    size_t owb_prf;
    size_t kw_padding;
    size_t channel;
    size_t channel_prf;
    size_t oc_blocks;
    size_t ur_w;
    size_t ur_str_w;
    size_t ch_blocks;
    size_t ch_blocks_prf;
    size_t reduce_work;
    size_t reduce_work_prf;
    size_t load_work;
    size_t load_work_prf;
    size_t t_overflow;
    size_t b_overflow;
    size_t f_overflow;
    size_t back_overflow;
    size_t last_h;
    size_t tail;
    size_t current_iw;
    size_t is_osb;
    int flags;
    int flags_prf;
    int oc_flag;
};

struct jit_dw_conv_call_s {
    const void *input;
    const void *output;
    const void *filter;
    const void *bias;
    size_t kh_count;
    size_t oh_count;
    size_t oh_index;
    size_t filter_pad_off;
    unsigned char
            exec_flags; /* Flags passed by driver execution to inner kernel */
};

struct jit_1x1_conv_conf_t {
    prop_kind_t prop_kind;
    conv_version_t ver;

    int ndims;
    int mb;
    int ngroups, ic, oc, oc_without_padding, ic_without_padding;
    int id, ih, iw, od, oh, ow;
    int f_pad, t_pad, l_pad;
    int kd, kh, kw;
    int stride_d, stride_h, stride_w;
    format_tag_t src_tag, wei_tag, dst_tag; // temporary workaround
    bool with_bias;
    bool with_sum;
    bool with_eltwise;
    bool with_binary;
    bool with_dw_conv;

    post_ops_t post_ops;
    post_ops_t::entry_t::eltwise_t eltwise;

    int is, os;
    int ic_block, oc_block;

    int ur, ur_tail;

    int reduce_dim, reduce_block, nb_reduce, nb_reduce_blocking,
            nb_reduce_blocking_max;
    int load_dim, load_block, nb_load, nb_load_blocking, nb_load_blocking_max,
            nb_load_chunk;
    int bcast_dim, bcast_block, nb_bcast, nb_bcast_blocking,
            nb_bcast_blocking_max;

    int reduce_loop_unroll, reduce_loop_bcast_step, reduce_loop_load_step;
    int load_loop_load_step, load_loop_iter_step;
    int bcast_loop_output_step, bcast_loop_output_substep;
    int bcast_loop_bcast_step, bcast_loop_bcast_substep;
    int fma_step;
    int load_grp_count;
    conv_1x1_loop_order_t loop_order;
    bool use_vmovntps;
    /* sve512 core */
    bool expl_bcast;
    int typesize_in;
    int typesize_out;
    int typesize_bia;
    int typesize_acc;
    /* 4fma */
    bool transpose_src;
    int tr_is;
    int nthr, nthr_mb, nthr_g, nthr_oc_b, nthr_ic_b;
    int is_oc_scale;
    data_type_t bia_dt;
    data_type_t dst_dt;
    bool signed_input;
    float wei_adj_scale;
    // zero-point compensation
    bool src_zero_point;
    bool dst_zero_point;
    bool zp_src_is_common; // common, otherwise (TODO) per-channel

    cpu_isa_t isa;
    bool uses_permw_transposition;
};

struct jit_1x1_conv_call_s {
    const void *bcast_data;
    const void *load_data;
    const void *output_data;
    const void *bias_data; // used in forward and backward_weights only
    const void *acc_s32;
    const void *scales;
    const void *compensation;
    const void *store_buffer;
    const int32_t *zp_compensation;
    const int32_t *src_zero_point;
    const int32_t *dst_zero_point;

    // ptr to table of void * elements that are pointers to
    // post_op binary src1 tensors
    const void *post_ops_binary_rhs_arg_vec;
    // logical (# of elems) offset to the processed output channel
    // (for broadcasting [1,OC,1,1])
    size_t oc_l_off;
    const void *dst_orig; // pointer to dst memory (not offseted)

    size_t load_dim;
    size_t bcast_dim;
    size_t reduce_dim;

    size_t output_stride; // used in backward_weights only

    size_t first_last_flag;
};

struct jit_pool_conf_t {
    int ndims;
    int mb, c, c_without_padding;
    int id, ih, iw, od, oh, ow;
    int stride_d, stride_h, stride_w;
    int kd, kh, kw;
    int f_pad, t_pad, l_pad;
    alg_kind_t alg;
    bool is_training;
    bool pad_w_is_null;
    bool is_backward;
    bool simple_alg;
    bool is_c_padded;
    data_type_t ind_dt;

    int c_block, c_tail, nb_c;
    int ur_bc, ur_bc_tail;
    int ur_c, ur_c_tail;
    int ur;
    size_t tail[4];
    bool safe_c_tail;
    data_type_t src_dt;
    data_type_t dst_dt;

    int dt_size;
    bool is_bf16;
    jit_memory_tag_kind_t tag_kind;
    bool is_plain() const {
        return (tag_kind == jit_memory_tag_kind_t::ncsp
                || tag_kind == jit_memory_tag_kind_t::nspc);
    }

    cpu_isa_t isa;
    post_ops_t post_ops;
    bool with_postops;
    bool with_eltwise;
    bool with_binary;
};

struct jit_pool_call_s {
    const void *src;
    const void *dst;
    const void *indices;
    const void *src_prf;
    const void *dst_prf;
    const void *indices_prf;
    const void *post_ops_binary_rhs_arg_vec;
    size_t c_elem_off;
    size_t zero_ih;
    size_t zero_id;
    const void *zero_ptr;
    size_t kd_padding;
    size_t kh_padding;
    size_t kh_padding_shift;
    size_t kd_padding_shift;
    size_t kw_padding;
    const void *init_value;
    float ker_area_h;
    size_t ur_bc; // contains number of channel blocks to processing
    size_t b_c; // contains number of channel blocks already processed
};

} // namespace aarch64
} // namespace cpu
} // namespace impl
} // namespace zendnn

#endif
