﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2016-2020 Intel Corporation
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

#ifndef COMMON_CONVOLUTION_PD_HPP
#define COMMON_CONVOLUTION_PD_HPP

#include "zendnn.h"

#include "c_types_map.hpp"
#include "primitive_desc.hpp"
#include "utils.hpp"

namespace zendnn {
namespace impl {

status_t conv_desc_init(convolution_desc_t *conv_desc, prop_kind_t prop_kind,
        alg_kind_t alg_kind, const memory_desc_t *src_desc,
        const memory_desc_t *weights_desc, const memory_desc_t *bias_desc,
        const memory_desc_t *dst_desc, const dims_t strides,
        const dims_t dilates, const dims_t padding_l, const dims_t padding_r,
        bool reluFused, bool batchNormFused,
        const memory_desc_t *batchNormScale_desc,
        const memory_desc_t *batchNormMean_desc,
        const memory_desc_t *batchNormOffset_desc);

memory_desc_t *conv_prop_invariant_src_d(convolution_desc_t *desc);
memory_desc_t *conv_prop_invariant_wei_d(convolution_desc_t *desc);
memory_desc_t *conv_prop_invariant_bia_d(convolution_desc_t *desc);
memory_desc_t *conv_prop_invariant_dst_d(convolution_desc_t *desc);
memory_desc_t *conv_prop_invariant_batchNormScale_d(convolution_desc_t *desc);
memory_desc_t *conv_prop_invariant_batchNormMean_d(convolution_desc_t *desc);
memory_desc_t *conv_prop_invariant_batchNormOffset_d(convolution_desc_t *desc);
const memory_desc_t *conv_prop_invariant_src_d(const convolution_desc_t *desc);
const memory_desc_t *conv_prop_invariant_wei_d(const convolution_desc_t *desc);
const memory_desc_t *conv_prop_invariant_bia_d(const convolution_desc_t *desc);
const memory_desc_t *conv_prop_invariant_dst_d(const convolution_desc_t *desc);
const memory_desc_t *conv_prop_invariant_batchNormScale_d(const convolution_desc_t *desc);
const memory_desc_t *conv_prop_invariant_batchNormMean_d(const convolution_desc_t *desc);
const memory_desc_t *conv_prop_invariant_batchNormOffset_d(const convolution_desc_t *desc);

struct convolution_fwd_pd_t;

struct convolution_pd_t : public primitive_desc_t {
    static constexpr auto base_pkind = primitive_kind::convolution;

    convolution_pd_t(const convolution_desc_t *adesc,
            const primitive_attr_t *attr,
            const convolution_fwd_pd_t *hint_fwd_pd)
        : primitive_desc_t(attr, base_pkind)
        , desc_(*adesc)
        , hint_fwd_pd_(hint_fwd_pd) {}

    const convolution_desc_t *desc() const { return &desc_; }
    const op_desc_t *op_desc() const override {
        return reinterpret_cast<const op_desc_t *>(this->desc());
    }

    status_t query(query_t what, int idx, void *result) const override {
        switch (what) {
            case query::prop_kind:
                *(prop_kind_t *)result = desc()->prop_kind;
                break;
            case pkind_traits<base_pkind>::query_d:
                *(const convolution_desc_t **)result = desc();
                break;
            default: return primitive_desc_t::query(what, idx, result);
        }
        return status::success;
    }

    /* common conv aux functions */

    dim_t MB() const { return invariant_src_md()->dims[0]; }

    dim_t IC() const { return invariant_src_md()->dims[1]; }
    dim_t OC() const { return invariant_dst_md()->dims[1]; }
    dim_t G() const { return with_groups() ? invariant_wei_md()->dims[0] : 1; }

    dim_t ID() const {
        return ndims() >= 5 ? invariant_src_md()->dims[ndims() - 3] : 1;
    }
    dim_t IH() const {
        return ndims() >= 4 ? invariant_src_md()->dims[ndims() - 2] : 1;
    }
    dim_t IW() const { return invariant_src_md()->dims[ndims() - 1]; }

