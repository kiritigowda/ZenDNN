﻿/*******************************************************************************
* Modifications Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
* Notified per clause 4(b) of the license.
*******************************************************************************/

/*******************************************************************************
* Copyright 2018-2021 Intel Corporation
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

#ifdef _WIN32
#include <malloc.h>
#include <windows.h>
#endif

#if defined __linux__ || defined __APPLE__ || defined __FreeBSD__
#include <unistd.h>
#endif

#ifdef __linux__
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <string>

#include "zendnn.h"

#include "memory_debug.hpp"
#include "utils.hpp"

#include "cpu/platform.hpp"

namespace zendnn {
namespace impl {

int getenv(const char *name, char *buffer, int buffer_size) {
    if (name == nullptr || buffer_size < 0
            || (buffer == nullptr && buffer_size > 0))
        return INT_MIN;

    int result = 0;
    int term_zero_idx = 0;
    size_t value_length = 0;

#ifdef _WIN32
    value_length = GetEnvironmentVariable(name, buffer, buffer_size);
#else
    const char *value = ::getenv(name);
    value_length = value == nullptr ? 0 : strlen(value);
#endif

    if (value_length > INT_MAX)
        result = INT_MIN;
    else {
        int int_value_length = (int)value_length;
        if (int_value_length >= buffer_size) {
            result = -int_value_length;
        } else {
            term_zero_idx = int_value_length;
            result = int_value_length;
#ifndef _WIN32
            if (value) strncpy(buffer, value, buffer_size - 1);
#endif
        }
    }

    if (buffer != nullptr) buffer[term_zero_idx] = '\0';
    return result;
}

int getenv_int(const char *name, int default_value) {
    int value = default_value;
    // # of digits in the longest 32-bit signed int + sign + terminating null
    const int len = 12;
    char value_str[len];
    if (getenv(name, value_str, len) > 0) value = atoi(value_str);
    return value;
}

FILE *fopen(const char *filename, const char *mode) {
#ifdef _WIN32
    FILE *fp = NULL;
    return ::fopen_s(&fp, filename, mode) ? NULL : fp;
#else
    return ::fopen(filename, mode);
#endif
}

int getpagesize() {
#ifdef _WIN32
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwPageSize;
#else
    return ::getpagesize();
#endif
}

void *malloc(size_t size, int alignment) {
    void *ptr;
    if (memory_debug::is_mem_debug())
        return memory_debug::malloc(size, alignment);

#ifdef _WIN32
    ptr = _aligned_malloc(size, alignment);
    int rc = ptr ? 0 : -1;
#else
    int rc = ::posix_memalign(&ptr, alignment, size);
#endif

    return (rc == 0) ? ptr : nullptr;
}

void free(void *p) {

    if (memory_debug::is_mem_debug()) return memory_debug::free(p);

#ifdef _WIN32
    _aligned_free(p);
#else
    ::free(p);
#endif
}

// Atomic operations
int32_t fetch_and_add(int32_t *dst, int32_t val) {
#ifdef _WIN32
    return InterlockedExchangeAdd(reinterpret_cast<long *>(dst), val);
#else
    return __sync_fetch_and_add(dst, val);
#endif
}

static setting_t<bool> jit_dump {false};
bool get_jit_dump() {
    if (!jit_dump.initialized()) {
        jit_dump.set(!!getenv_int("MKLDNN_JIT_DUMP", 0));
        jit_dump.set(!!getenv_int("ZENDNN_JIT_DUMP", jit_dump.get()));
    }
    return jit_dump.get();
}

#if ZENDNN_AARCH64
static setting_t<unsigned> jit_profiling_flags {ZENDNN_JIT_PROFILE_LINUX_PERFMAP};
#else
static setting_t<unsigned> jit_profiling_flags {ZENDNN_JIT_PROFILE_VTUNE};
#endif
unsigned get_jit_profiling_flags() {
    if (!jit_profiling_flags.initialized()) {
        jit_profiling_flags.set(
                getenv_int("ZENDNN_JIT_PROFILE", jit_profiling_flags.get()));
    }
    return jit_profiling_flags.get();
}

static setting_t<std::string> jit_profiling_jitdumpdir;
zendnn_status_t init_jit_profiling_jitdumpdir(
        const char *jitdumpdir, bool overwrite) {
#ifdef __linux__
    static std::mutex m;
    std::lock_guard<std::mutex> g(m);

    if (jit_profiling_jitdumpdir.initialized() && !overwrite)
        return status::success;

    if (!jitdumpdir) {
        char buf[PATH_MAX];
        if (getenv("JITDUMPDIR", buf, sizeof(buf)) > 0)
            jit_profiling_jitdumpdir.set(buf);
        else if (getenv("HOME", buf, sizeof(buf)) > 0)
            jit_profiling_jitdumpdir.set(buf);
        else
            jit_profiling_jitdumpdir.set(".");
    } else
        jit_profiling_jitdumpdir.set(jitdumpdir);

    return status::success;
#else
    return status::unimplemented;
#endif
}

std::string get_jit_profiling_jitdumpdir() {
    if (!jit_profiling_jitdumpdir.initialized())
        init_jit_profiling_jitdumpdir(nullptr, false);
    return jit_profiling_jitdumpdir.get();
}

} // namespace impl
} // namespace zendnn

zendnn_status_t zendnn_set_jit_dump(int enabled) {
    using namespace zendnn::impl;
    jit_dump.set(enabled);
    return status::success;
}

zendnn_status_t zendnn_set_jit_profiling_flags(unsigned flags) {
    using namespace zendnn::impl;
    unsigned mask = ZENDNN_JIT_PROFILE_VTUNE;
#ifdef __linux__
    mask |= ZENDNN_JIT_PROFILE_LINUX_PERF;
    mask |= ZENDNN_JIT_PROFILE_LINUX_JITDUMP_USE_TSC;
#endif
    if (flags & ~mask) return status::invalid_arguments;
    jit_profiling_flags.set(flags);
    return status::success;
}

zendnn_status_t zendnn_set_jit_profiling_jitdumpdir(const char *dir) {
    return zendnn::impl::init_jit_profiling_jitdumpdir(dir, true);
}

zendnn_status_t zendnn_set_max_cpu_isa(zendnn_cpu_isa_t isa) {
    return zendnn::impl::cpu::platform::set_max_cpu_isa(isa);
}

zendnn_cpu_isa_t zendnn_get_effective_cpu_isa() {
    return zendnn::impl::cpu::platform::get_effective_cpu_isa();
}

zendnn_status_t zendnn_set_cpu_isa_hints(zendnn_cpu_isa_hints_t isa_hints) {
    return zendnn::impl::cpu::platform::set_cpu_isa_hints(isa_hints);
}

zendnn_cpu_isa_hints_t zendnn_get_cpu_isa_hints() {
    return zendnn::impl::cpu::platform::get_cpu_isa_hints();
}

#if ZENDNN_CPU_THREADING_RUNTIME == ZENDNN_RUNTIME_THREADPOOL
#include "zendnn_threadpool_iface.hpp"
namespace zendnn {
namespace impl {
namespace threadpool_utils {

namespace {
static thread_local zendnn::threadpool_interop::threadpool_iface
        *active_threadpool
        = nullptr;
}

void ZENDNN_API activate_threadpool(
        zendnn::threadpool_interop::threadpool_iface *tp) {
    assert(!active_threadpool);
    if (!active_threadpool) active_threadpool = tp;
}

void ZENDNN_API deactivate_threadpool() {
    active_threadpool = nullptr;
}

zendnn::threadpool_interop::threadpool_iface *get_active_threadpool() {
    return active_threadpool;
}

} // namespace threadpool_utils
} // namespace impl
} // namespace zendnn
#endif
