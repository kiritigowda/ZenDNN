﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2018-2020 Intel Corporation
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
#include "concat_pd.hpp"
#include "engine.hpp"
#include "type_helpers.hpp"
#include "utils.hpp"

using namespace zendnn::impl;
using namespace zendnn::impl::utils;
using namespace zendnn::impl::status;

status_t zendnn_concat_primitive_desc_create(
        primitive_desc_iface_t **concat_pd_iface, const memory_desc_t *dst_md,
        int n, int concat_dim, const memory_desc_t *src_mds,
        const primitive_attr_t *attr, engine_t *engine) {
    bool args_ok = !any_null(concat_pd_iface, src_mds) && n > 0;
    if (!args_ok) return invalid_arguments;

    if (attr == nullptr) attr = &default_attr();

    const int ndims = src_mds[0].ndims;
    const dims_t &dims = src_mds[0].dims;
    const data_type_t dt = src_mds[0].data_type;
    if (memory_desc_wrapper(src_mds[0]).has_runtime_dims_or_strides())
        return unimplemented;

    int concat_dim_sz = dims[concat_dim];
    for (int i = 1; i < n; ++i) {
        if (src_mds[i].ndims != ndims) return invalid_arguments;
        if (memory_desc_wrapper(src_mds[i]).has_runtime_dims_or_strides())
            return unimplemented;

        for (int d = 0; d < ndims; ++d) {
            if (d == concat_dim) continue;
            if (src_mds[i].dims[d] != dims[d]) return invalid_arguments;
        }
        if (src_mds[i].data_type != dt) return invalid_arguments;
        concat_dim_sz += src_mds[i].dims[concat_dim];
    }

    memory_desc_t dummy_dst_md;
    if (dst_md) {
        if (dst_md->ndims != ndims) return invalid_arguments;
        if (memory_desc_wrapper(dst_md).has_runtime_dims_or_strides())
            return unimplemented;
        for (int d = 0; d < ndims; ++d) {
            if (dst_md->dims[d] != (d == concat_dim ? concat_dim_sz : dims[d]))
                return invalid_arguments;
        }
    } else {
        dummy_dst_md = src_mds[0];
        dummy_dst_md.dims[concat_dim] = concat_dim_sz;
        dummy_dst_md.format_kind = format_kind::any;
        dst_md = &dummy_dst_md;
    }

    concat_pd_t *concat_pd = nullptr;
    for (auto c = engine->get_concat_implementation_list(); *c; ++c) {
        if ((*c)(&concat_pd, engine, attr, dst_md, n, concat_dim, src_mds)
                == success) {
            auto status = safe_ptr_assign(*concat_pd_iface,
                    new primitive_desc_iface_t(concat_pd, engine));
            if (status != status::success) delete concat_pd;
            return status;
        }
    }
    return unimplemented;
}
