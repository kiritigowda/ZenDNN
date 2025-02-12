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

#include "primitive_cache.hpp"
#include "c_types_map.hpp"
#include "primitive_desc.hpp"
#include "rw_mutex.hpp"

#include <list>
#include <unordered_map>
#include "zendnn_logging.hpp"

namespace zendnn {
namespace impl {

primitive_cache_t &primitive_cache() {
#if ZENDNN_ENABLE_PRIMITIVE_CACHE
    static int capacity = 0;
    static std::once_flag initialized;
    std::call_once(initialized, [&] {
            capacity
            = getenv_int("ZENDNN_PRIMITIVE_CACHE_CAPACITY", 1024);
    });
#else
    static const int capacity = 0;
#endif
	zendnnInfo(ZENDNN_CORELOG, "CPU Engine: primitive_cache_capacity: ",capacity);

    static lru_primitive_cache_t cache(capacity);
    return cache;
}

// Undocumented API, for testing only
status_t get_primitive_cache_size(int *size) {
    if (size == nullptr) return zendnn::impl::status::invalid_arguments;
    *size = 0;
#if ZENDNN_ENABLE_PRIMITIVE_CACHE
    *size = primitive_cache().get_size();
#endif
    return zendnn::impl::status::success;
}

status_t lru_primitive_cache_t::set_capacity(int capacity) {
    utils::lock_write_t lock_w(rw_mutex());
    capacity_ = (size_t)capacity;
    // Check if number of entries exceeds the new capacity
    if (cache_list_.size() > capacity_) {
        // Evict excess entries
        size_t n_excess_entries = cache_list_.size() - capacity_;
        evict(n_excess_entries);
    }
    return status::success;
}

int lru_primitive_cache_t::get_capacity() const {
    utils::lock_read_t lock_r(rw_mutex());
    return (int)capacity_;
}

// For undocumented API
int lru_primitive_cache_t::get_size() const {
    utils::lock_read_t lock_r(rw_mutex());
    return (int)cache_list_.size();
}

lru_primitive_cache_t::value_t lru_primitive_cache_t::get_or_add(
        const key_t &key, const value_t &value) {
    // Cache is disabled
    lock_read();
    if (capacity_ == 0) {
        unlock_read();
        return value_t();
    }

    unlock_read();
    lock_write();

    // Double check the capacity due to possible race condition
    if (capacity_ == 0) {
        unlock_write();
        return value_t();
    }

    // Check if the requested entry is present in the cache
    auto e = get(key);
    if (!e.valid()) {
        // If the entry is missing in the cache then add it
        add(key, value);
    }
    unlock_write();
    return e;
}

void lru_primitive_cache_t::add(const key_t &key, const value_t &value) {
    // std::list::size() method has linear complexity. Check the primitive cache
    // size using std::unordered_map::size();
    if (cache_mapper_.size() == capacity_) {
        // Evict the least recently used entry
        evict(1);
    }
    // Place a new entry to cache_list_ and update cache_mapper_
    cache_list_.emplace_front(key, value);
    cache_mapper_.insert(std::make_pair(key, cache_list_.begin()));
    assert(cache_list_.size() == cache_mapper_.size());
}

lru_primitive_cache_t::value_t lru_primitive_cache_t::get(const key_t &key) {
    auto it = cache_mapper_.find(key);
    if (it == cache_mapper_.end()) { return value_t(); }

    // Move 1 cache_list_ node to the front of the cache_list_
    cache_list_.splice(cache_list_.begin(), cache_list_, it->second);
    return cache_list_.front().second;
}

void lru_primitive_cache_t::remove_if_invalidated(const key_t &key) {
    lock_write();
    auto it = cache_mapper_.find(key);
    if (it == cache_mapper_.end()) {
        // The entry has been already evicted at this point
        unlock_write();
        return;
    }

    const auto &value = it->second->second;
    if (value.get().primitive) {
        // If the entry is not invalidated
        unlock_write();
        return;
    }

    // Remove the invalidated entry
    cache_list_.erase(it->second);
    cache_mapper_.erase(it);
    assert(cache_list_.size() == cache_mapper_.size());
    unlock_write();
}

void lru_primitive_cache_t::update_entry(
        const key_t &key, const primitive_desc_t *pd) {
    utils::lock_write_t lock_w(rw_mutex());
    auto it = cache_mapper_.find(key);

    // There is nothing to do in two cases:
    // 1. The requested entry is not in the cache because it has been evicted
    //    by another thread
    // 2. After the requested entry had been evicted it was inserted again
    //    by another thread
    if (it == cache_mapper_.end() || it->first.thread_id() != key.thread_id())
        return;

    const auto *op_desc = pd->op_desc();
    const auto *attr = pd->attr();

    // Update key in cache_mapper_
    it->first.op_desc_ = op_desc;
    it->first.attr_ = attr;

    // Update key in cache_list_
    it->second->first.op_desc_ = op_desc;
    it->second->first.attr_ = attr;
}

// Evicts n the least recently used entries
void lru_primitive_cache_t::evict(size_t n) {
    for (size_t e = 0; e < n; e++) {
        cache_mapper_.erase(cache_list_.back().first);
        cache_list_.pop_back();
    }
}

} // namespace impl
} // namespace zendnn

// API
zendnn::impl::status_t zendnn_get_primitive_cache_capacity(int *capacity) {
    if (capacity == nullptr) return zendnn::impl::status::invalid_arguments;
    *capacity = 0;
#if ZENDNN_ENABLE_PRIMITIVE_CACHE
    *capacity = zendnn::impl::primitive_cache().get_capacity();
#endif
    return zendnn::impl::status::success;
}

zendnn::impl::status_t zendnn_set_primitive_cache_capacity(int capacity) {
    if (capacity < 0) return zendnn::impl::status::invalid_arguments;
#if ZENDNN_ENABLE_PRIMITIVE_CACHE
    return zendnn::impl::primitive_cache().set_capacity(capacity);
#endif
    return zendnn::impl::status::success;
}
