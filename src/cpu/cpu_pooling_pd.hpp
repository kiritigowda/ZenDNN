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

#ifndef CPU_CPU_POOLING_PD_HPP
#define CPU_CPU_POOLING_PD_HPP

#include "common/pooling_pd.hpp"
#include "cpu/cpu_engine.hpp"

namespace zendnn {
namespace impl {
namespace cpu {

struct cpu_pooling_fwd_pd_t : public pooling_fwd_pd_t {
    using pooling_fwd_pd_t::pooling_fwd_pd_t;
};

struct cpu_pooling_bwd_pd_t : public pooling_bwd_pd_t {
    using pooling_bwd_pd_t::pooling_bwd_pd_t;
};

} // namespace cpu
} // namespace impl
} // namespace zendnn

#endif

// vim: et ts=4 sw=4 cindent cino+=l0,\:4,N-s
