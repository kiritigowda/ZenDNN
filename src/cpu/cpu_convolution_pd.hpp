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

#ifndef CPU_CPU_CONVOLUTION_PD_HPP
#define CPU_CPU_CONVOLUTION_PD_HPP

#include <assert.h>

#include "common/c_types_map.hpp"
#include "common/convolution_pd.hpp"
#include "common/type_helpers.hpp"
#include "common/utils.hpp"
#include "cpu/cpu_eltwise_pd.hpp"
#include "cpu/cpu_engine.hpp"

namespace zendnn {
namespace impl {
namespace cpu {

struct cpu_convolution_fwd_pd_t : public convolution_fwd_pd_t {
    using convolution_fwd_pd_t::convolution_fwd_pd_t;

    bool has_padded_dst() const {
        memory_desc_wrapper dst_d(&dst_md_);
        return OC() != dst_d.padded_dims()[1];
    }

    bool wants_padded_bias() const {
        if (!with_bias()) return false;
        return has_padded_dst();
    }

    bool wants_zero_pad_dst() const {
        if (!has_padded_dst()) return false;
        const auto &po = attr()->post_ops_;
        int idx = po.find(primitive_kind::eltwise);
        if (idx == -1) return false;
        const auto &ee = po.entry_[idx].eltwise;
        return !cpu_eltwise_fwd_pd_t::eltwise_preserves_zero(
                ee.alg, ee.alpha, ee.beta);
    }
};

struct cpu_convolution_bwd_data_pd_t : public convolution_bwd_data_pd_t {
    using convolution_bwd_data_pd_t::convolution_bwd_data_pd_t;
};

struct cpu_convolution_bwd_weights_pd_t : public convolution_bwd_weights_pd_t {
    using convolution_bwd_weights_pd_t::convolution_bwd_weights_pd_t;

    bool wants_padded_bias() const {
        if (!with_bias()) return false;
        memory_desc_wrapper diff_dst_d(&diff_dst_md_);
        return OC() != diff_dst_d.padded_dims()[1];
    }
};

} // namespace cpu
} // namespace impl
} // namespace zendnn

#endif

// vim: et ts=4 sw=4 cindent cino+=l0,\:4,N-s