    dim_t OD() const {
        return ndims() >= 5 ? invariant_dst_md()->dims[ndims() - 3] : 1;
    }
    dim_t OH() const {
        return ndims() >= 4 ? invariant_dst_md()->dims[ndims() - 2] : 1;
    }
    dim_t OW() const { return invariant_dst_md()->dims[ndims() - 1]; }

    dim_t KD() const {
        return ndims() >= 5
                ? invariant_wei_md()->dims[ndims() + with_groups() - 3]
                : 1;
    }
    dim_t KH() const {
        return ndims() >= 4
                ? invariant_wei_md()->dims[ndims() + with_groups() - 2]
                : 1;
    }
    dim_t KW() const {
        return invariant_wei_md()->dims[ndims() + with_groups() - 1];
    }

    dim_t KSD() const { return ndims() >= 5 ? desc_.strides[ndims() - 5] : 1; }
    dim_t KSH() const { return ndims() >= 4 ? desc_.strides[ndims() - 4] : 1; }
    dim_t KSW() const { return desc_.strides[ndims() - 3]; }

    dim_t KDD() const { return ndims() >= 5 ? desc_.dilates[ndims() - 5] : 0; }
    dim_t KDH() const { return ndims() >= 4 ? desc_.dilates[ndims() - 4] : 1; }
    dim_t KDW() const { return desc_.dilates[ndims() - 3]; }

    dim_t padFront() const {
        return ndims() >= 5 ? desc_.padding[0][ndims() - 5] : 0;
    }
    dim_t padBack() const {
        return ndims() >= 5 ? desc_.padding[1][ndims() - 5] : 0;
    }
    dim_t padT() const {
        return ndims() >= 4 ? desc_.padding[0][ndims() - 4] : 0;
    }
    dim_t padB() const {
        return ndims() >= 4 ? desc_.padding[1][ndims() - 4] : 0;
    }
    dim_t padL() const { return desc_.padding[0][ndims() - 3]; }
    dim_t padR() const { return desc_.padding[1][ndims() - 3]; }

    int ndims() const { return invariant_src_md()->ndims; }

    bool with_bias() const {
        auto *bia_d = desc()->prop_kind == prop_kind::backward_weights
                ? &desc()->diff_bias_desc
                : &desc()->bias_desc;
        return !memory_desc_wrapper(bia_d).is_zero();
    }
    bool with_groups() const {
        return invariant_wei_md()->ndims == ndims() + 1;
    }

    bool is_fwd() const {
        return utils::one_of(desc_.prop_kind, prop_kind::forward_training,
                prop_kind::forward_inference);
    }

    bool is_bwd_d() const {
        return desc_.prop_kind == prop_kind::backward_data;
    }

    bool is_bwd_w() const {
        return desc_.prop_kind == prop_kind::backward_weights;
    }

    bool has_zero_dim_memory() const {
        const auto s_d = memory_desc_wrapper(*invariant_src_md());
        const auto d_d = memory_desc_wrapper(*invariant_dst_md());
        return s_d.has_zero_dim() || d_d.has_zero_dim();
    }

    const memory_desc_t *invariant_src_md() const {
        return desc()->prop_kind == prop_kind::backward_data ? diff_src_md()
                                                             : src_md();
    }
    const memory_desc_t *invariant_wei_md(int index = 0) const {
        return desc()->prop_kind == prop_kind::backward_weights
                ? diff_weights_md(index)
                : weights_md(index);
    }
    const memory_desc_t *invariant_bia_md() const {
        return invariant_wei_md(1);
    }
    const memory_desc_t *invariant_dst_md() const {
        return is_fwd() ? dst_md() : diff_dst_md();
    }

    const memory_desc_t *invariant_batchNormScale_md() const {
        return invariant_wei_md(2);
    }

    const memory_desc_t *invariant_batchNormMean_md() const {
        return invariant_wei_md(3);
    }

    const memory_desc_t *invariant_batchNormOffset_md() const {
        return invariant_wei_md(4);
    }

    bool with_batchNormScale() const {
        return !memory_desc_wrapper(*invariant_batchNormScale_md()).is_zero();
    }
    bool with_batchNormMean() const {
        return !memory_desc_wrapper(*invariant_batchNormMean_md()).is_zero();
    }
    bool with_batchNormOffset() const {
        return !memory_desc_wrapper(*invariant_batchNormOffset_md()).is_zero();
    }

protected:
    convolution_desc_t desc_;
    const convolution_fwd_pd_t *hint_fwd_pd_;

