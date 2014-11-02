// Copyright (c) 2011 Elysia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#ifndef ELYSIA_CORE_UTIL_BASE64_HPP__
#define ELYSIA_CORE_UTIL_BASE64_HPP__

#include <sirikata/core/util/Platform.hpp>

namespace Elysia {
namespace Base64 {

ELYSIA_FUNCTION_EXPORT String encode(const String& orig, bool padWithEquals);
ELYSIA_FUNCTION_EXPORT String encodeURL(const String& orig);
ELYSIA_FUNCTION_EXPORT String decode(const String& orig);
ELYSIA_FUNCTION_EXPORT String decodeURL(const String& orig);

} // namespace Base64
} // Elysia

#endif // ELYSIA_CORE_UTIL_BASE64_HPP__
