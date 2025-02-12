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
#include "primitive.hpp"
#include "primitive_exec_types.hpp"
#include "stream.hpp"
#include "utils.hpp"
#include "zendnn_logging.hpp"

using namespace zendnn;
using namespace zendnn::impl;
using namespace zendnn::impl::status;
using namespace zendnn::impl::utils;

status_t stream_t::enqueue_primitive(
        const primitive_iface_t *primitive_iface, exec_ctx_t &ctx) {
    return primitive_iface->execute(ctx);
}

/* API */

status_t zendnn_stream_create(
        stream_t **stream, engine_t *engine, unsigned flags) {
    bool args_ok = !utils::any_null(stream, engine);
    if (!args_ok) return invalid_arguments;

    zendnnInfo(ZENDNN_CORELOG, "CPU Stream created [stream]");
    return engine->create_stream(stream, flags);
}

status_t zendnn_stream_get_engine(const stream_t *stream, engine_t **engine) {
    if (any_null(stream, engine)) return invalid_arguments;
    *engine = stream->engine();
    return success;
}

status_t zendnn_stream_wait(stream_t *stream) {
    bool args_ok = !any_null(stream);
    if (!args_ok) return invalid_arguments;

    zendnnInfo(ZENDNN_CORELOG, "Stream wait [stream]");
    return stream->wait();
}

status_t zendnn_stream_destroy(stream_t *stream) {
    zendnnInfo(ZENDNN_CORELOG, "CPU Stream deleted [stream]");
    delete stream;
    return success;
}

// vim: et ts=4 sw=4 cindent cino+=l0,\:4,N-s