    bool set_default_formats_common_template(memory_desc_t &src_md,
            format_tag_t src_tag, memory_desc_t &wei_md, format_tag_t wei_tag,
            memory_desc_t &dst_md, format_tag_t dst_tag,
            memory_desc_t &bia_md) {
        using namespace format_tag;

#define IS_OK(f) \
    do { \
        if ((f) != status::success) return false; \
    } while (0)
        if (src_md.format_kind == format_kind::any
                && !utils::one_of(src_tag, any, undef))
            IS_OK(memory_desc_init_by_tag(src_md, src_tag));
        if (dst_md.format_kind == format_kind::any
                && !utils::one_of(dst_tag, any, undef))
            IS_OK(memory_desc_init_by_tag(dst_md, dst_tag));
        if (wei_md.format_kind == format_kind::any
                && !utils::one_of(wei_tag, any, undef))
            IS_OK(memory_desc_init_by_tag(wei_md, wei_tag));
        if (with_bias() && bia_md.format_kind == format_kind::any)
            IS_OK(memory_desc_init_by_tag(bia_md, x));
#undef IS_OK

        return true;
    }

    bool set_default_alg_kind(alg_kind_t alg_kind) {
        assert(utils::one_of(alg_kind, alg_kind::convolution_direct,
                             alg_kind::convolution_winograd,
                             alg_kind::convolution_gemm,
                             alg_kind::convolution_ref));
        if (desc_.alg_kind == alg_kind::convolution_auto)
            desc_.alg_kind = alg_kind;
        return desc_.alg_kind == alg_kind;
    }

    bool expect_data_types(data_type_t src_dt, data_type_t wei_dt,
            data_type_t bia_dt, data_type_t dst_dt, data_type_t acc_dt) const {
        bool ok = true
                && (src_dt == data_type::undef
                        || invariant_src_md()->data_type == src_dt)
                && (wei_dt == data_type::undef
                        || invariant_wei_md()->data_type == wei_dt)
                && (dst_dt == data_type::undef
                        || invariant_dst_md()->data_type == dst_dt)
                && (acc_dt == data_type::undef
                        || desc_.accum_data_type == acc_dt);
        if (with_bias() && bia_dt != data_type::undef)
            ok = ok && invariant_bia_md()->data_type == bia_dt;
        return ok;
    }
};

struct convolution_fwd_pd_t : public convolution_pd_t {
    typedef convolution_fwd_pd_t base_class;
    typedef convolution_fwd_pd_t hint_class;

    convolution_fwd_pd_t(const convolution_desc_t *adesc,
            const primitive_attr_t *attr,
            const convolution_fwd_pd_t *hint_fwd_pd)
        : convolution_pd_t(adesc, attr, hint_fwd_pd)
        , src_md_(desc_.src_desc)
        , weights_md_(desc_.weights_desc)
        , bias_md_(desc_.bias_desc)
        , dst_md_(desc_.dst_desc)
        , batchNormScale_md_(desc_.batchNormScale_desc)
        , batchNormMean_md_(desc_.batchNormMean_desc)
        , batchNormOffset_md_(desc_.batchNormOffset_desc) {}

    arg_usage_t arg_usage(int arg) const override {
        if (utils::one_of(arg, ZENDNN_ARG_SRC, ZENDNN_ARG_WEIGHTS))
            return arg_usage_t::input;

        if (arg == ZENDNN_ARG_BIAS && with_bias()) return arg_usage_t::input;

        if (arg == ZENDNN_ARG_DST) return arg_usage_t::output;

        if (arg == ZENDNN_ARG_BN_SCALE) {
            return arg_usage_t::input;
        }
        if (arg == ZENDNN_ARG_BN_MEAN) {
            return arg_usage_t::input;
        }
        if (arg == ZENDNN_ARG_BN_OFFSET) {
            return arg_usage_t::input;
        }

        return primitive_desc_t::arg_usage(arg);
    }

