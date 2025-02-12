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

#ifndef CPU_X64_MATMUL_BRGEMM_MATMUL_COPY_UTILS_HPP
#define CPU_X64_MATMUL_BRGEMM_MATMUL_COPY_UTILS_HPP

#include "cpu/x64/matmul/brgemm_matmul_utils.hpp"

namespace zendnn {
namespace impl {
namespace cpu {
namespace x64 {
namespace matmul {

struct jit_brgemm_matmul_copy_B_t {
    struct ctx_t {
        const void *src;
        const void *tr_src;
        const void *compensation_ptr;

        dim_t current_K_start;
        dim_t current_K_iters;
        dim_t current_N_blk;
    };

    virtual void operator()(ctx_t *ctx) = 0;
    virtual status_t create_kernel() = 0;

    jit_brgemm_matmul_copy_B_t(const brgemm_matmul_conf_t *conf)
        : conf_(conf) {}
    virtual ~jit_brgemm_matmul_copy_B_t() {}

    const brgemm_matmul_conf_t *conf_;
};

struct jit_brgemm_matmul_copy_A_t {
    struct ctx_t {
        const void *src;
        const void *tr_src;

        dim_t current_K_blk;
        dim_t current_M_blk;
    };

    virtual void operator()(ctx_t *ctx) = 0;
    virtual status_t create_kernel() = 0;

    jit_brgemm_matmul_copy_A_t(const brgemm_matmul_conf_t *conf)
        : conf_(conf) {}
    virtual ~jit_brgemm_matmul_copy_A_t() {}

    const brgemm_matmul_conf_t *conf_;
};

status_t create_brgemm_matmul_copy_B(
        std::unique_ptr<jit_brgemm_matmul_copy_B_t> &copy_ker,
        const brgemm_matmul_conf_t *conf);

status_t create_brgemm_matmul_copy_A(
        std::unique_ptr<jit_brgemm_matmul_copy_A_t> &copy_ker,
        const brgemm_matmul_conf_t *conf);

} // namespace matmul
} // namespace x64
} // namespace cpu
} // namespace impl
} // namespace zendnn

#endif
