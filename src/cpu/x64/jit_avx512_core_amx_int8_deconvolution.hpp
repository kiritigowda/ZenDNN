﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2021 Intel Corporation
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

#ifndef CPU_X64_JIT_AVX512_CORE_AMX_INT8_CONVOLUTION_HPP
#define CPU_X64_JIT_AVX512_CORE_AMX_INT8_CONVOLUTION_HPP

#include "common/c_types_map.hpp"
#include "common/zendnn_thread.hpp"
#include "common/memory_tracking.hpp"
#include "common/primitive.hpp"
#include "common/utils.hpp"

#include "cpu/cpu_deconvolution_pd.hpp"

#include "cpu/x64/amx_tile_configure.hpp"
#include "cpu/x64/jit_avx512_core_amx_conv_kernel.hpp"

namespace zendnn {
namespace impl {
namespace cpu {
namespace x64 {

struct jit_avx512_core_amx_int8_deconvolution_fwd_t : public primitive_t {
    struct pd_t : public cpu_deconvolution_fwd_pd_t {
        pd_t(const deconvolution_desc_t *adesc, const primitive_attr_t *attr,
                const typename pd_t::base_class *hint_fwd_pd)
            : cpu_deconvolution_fwd_pd_t(adesc, attr, hint_fwd_pd), jcp_() {}

        DECLARE_COMMON_PD_T(
                JIT_IMPL_NAME_HELPER("jit_deconvolution:", jcp_.isa, ""),
                jit_avx512_core_amx_int8_deconvolution_fwd_t);

        status_t init(engine_t *engine) {
            using namespace data_type;
            using smask_t = primitive_attr_t::skip_mask_t;
            bool is_int8_deconvolution = true
                    && utils::everyone_is(true,
                            utils::one_of(src_md_.data_type, s8, u8),
                            weights_md_.data_type == s8,
                            utils::one_of(dst_md_.data_type, f32, s32, s8, u8))
                    && IMPLICATION(with_bias(),
                            utils::one_of(bias_md_.data_type, f32, s32, s8, u8))
                    && attr()->has_default_values(
                            smask_t::oscale | smask_t::post_ops);

            bool ok = is_fwd()
                    && (desc()->alg_kind & alg_kind::deconvolution_direct)
                    && is_int8_deconvolution && !has_zero_dim_memory();
            if (!ok) return status::unimplemented;

            status_t status = jit_avx512_core_amx_bwd_data_kernel_t::init_conf(
                    jcp_, *desc(), dst_md_, weights_md_, src_md_, &bias_md_,
                    *attr(), zendnn_get_max_threads());
            if (status != status::success) return status;

            auto scratchpad = scratchpad_registry().registrar();
            jit_avx512_core_amx_bwd_data_kernel_t::init_scratchpad(
                    scratchpad, jcp_, *attr());

            return status;
        }

        jit_conv_conf_t jcp_;
    };

    jit_avx512_core_amx_int8_deconvolution_fwd_t(const pd_t *apd)
        : primitive_t(apd) {}

    status_t init(engine_t *engine) override {
        CHECK(safe_ptr_assign(kernel_,
                new jit_avx512_core_amx_bwd_data_kernel_t(
                        pd()->jcp_, *pd()->attr())));
        return kernel_->create_kernel();
    }

    status_t execute(const exec_ctx_t &ctx) const override {
        const auto &_pd = pd();
        if (_pd->ndims() > 4)
            return status::unimplemented;
        else if (_pd->jcp_.is_depthwise)
            return status::unimplemented;
        else
            execute_forward(ctx);
        return status::success;
    }

private:
    void execute_forward(const exec_ctx_t &ctx) const;
    const pd_t *pd() const { return (const pd_t *)primitive_t::pd().get(); }
    void prepare_padded_bias(const char *&bias,
            const memory_tracking::grantor_t &scratchpad) const;

    std::unique_ptr<jit_avx512_core_amx_bwd_data_kernel_t> kernel_;
};

} // namespace x64
} // namespace cpu
} // namespace impl
} // namespace zendnn

#endif

// vim: et ts=4 sw=4 cindent cino^=l0,\:0,N-s