    const memory_desc_t *arg_md(int arg) const override {
        switch (arg) {
            case ZENDNN_ARG_SRC: return src_md(0);
            case ZENDNN_ARG_WEIGHTS: return weights_md(0);
            case ZENDNN_ARG_BIAS: return weights_md(1);
            case ZENDNN_ARG_DST: return dst_md(0);
            case ZENDNN_ARG_BN_SCALE: return weights_md(2);
            case ZENDNN_ARG_BN_MEAN: return weights_md(3);
            case ZENDNN_ARG_BN_OFFSET: return weights_md(4);
            default: return convolution_pd_t::arg_md(arg);
        }
    }

    const memory_desc_t *src_md(int index = 0) const override {
        return index == 0 ? &src_md_ : &glob_zero_md;
    }
    const memory_desc_t *dst_md(int index = 0) const override {
        return index == 0 ? &dst_md_ : &glob_zero_md;
    }
    const memory_desc_t *weights_md(int index = 0) const override {
        if (index == 0) return &weights_md_;
        if (index == 1 && with_bias()) return &bias_md_;
        else if (index == 2) {
            return &batchNormScale_md_;
        }
        else if (index == 3) {
            return &batchNormMean_md_;
        }
        else if (index == 4) {
            return &batchNormOffset_md_;
        }
        return &glob_zero_md;
    }

    int n_inputs() const override {
        //return 2 + with_bias() + attr_post_op_dw_inputs()
        //        + n_binary_po_inputs();
        return 2 + with_bias() + with_batchNormScale() +\
               with_batchNormMean() + with_batchNormOffset() +\
               n_binary_po_inputs();
        /*
        auto num_bias = with_bias();
        auto num_scale = with_batchNormScale();
        auto num_mean = with_batchNormMean();
        auto num_offset = with_batchNormOffset();
        return 2 + num_bias + num_scale + num_mean +num_offset;
        */
    }

    int n_outputs() const override { return 1; }

protected:
    memory_desc_t src_md_;
    memory_desc_t weights_md_;
    memory_desc_t bias_md_;
    memory_desc_t dst_md_;
    memory_desc_t batchNormScale_md_;
    memory_desc_t batchNormMean_md_;
    memory_desc_t batchNormOffset_md_;

    bool set_default_formats_common(
            format_tag_t src_tag, format_tag_t wei_tag, format_tag_t dst_tag) {
        return set_default_formats_common_template(src_md_, src_tag,
                weights_md_, wei_tag, dst_md_, dst_tag, bias_md_);
    }

    int attr_post_op_dw_inputs() const {
        const auto &po = attr_.post_ops_;
        int conv = po.find(primitive_kind::convolution);
        if (conv == -1) return 0;
        return po.entry_[conv].depthwise_conv.bias_dt == data_type::undef ? 1
                                                                          : 2;
    }
};

struct convolution_bwd_data_pd_t : public convolution_pd_t {
    typedef convolution_bwd_data_pd_t base_class;
    typedef convolution_fwd_pd_t hint_class;

    convolution_bwd_data_pd_t(const convolution_desc_t *adesc,
            const primitive_attr_t *attr,
            const convolution_fwd_pd_t *hint_fwd_pd)
        : convolution_pd_t(adesc, attr, hint_fwd_pd)
        , diff_src_md_(desc_.diff_src_desc)
        , weights_md_(desc_.weights_desc)
        , bias_md_(desc_.bias_desc)
        , diff_dst_md_(desc_.diff_dst_desc) {}

    arg_usage_t arg_usage(int arg) const override {
        if (utils::one_of(arg, ZENDNN_ARG_WEIGHTS, ZENDNN_ARG_DIFF_DST))
            return arg_usage_t::input;

        if (arg == ZENDNN_ARG_DIFF_SRC) return arg_usage_t::output;

        return primitive_desc_t::arg_usage(arg);
    }

    const memory_desc_t *arg_md(int arg) const override {
        switch (arg) {
            case ZENDNN_ARG_DIFF_SRC: return diff_src_md(0);
            case ZENDNN_ARG_WEIGHTS: return weights_md(0);
            case ZENDNN_ARG_BIAS: return weights_md(1);
            case ZENDNN_ARG_DIFF_DST: return diff_dst_md(0);
            default: return convolution_pd_t::arg_md(arg);
        }
    }

