﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2018-2021 Intel Corporation
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

#ifndef CPU_REF_SHUFFLE_HPP
#define CPU_REF_SHUFFLE_HPP

#include <assert.h>

#include "common/c_types_map.hpp"
#include "common/zendnn_thread.hpp"
#include "common/primitive.hpp"
#include "common/type_helpers.hpp"
#include "common/utils.hpp"

#include "cpu/platform.hpp"

#include "cpu/cpu_shuffle_pd.hpp"

namespace zendnn {
namespace impl {
namespace cpu {

struct ref_shuffle_t : public primitive_t {
    struct pd_t : public cpu_shuffle_pd_t {
        using cpu_shuffle_pd_t::cpu_shuffle_pd_t;

        DECLARE_COMMON_PD_T("ref:any", ref_shuffle_t);

        status_t init(engine_t *engine) {
            using namespace format_tag;

            const data_type_t data_type = data_md()->data_type;
            bool ok = platform::has_data_type_support(data_type)
                    && attr()->has_default_values()
                    && IMPLICATION(!is_fwd(), set_default_formats_common());
            if (!ok) return status::unimplemented;

            if (ndims() == 5) {
                dat_tag_ = memory_desc_matches_one_of_tag(
                        *data_md(), nCdhw16c, nCdhw8c, nCdhw4c, ncdhw, ndhwc);
            } else if (ndims() == 4) {
                dat_tag_ = memory_desc_matches_one_of_tag(
                        *data_md(), nChw16c, nChw8c, nChw4c, nchw, nhwc);
            } else
                dat_tag_ = any;

            return status::success;
        }

        format_tag_t dat_tag_;
    };

    ref_shuffle_t(const pd_t *apd) : primitive_t(apd) {}

    status_t init(engine_t *engine) override {
        const int axis_size = pd()->axis_size();
        const int group_size = pd()->group_size();
        const int transpose_row
                = pd()->is_fwd() ? group_size : axis_size / group_size;
        const int transpose_col
                = pd()->is_fwd() ? axis_size / group_size : group_size;
        rev_transposed_ = (int *)malloc(
                axis_size * sizeof(int), platform::get_cache_line_size());
        if (rev_transposed_ == nullptr) return zendnn_out_of_memory;
        parallel_nd(transpose_col, transpose_row, [&](int i, int j) {
            rev_transposed_[j * transpose_col + i] = i * transpose_row + j;
        });
        return zendnn_success;
    }

    ~ref_shuffle_t() { free(rev_transposed_); }

    status_t execute(const exec_ctx_t &ctx) const override {
        const data_type_t data_type = pd()->data_md()->data_type;
        switch (types::data_type_size(data_type)) {
            case sizeof(float): return execute_<sizeof(float)>(ctx); break;
            case sizeof(bfloat16_t):
                return execute_<sizeof(bfloat16_t)>(ctx);
                break;
            case sizeof(int8_t): return execute_<sizeof(int8_t)>(ctx); break;
            default: assert(!"unsupported data type size");
        }
        return status::success;
    }

private:
    template <int data_type_size>
    status_t execute_(const exec_ctx_t &ctx) const;
    const pd_t *pd() const { return (const pd_t *)primitive_t::pd().get(); }
    int *rev_transposed_ = nullptr;
};

} // namespace cpu
} // namespace impl
} // namespace zendnn

#endif

// vim: et ts=4 sw=4 cindent cino+=l0,\:4,N-s
