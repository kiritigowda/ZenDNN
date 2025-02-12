﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2020-2021 Intel Corporation
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

#include "common/memory_storage.hpp"
#include "common/memory.hpp"

#include <assert.h>

namespace zendnn {
namespace impl {

status_t memory_storage_t::init(unsigned flags, size_t size, void *handle) {
    assert(flags & (memory_flags_t::alloc | memory_flags_t::use_runtime_ptr));
    if (flags & memory_flags_t::alloc) {
        assert(!handle);
        if (size == 0) return status::success;
        return init_allocate(size);
    }

    assert(flags & memory_flags_t::use_runtime_ptr);
    return set_data_handle(handle);
}

} // namespace impl
} // namespace zendnn