    const memory_desc_t *diff_src_md(int index = 0) const override {
        return index == 0 ? &diff_src_md_ : &glob_zero_md;
    }
    const memory_desc_t *diff_dst_md(int index = 0) const override {
        return index == 0 ? &diff_dst_md_ : &glob_zero_md;
    }
    const memory_desc_t *weights_md(int index = 0) const override {
        if (index == 0) return &weights_md_;
        if (index == 1 && with_bias()) return &bias_md_;
        return &glob_zero_md;
    }

    int n_inputs() const override { return 2 + with_bias(); }
    int n_outputs() const override { return 1; }

    virtual bool support_bias() const { return false; }

protected:
    memory_desc_t diff_src_md_;
    memory_desc_t weights_md_;
    memory_desc_t bias_md_;
    memory_desc_t diff_dst_md_;

    bool set_default_formats_common(format_tag_t diff_src_tag,
            format_tag_t wei_tag, format_tag_t diff_dst_tag) {
        return set_default_formats_common_template(diff_src_md_, diff_src_tag,
                weights_md_, wei_tag, diff_dst_md_, diff_dst_tag, bias_md_);
    }
};

struct convolution_bwd_weights_pd_t : public convolution_pd_t {
    typedef convolution_bwd_weights_pd_t base_class;
    typedef convolution_fwd_pd_t hint_class;

    convolution_bwd_weights_pd_t(const convolution_desc_t *adesc,
            const primitive_attr_t *attr,
            const convolution_fwd_pd_t *hint_fwd_pd)
        : convolution_pd_t(adesc, attr, hint_fwd_pd)
        , src_md_(desc_.src_desc)
        , diff_weights_md_(desc_.diff_weights_desc)
        , diff_bias_md_(desc_.diff_bias_desc)
        , diff_dst_md_(desc_.diff_dst_desc) {}

    arg_usage_t arg_usage(int arg) const override {
        if (utils::one_of(arg, ZENDNN_ARG_SRC, ZENDNN_ARG_DIFF_DST))
            return arg_usage_t::input;

        if (arg == ZENDNN_ARG_DIFF_WEIGHTS) return arg_usage_t::output;

        if (arg == ZENDNN_ARG_DIFF_BIAS && with_bias())
            return arg_usage_t::output;

        return primitive_desc_t::arg_usage(arg);
    }

    const memory_desc_t *arg_md(int arg) const override {
        switch (arg) {
            case ZENDNN_ARG_SRC: return src_md(0);
            case ZENDNN_ARG_DIFF_WEIGHTS: return diff_weights_md(0);
            case ZENDNN_ARG_DIFF_BIAS: return diff_weights_md(1);
            case ZENDNN_ARG_DIFF_DST: return diff_dst_md(0);
            default: return convolution_pd_t::arg_md(arg);
        }
    }

    const memory_desc_t *src_md(int index = 0) const override {
        return index == 0 ? &src_md_ : &glob_zero_md;
    }
    const memory_desc_t *diff_dst_md(int index = 0) const override {
        return index == 0 ? &diff_dst_md_ : &glob_zero_md;
    }
    const memory_desc_t *diff_weights_md(int index = 0) const override {
        if (index == 0) return &diff_weights_md_;
        if (index == 1 && with_bias()) return &diff_bias_md_;
        return &glob_zero_md;
    }

    int n_inputs() const override { return 2; }
    int n_outputs() const override { return 1 + with_bias(); }

protected:
    memory_desc_t src_md_;
    memory_desc_t diff_weights_md_;
    memory_desc_t diff_bias_md_;
    memory_desc_t diff_dst_md_;

    bool set_default_formats_common(format_tag_t src_tag,
            format_tag_t diff_wei_tag, format_tag_t diff_dst_tag) {
        return set_default_formats_common_template(src_md_, src_tag,
                diff_weights_md_, diff_wei_tag, diff_dst_md_, diff_dst_tag,
                diff_bias_md_);
    }
};

} // namespace impl
} // namespace zendnn

#endif

// vim: et ts=4 sw=4 cindent cino+=l0,\:4,N-s
