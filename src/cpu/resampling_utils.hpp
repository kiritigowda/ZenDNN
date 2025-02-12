﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2019-2020 Intel Corporation
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

#ifndef CPU_RESAMPLING_UTILS_HPP
#define CPU_RESAMPLING_UTILS_HPP

#include "common/c_types_map.hpp"

#include "cpu/simple_q10n.hpp"

namespace zendnn {
namespace impl {
namespace cpu {

namespace resampling_utils {

static inline float linear_map(dim_t y, dim_t y_max, dim_t x_max) {
    return ((y + 0.5f) * x_max / y_max) - 0.5f;
}
static inline dim_t nearest_idx(dim_t y, dim_t y_max, dim_t x_max) {
    return (dim_t)roundf(linear_map(y, y_max, x_max));
}
static inline dim_t ceil_idx(float x) {
    if (x < 0) return (dim_t)0;
    return (dim_t)x == x ? (dim_t)x : (dim_t)x + 1;
}
static inline float linear_weight(int i, dim_t x, dim_t y_max, dim_t x_max) {
    float s = linear_map(x, y_max, x_max);
    float w = nstl::abs(s - (dim_t)s);
    return i == 0 ? 1.f - w : w;
};

struct linear_coeffs_t {
    linear_coeffs_t(dim_t y, dim_t y_max, dim_t x_max) {
        float s = linear_map(y, y_max, x_max);
        idx[0] = left(s);
        idx[1] = right(s, x_max);
        wei[1] = nstl::abs(s - saturate<float>(idx[0]));
        wei[0] = 1.f - wei[1];
    }
    // left and right index of source image used for interpolation
    dim_t idx[2];
    // left and right interpolation weights
    float wei[2];

private:
    static dim_t right(float s, dim_t x_max) {
        return nstl::min(ceil_idx(s), x_max - 1);
    }
    static dim_t left(float s) { return nstl::max((dim_t)s, (dim_t)0); }
};

struct bwd_linear_coeffs_t {
    bwd_linear_coeffs_t(dim_t x, dim_t y_max, dim_t x_max) {
        start[0] = x == 0 ? 0 : left_start(x, y_max, x_max);
        start[1] = right_start(x, y_max, x_max);
        end[0] = left_end(x, y_max, x_max);
        end[1] = x == x_max - 1 ? y_max : right_end(x, y_max, x_max);
    }
    // index range (from start to end) of source image used as left and right
    // edge for interpolation
    dim_t start[2], end[2];

private:
    static dim_t left_start(dim_t x, dim_t y_max, dim_t x_max) {
        return ceil_idx(linear_map(x, x_max, y_max));
    }
    static dim_t left_end(dim_t x, dim_t y_max, dim_t x_max) {
        return nstl::min(ceil_idx(linear_map(x + 1, x_max, y_max)), y_max);
    }
    static dim_t right_start(dim_t x, dim_t y_max, dim_t x_max) {
        float s = linear_map(x - 1, x_max, y_max);
        return s < 0 ? 0 : (dim_t)(s) + 1;
    }
    static dim_t right_end(dim_t x, dim_t y_max, dim_t x_max) {
        float s = linear_map(x, x_max, y_max);
        return nstl::min(s < 0 ? 0 : (dim_t)s + 1, y_max);
    }
};

} // namespace resampling_utils

} // namespace cpu
} // namespace impl
} // namespace zendnn

#endif
