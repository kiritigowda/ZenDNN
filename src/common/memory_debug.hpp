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

#ifndef COMMON_MEMORY_DEBUG_HPP
#define COMMON_MEMORY_DEBUG_HPP

#ifndef ZENDNN_MEM_DEBUG_PROTECT_SIZE
#define ZENDNN_MEM_DEBUG_PROTECT_SIZE 1
#endif

#define ZENDNN_MEM_DEBUG_UNDERFLOW 0
#define ZENDNN_MEM_DEBUG_OVERFLOW 1

#include <stddef.h>

#include "c_types_map.hpp"
#include "zendnn_config.h"

namespace zendnn {
namespace impl {
namespace memory_debug {

// Static inline for optimization purposes when memory_debug is disabled
static inline bool is_mem_debug() {
#ifndef ZENDNN_ENABLE_MEM_DEBUG
    return false;
#else
    return true;
#endif
}

// Static inline for optimization purposes when memory_debug is disabled
static inline bool is_mem_debug_overflow() {
    if (is_mem_debug())
#if (ZENDNN_ENABLE_MEM_DEBUG == ZENDNN_MEM_DEBUG_UNDERFLOW)
        return false;
#else
        // Default to ZENDNN_MEM_DEBUG_OVERFLOW as buffer overflows are a
        // more common memory error.
        return true;
#endif
    else
        return false;
}

// Export the memory_debug::malloc symbol to for external linkage.
#ifdef ZENDNN_ENABLE_MEM_DEBUG
void ZENDNN_API *malloc(size_t size, int alignment);
#else
void *malloc(size_t size, int alignment);
#endif
void free(void *p);

size_t protect_size();
void protect_buffer(void *addr, size_t size, engine_kind_t engine_kind);
void unprotect_buffer(const void *addr, size_t size, engine_kind_t engine_kind);

} // namespace memory_debug
} // namespace impl
} // namespace zendnn
#endif

// vim: et ts=4 sw=4 cindent cino+=l0,\:4,N-s
