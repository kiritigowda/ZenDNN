﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2020 Intel Corporation
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

#include "cpu/reorder/cpu_reorder.hpp"

namespace zendnn {
namespace impl {
namespace cpu {

// clang-format off

const impl_list_map_t regular_s32_impl_list_map {
    // s32 ->
    {{s32, data_type::undef, 0}, {
        REG_FAST_DIRECT_COPY_COMMA(s32, f32)
        REG_FAST_DIRECT_COPY_COMMA(s32, s32)
        REG_FAST_DIRECT_COPY_COMMA(s32, s8)
        REG_FAST_DIRECT_COPY_COMMA(s32, u8)

        ZENDNN_X64_ONLY(x64::jit_uni_reorder_create,)
        ZENDNN_AARCH64_ONLY(aarch64::jit_uni_reorder_create,)

        REG_SR_BIDIR(s32, any, f32, nChw16c),
        REG_SR_BIDIR(s32, any, s32, nChw16c),
        REG_SR_BIDIR(s32, any, s8, nChw16c),
        REG_SR_BIDIR(s32, any, u8, nChw16c),

        REG_SR(s32, any, f32, any, fmt_order::any, spec::reference),
        REG_SR(s32, any, s32, any, fmt_order::any, spec::reference),
        REG_SR(s32, any, s8, any, fmt_order::any, spec::reference),
        REG_SR(s32, any, u8, any, fmt_order::any, spec::reference),

        nullptr,
    }},
};

// clang-format on

} // namespace cpu
} // namespace impl
} // namespace zendnn
