﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2019-2021 Intel Corporation
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

#ifndef CPU_MATMUL_GEMM_X8S8S32X_MATMUL_HPP
#define CPU_MATMUL_GEMM_X8S8S32X_MATMUL_HPP

#include <assert.h>

#include <memory>

#include "common/c_types_map.hpp"
#include "common/primitive.hpp"
#include "common/type_helpers.hpp"

#include "cpu/gemm_inner_product_utils.hpp"

#include "cpu/matmul/cpu_matmul_pd.hpp"
#include "cpu/matmul/gemm_based_common.hpp"

namespace zendnn {
namespace impl {
namespace cpu {
namespace matmul {

template <data_type_t src_type, data_type_t weights_type, data_type_t dst_type>
struct gemm_x8s8s32x_matmul_t : public primitive_t {
    struct pd_t : public cpu_matmul_pd_t {
        using cpu_matmul_pd_t::cpu_matmul_pd_t;

        DECLARE_COMMON_PD_T("gemm:jit", gemm_x8s8s32x_matmul_t);

        status_t init(engine_t *engine);
        const gemm_based::params_t &params() const { return params_; }

    private:
        gemm_based::params_t params_;
    };

    gemm_x8s8s32x_matmul_t(const pd_t *apd) : primitive_t(apd) {}

    status_t init(engine_t *engine) override {
        if (pd()->params().has_pp_kernel_) {
            const bool has_runtime_dims
                    = memory_desc_wrapper(pd()->dst_md()).has_runtime_dims();
            const int nthr = zendnn_get_max_threads();
            const dim_t batch = pd()->batch();
            const dim_t M = pd()->M();

            // mb value is calculated based on work-sharing using
            // balance211 in execute()
            dim_t mb = ZENDNN_RUNTIME_DIM_VAL;
            if (!has_runtime_dims && ((batch * M) % nthr == 0)) {
                const dim_t m_per_thr = nstl::max<dim_t>(1, (batch * M) / nthr);
                if (m_per_thr >= M && m_per_thr % M == 0) {
                    mb = M;
                } else if (m_per_thr < M && M % m_per_thr == 0) {
                    mb = m_per_thr;
                }
            }

            pp_kernel_.reset(pp_kernel_t::create(pd()->N(), mb, pd()->ldc(),
                    &pd()->params().pp_attr_, pd()->desc()->bias_desc.data_type,
                    pd()->dst_md(), false));
            return pp_kernel_->create_kernel();
        }
        return status::success;
    }

    static constexpr data_type_t acc_type = data_type::s32;

    typedef typename prec_traits<src_type>::type src_data_t;
    typedef typename prec_traits<weights_type>::type weights_data_t;
    typedef typename prec_traits<dst_type>::type dst_data_t;
    typedef typename prec_traits<acc_type>::type acc_data_t;

    status_t execute(const exec_ctx_t &ctx) const override {
        return execute_ref(ctx);
    }

private:
    const pd_t *pd() const { return (const pd_t *)primitive_t::pd().get(); }
    status_t execute_ref(const exec_ctx_t &ctx) const;
    void post_process_src_and_weights_zero_points(
            std::vector<acc_data_t> &src_comp,
            std::vector<acc_data_t> &wei_comp, dim_t M, dim_t N, dim_t K,
            const src_data_t *src, dim_t src_s0, dim_t src_s1,
            const weights_data_t *wei, dim_t wei_s0, dim_t wei_s1,
            acc_data_t *acc, int ldc, acc_data_t src_zero_point,
            acc_data_t wei_zero_point) const;

    using pp_kernel_t = inner_product_utils::pp_kernel_t<acc_type, dst_type>;
    std::unique_ptr<pp_kernel_t> pp_kernel_;
};

} // namespace matmul
} // namespace cpu
} // namespace impl
} // namespace zendnn

#endif
