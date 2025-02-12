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

#ifndef COMMON_BFLOAT16_HPP
#define COMMON_BFLOAT16_HPP

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <type_traits>

#include "common/bit_cast.hpp"

#include "zendnn.h"

namespace zendnn {
namespace impl {

struct bfloat16_t {
    uint16_t raw_bits_;
    bfloat16_t() = default;
    constexpr bfloat16_t(uint16_t r, bool) : raw_bits_(r) {}
    bfloat16_t(float f) { (*this) = f; }

    template <typename IntegerType,
            typename SFINAE = typename std::enable_if<
                    std::is_integral<IntegerType>::value>::type>
    bfloat16_t(const IntegerType i)
        : raw_bits_ {convert_bits_of_normal_or_zero(
                utils::bit_cast<uint32_t>(static_cast<float>(i)))} {}

    bfloat16_t ZENDNN_API &operator=(float f);

    template <typename IntegerType,
            typename SFINAE = typename std::enable_if<
                    std::is_integral<IntegerType>::value>::type>
    bfloat16_t &operator=(const IntegerType i) {
        // Call the converting constructor that is optimized for integer types,
        // followed by the fast defaulted move-assignment operator.
        return (*this) = bfloat16_t {i};
    }

    ZENDNN_API operator float() const;

    bfloat16_t &operator+=(const float a) {
        (*this) = float {*this} + a;
        return *this;
    }

private:
    // Converts the 32 bits of a normal float or zero to the bits of a bfloat16.
    static constexpr uint16_t convert_bits_of_normal_or_zero(
            const uint32_t bits) {
        return uint32_t {
                       bits + uint32_t {0x7FFFU + (uint32_t {bits >> 16} & 1U)}}
        >> 16;
    }
};

static_assert(sizeof(bfloat16_t) == 2, "bfloat16_t must be 2 bytes");

void cvt_float_to_bfloat16(bfloat16_t *out, const float *inp, size_t nelems);
void cvt_bfloat16_to_float(float *out, const bfloat16_t *inp, size_t nelems);

// performs element-by-element sum of inp and add float arrays and stores
// result to bfloat16 out array with downconversion
// out[:] = (bfloat16_t)(inp0[:] + inp1[:])
void add_floats_and_cvt_to_bfloat16(
        bfloat16_t *out, const float *inp0, const float *inp1, size_t nelems);

} // namespace impl
} // namespace zendnn

#endif
