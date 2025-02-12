﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2020 Intel Corporation
* Copyright 2020 Arm Ltd. and affiliates
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

#ifndef CPU_PLATFORM_HPP
#define CPU_PLATFORM_HPP

#include "zendnn_config.h"

#include "common/c_types_map.hpp"
#include "common/z_magic.hpp"

// Possible architectures:
// - ZENDNN_X64
// - ZENDNN_AARCH64
// - ZENDNN_PPC64
// - ZENDNN_S390X
// - ZENDNN_ARCH_GENERIC
// Target architecture macro is set to 1, others to 0. All macros are defined.

#if defined(ZENDNN_X64) + defined(ZENDNN_AARCH64) + defined(ZENDNN_PPC64) \
                + defined(ZENDNN_S390X) + defined(ZENDNN_ARCH_GENERIC) \
        == 0
#if defined(__x86_64__) || defined(_M_X64)
#define ZENDNN_X64 1
#elif defined(__aarch64__)
#define ZENDNN_AARCH64 1
#elif defined(__powerpc64__) || defined(__PPC64__) || defined(_ARCH_PPC64)
#define ZENDNN_PPC64 1
#elif defined(__s390x__)
#define ZENDNN_S390X 1
#else
#define ZENDNN_ARCH_GENERIC 1
#endif
#endif // defined(ZENDNN_X64) + ... == 0

#if defined(ZENDNN_X64) + defined(ZENDNN_AARCH64) + defined(ZENDNN_PPC64) \
                + defined(ZENDNN_S390X) + defined(ZENDNN_ARCH_GENERIC) \
        != 1
#error One and only one architecture should be defined at a time
#endif

#if !defined(ZENDNN_X64)
#define ZENDNN_X64 0
#endif
#if !defined(ZENDNN_AARCH64)
#define ZENDNN_AARCH64 0
#endif
#if !defined(ZENDNN_PPC64)
#define ZENDNN_PPC64 0
#endif
#if !defined(ZENDNN_S390X)
#define ZENDNN_S390X 0
#endif
#if !defined(ZENDNN_ARCH_GENERIC)
#define ZENDNN_ARCH_GENERIC 0
#endif

// Helper macros: expand the parameters only on the corresponding architecture.
// Equivalent to: #if ZENDNN_$ARCH ... #endif
#define ZENDNN_X64_ONLY(...) Z_CONDITIONAL_DO(ZENDNN_X64, __VA_ARGS__)
#define ZENDNN_PPC64_ONLY(...) Z_CONDITIONAL_DO(ZENDNN_PPC64_ONLY, __VA_ARGS__)
#define ZENDNN_S390X_ONLY(...) Z_CONDITIONAL_DO(ZENDNN_S390X_ONLY, __VA_ARGS__)
#define ZENDNN_AARCH64_ONLY(...) Z_CONDITIONAL_DO(ZENDNN_AARCH64, __VA_ARGS__)

// Using Arm Compute Library kernels is optional for AArch64 builds
// and can be enabled with the ZENDNN_AARCH64_USE_ACL CMake option
#if defined(ZENDNN_AARCH64) && defined(ZENDNN_AARCH64_USE_ACL)
#define ZENDNN_AARCH64_ACL_ONLY(...) __VA_ARGS__
#else
#define ZENDNN_AARCH64_ACL_ONLY(...)
#endif

namespace zendnn {
namespace impl {
namespace cpu {
namespace platform {

const char *get_isa_info();
zendnn_cpu_isa_t get_effective_cpu_isa();
status_t set_max_cpu_isa(zendnn_cpu_isa_t isa);
status_t set_cpu_isa_hints(zendnn_cpu_isa_hints_t isa_hints);
zendnn_cpu_isa_hints_t get_cpu_isa_hints();

bool ZENDNN_API prefer_ymm_requested();
bool ZENDNN_API has_data_type_support(data_type_t data_type);
float s8s8_weights_scale_factor();

unsigned get_per_core_cache_size(int level);
unsigned get_num_cores();
#if ZENDNN_CPU_THREADING_RUNTIME == ZENDNN_RUNTIME_THREADPOOL
unsigned ZENDNN_API get_max_threads_to_use();
#endif

constexpr int get_cache_line_size() {
    return 64;
}

int get_vector_register_size();

} // namespace platform

// XXX: find a better place for these values?
enum {
    PAGE_4K = 4096,
    PAGE_2M = 2097152,
};

} // namespace cpu
} // namespace impl
} // namespace zendnn

#endif
