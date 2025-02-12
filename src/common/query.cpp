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

#include <assert.h>
#include "zendnn.h"

#include "c_types_map.hpp"
#include "engine.hpp"
#include "primitive_desc.hpp"
#include "utils.hpp"

using namespace zendnn::impl;
using namespace zendnn::impl::utils;
using namespace zendnn::impl::status;

status_t zendnn_primitive_desc_query(
        const primitive_desc_iface_t *primitive_desc_iface, query_t what,
        int index, void *result) {
    if (any_null(primitive_desc_iface, result)) return invalid_arguments;

    return primitive_desc_iface->query(what, index, result);
}

const memory_desc_t *zendnn_primitive_desc_query_md(
        const primitive_desc_iface_t *primitive_desc_iface, query_t what,
        int index) {
    const memory_desc_t *res_md = nullptr;
    bool args_ok = true && primitive_desc_iface != nullptr
            && (what & query::some_md) == query::some_md
            && what != query::some_md
            && zendnn_primitive_desc_query(
                       primitive_desc_iface, what, index, &res_md)
                    == success;
    return args_ok ? res_md : nullptr;
}

int zendnn_primitive_desc_query_s32(
        const primitive_desc_iface_t *primitive_desc_iface, query_t what,
        int index) {
    int res_s32;
    bool args_ok = primitive_desc_iface != nullptr
            && one_of(what, query::num_of_inputs_s32, query::num_of_outputs_s32)
            && zendnn_primitive_desc_query(
                       primitive_desc_iface, what, index, &res_s32)
                    == success;
    return args_ok ? res_s32 : 0;
}

// vim: et ts=4 sw=4 cindent cino+=l0,\:4,N-s
