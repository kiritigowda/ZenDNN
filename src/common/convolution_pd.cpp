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

#include "utils.hpp"

#include "convolution_pd.hpp"

namespace zendnn {
namespace impl {

using namespace prop_kind;

memory_desc_t *conv_prop_invariant_src_d(convolution_desc_t *desc) {
    return desc->prop_kind == backward_data ? &desc->diff_src_desc
                                            : &desc->src_desc;
}

memory_desc_t *conv_prop_invariant_wei_d(convolution_desc_t *desc) {
    return desc->prop_kind == backward_weights ? &desc->diff_weights_desc
                                               : &desc->weights_desc;
}

memory_desc_t *conv_prop_invariant_bia_d(convolution_desc_t *desc) {
    return desc->prop_kind == backward_weights ? &desc->diff_bias_desc
                                               : &desc->bias_desc;
}

memory_desc_t *conv_prop_invariant_dst_d(convolution_desc_t *desc) {
    return utils::one_of(desc->prop_kind, forward_inference, forward_training)
            ? &desc->dst_desc
            : &desc->diff_dst_desc;
}

memory_desc_t *conv_prop_invariant_batchNormScale_d(convolution_desc_t *desc) {
    return &desc->batchNormScale_desc;
}

memory_desc_t *conv_prop_invariant_batchNormMean_d(convolution_desc_t *desc) {
    return &desc->batchNormMean_desc;
}

memory_desc_t *conv_prop_invariant_batchNormOffset_d(convolution_desc_t *desc) {
    return &desc->batchNormMean_desc;
}

const memory_desc_t *conv_prop_invariant_src_d(const convolution_desc_t *desc) {
    return conv_prop_invariant_src_d(const_cast<convolution_desc_t *>(desc));
}
const memory_desc_t *conv_prop_invariant_wei_d(const convolution_desc_t *desc) {
    return conv_prop_invariant_wei_d(const_cast<convolution_desc_t *>(desc));
}
const memory_desc_t *conv_prop_invariant_bia_d(const convolution_desc_t *desc) {
    return conv_prop_invariant_bia_d(const_cast<convolution_desc_t *>(desc));
}
const memory_desc_t *conv_prop_invariant_dst_d(const convolution_desc_t *desc) {
    return conv_prop_invariant_dst_d(const_cast<convolution_desc_t *>(desc));
}

const memory_desc_t *conv_prop_invariant_batchNormScale_d(const convolution_desc_t *desc) {
    return conv_prop_invariant_batchNormScale_d(const_cast<convolution_desc_t *>(desc));
}
const memory_desc_t *conv_prop_invariant_batchNormMean_d(const convolution_desc_t *desc) {
    return conv_prop_invariant_batchNormMean_d(const_cast<convolution_desc_t *>(desc));
}
const memory_desc_t *conv_prop_invariant_batchNormOffset_d(const convolution_desc_t *desc) {
    return conv_prop_invariant_batchNormOffset_d(const_cast<convolution_desc_t *>(desc));
}

} // namespace impl
} // namespace zendnn
