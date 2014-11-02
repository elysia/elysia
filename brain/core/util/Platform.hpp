/*  Elysia -- Platform Dependent Definitions
 *  Platform.hpp
 *
 *  Copyright (c) 2009, Ewen Cheslack-Postava and Daniel Reiter Horn
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Elysia nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _ELYSIA_PLATFORM_HPP_
#define _ELYSIA_PLATFORM_HPP_


#define ELYSIA_PLATFORM_WINDOWS 0
#define ELYSIA_PLATFORM_LINUX   1
#define ELYSIA_PLATFORM_MAC     2


#if defined(__WIN32__) || defined(_WIN32)
#  define ELYSIA_PLATFORM ELYSIA_PLATFORM_WINDOWS
#elif defined(__APPLE_CC__) || defined(__APPLE__)
#  define ELYSIA_PLATFORM ELYSIA_PLATFORM_MAC
#  ifndef __MACOSX__
#    define __MACOSX__
#  endif
#else
#  define ELYSIA_PLATFORM ELYSIA_PLATFORM_LINUX
#endif

#ifdef ELYSIA_DEBUG_BUILD
#define ELYSIA_DEBUG 1
#else
#define ELYSIA_DEBUG 0
#endif

#ifndef ELYSIA_EXPORT
# if ELYSIA_PLATFORM == ELYSIA_PLATFORM_WINDOWS
#   if defined(STATIC_LINKED)
#     define ELYSIA_EXPORT
#   else
#     if defined(ELYSIA_BUILD)
#       define ELYSIA_EXPORT __declspec(dllexport)
#       define ELYSIA_EXPORT_TEMPLATE
#     else
#       define ELYSIA_EXPORT __declspec(dllimport)
#       define ELYSIA_EXPORT_TEMPLATE extern
#     endif
#   endif
#   define ELYSIA_PLUGIN_EXPORT __declspec(dllexport)
# else
#   if defined(__GNUC__) && __GNUC__ >= 4
#     define ELYSIA_EXPORT __attribute__ ((visibility("default")))
#     define ELYSIA_PLUGIN_EXPORT __attribute__ ((visibility("default")))
#   else
#     define ELYSIA_EXPORT
#     define ELYSIA_PLUGIN_EXPORT
#   endif
#   define ELYSIA_EXPORT_TEMPLATE
# endif
#endif


#ifndef ELYSIA_FUNCTION_EXPORT
# if ELYSIA_PLATFORM == ELYSIA_PLATFORM_WINDOWS
#   if defined(STATIC_LINKED)
#     define ELYSIA_FUNCTION_EXPORT
#   else
#     if defined(ELYSIA_BUILD)
#       define ELYSIA_FUNCTION_EXPORT __declspec(dllexport)
#     else
#       define ELYSIA_FUNCTION_EXPORT __declspec(dllimport)
#     endif
#   endif
# else
#   if defined(__GNUC__) && __GNUC__ >= 4
#     define ELYSIA_FUNCTION_EXPORT __attribute__ ((visibility("default")))
#   else
#     define ELYSIA_FUNCTION_EXPORT
#   endif
# endif
#endif



#ifndef ELYSIA_EXPORT_C
# define ELYSIA_EXPORT_C extern "C" ELYSIA_EXPORT
#endif

#ifndef ELYSIA_PLUGIN_EXPORT_C
# define ELYSIA_PLUGIN_EXPORT_C extern "C" ELYSIA_PLUGIN_EXPORT
#endif


// Where supported, marking a function with WARN_UNUSED will cause the compiler
// to complain if a caller ignores the return value.
#ifndef WARN_UNUSED
# if defined(__GNUC__) && __GNUC__ >= 4  // This is conservative, maybe be available with earlier versions
#  define WARN_UNUSED __attribute__((warn_unused_result))
# else
#  define WARN_UNUSED
# endif
#endif


#ifdef __GLIBC__
# include <endian.h>
# define ELYSIA_LITTLE_ENDIAN __LITTLE_ENDIAN
# define ELYSIA_BIG_ENDIAN    __BIG_ENDIAN
# define ELYSIA_BYTE_ORDER    __BYTE_ORDER
#elif defined(__APPLE__) || defined(MACOSX) || defined(BSD) || defined(__FreeBSD__)
# include<machine/endian.h>
# ifdef BYTE_ORDER
#  define ELYSIA_LITTLE_ENDIAN LITTLE_ENDIAN
#  define ELYSIA_BIG_ENDIAN    BIG_ENDIAN
#  define ELYSIA_BYTE_ORDER    BYTE_ORDER
# else
#  error "MACINTOSH DOES NOT DEFINE ENDIANNESS"
# endif
#else
# define ELYSIA_LITTLE_ENDIAN 1234
# define ELYSIA_BIG_ENDIAN    4321
# ifdef _BIG_ENDIAN
#  define ELYSIA_BYTE_ORDER ELYSIA_BIG_ENDIAN
#  ifdef _LITTLE_ENDIAN
#   error "BOTH little and big endian defined"
#  endif
# else
#  ifdef _LITTLE_ENDIAN
#   define ELYSIA_BYTE_ORDER ELYSIA_LITTLE_ENDIAN
#  elif defined(__sparc) || defined(__sparc__) \
   ||   defined(_POWER) || defined(__powerpc__) \
   ||   defined(__ppc__) || defined(__hpux) \
   ||   defined(_MIPSEB) || defined(_POWER) \
   ||   defined(__s390__)
#   define ELYSIA_BYTE_ORDER ELYSIA_BIG_ENDIAN
#  elif defined(__i386__) || defined(__alpha__) \
   ||   defined(__ia64) || defined(__ia64__) \
   ||   defined(_M_IX86) || defined(_M_IA64) \
   ||   defined(_M_ALPHA) || defined(__amd64) \
   ||   defined(__amd64__) || defined(_M_AMD64) \
   ||   defined(__x86_64) || defined(__x86_64__) \
   ||   defined(_M_X64)
#   define ELYSIA_BYTE_ORDER ELYSIA_LITTLE_ENDIAN
#  else
#   error "Not a known CPU type"
#  endif
# endif
#endif

#if ELYSIA_PLATFORM == ELYSIA_PLATFORM_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
//need to get rid of GetMessage for protocol buffer compatibility
#undef GetMessage
#endif
#ifndef _WIN32
#include <stdint.h>
#endif
#include <assert.h>

#ifdef __cplusplus

#include <cstddef>
#include <cstring>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <set>
#include <map>
#include <algorithm>


#include <memory>
#include <array>
#include <functional>
#include <unordered_set>
#include <unordered_map>

namespace Elysia {

// numeric typedefs to get standardized types
typedef unsigned char uchar;
#if ELYSIA_PLATFORM == ELYSIA_PLATFORM_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif
typedef __int8 int8;
typedef unsigned __int8 uint8;
typedef __int16 int16;
typedef unsigned __int16 uint16;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;

#else
# include <stdint.h>
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
#endif

typedef float float32;
typedef double float64;

typedef uchar byte;
typedef std::string String;
typedef std::vector<uint8> MemoryBuffer;

template <class T> inline void hash_combine(std::size_t& seed, const T& v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

namespace Network {
class IOService;
class Stream;
class Address;
typedef std::vector<uint8> Chunk;
}
#ifdef NDEBUG
class ThreadIdCheck{};
#else
class ThreadIdCheck { public:
    unsigned int mThreadId;
};
#endif
class OptionSet;

namespace Task {
class LocalTime;
class DeltaTime;
}
class Time;
typedef Task::DeltaTime Duration;

} // namespace Elysia
//#include "Version.hpp"
#include "MemoryReference.hpp"
#include "TotallyOrdered.hpp"
#include "Singleton.hpp"
#include "FactoryWithOptions.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix3x3.hpp"
#include "Matrix4x4.hpp"
#include "Quaternion.hpp"
#include "SolidAngle.hpp"
#include "SelfWeakPtr.hpp"
#include "Noncopyable.hpp"
#include "Array.hpp"
#include "Logging.hpp"
#include "Location.hpp"
#include "VInt.hpp"
namespace Elysia {
template<class T>T*aligned_malloc(size_t num_bytes, const unsigned char alignment) {
    unsigned char *data=(unsigned char*)malloc(num_bytes+alignment);
    if (data!=NULL) {
        size_t remainder=((size_t)data)%alignment;
        size_t offset=alignment-remainder;
        data+=offset;
        data[-1]=offset;
        return (T*)(data);
    }
    return (T*)NULL;
}
template<class T>T*aligned_new(const unsigned char alignment) {
    return aligned_malloc<T>(sizeof(T),alignment);
}
template<class T> void aligned_free(T* data) {
    if (data!=NULL) {
        unsigned char *bloc=(unsigned char*)data;
        unsigned char offset=bloc[-1];
        free(bloc-offset);
    }
}

typedef Vector2<float32> Vector2f;
typedef Vector2<float64> Vector2d;
typedef Vector2<uint32> Vector2ui32;
typedef Vector2<int32> Vector2i32;

typedef Vector3<float32> Vector3f;
typedef Vector3<float64> Vector3d;
typedef Vector3<uint32> Vector3ui32;
typedef Vector3<int32> Vector3i32;

typedef Vector4<float32> Vector4f;
typedef Vector4<float64> Vector4d;
typedef Vector4<uint32> Vector4ui32;
typedef Vector4<int32> Vector4i32;

typedef VInt<uint32> vuint32;
typedef VInt<uint64> vuint64;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;
using std::placeholders::_5;
using std::placeholders::_6;
using std::placeholders::_7;
using std::placeholders::_8;
using std::placeholders::_9;
}
#include "BoundingSphere.hpp"
#include "BoundingBox.hpp"
namespace Elysia {
typedef BoundingBox<float32> BoundingBox3f3f;
typedef BoundingBox<float64> BoundingBox3d3f;
typedef BoundingBox<float32> BoundingBox3f;
typedef BoundingBox<float64> BoundingBox3d;
typedef BoundingSphere<float32> BoundingSphere3f;
typedef BoundingSphere<float64> BoundingSphere3d;
}

// A select few system-wide types
namespace Elysia {

typedef uint32 ServerID;
#define NullServerID 0
struct ServerIDNull {
    ServerID operator()() {
        return NullServerID;
    };
};
struct ServerIDRandom {
    ServerID operator()() {
        // Because ServerIDs aren't really random (we start allocating linearly
        // from 1), this just tries to generate one that won't conflict by
        // choosing a large number ( > 1,000,000).
        return (1 << 20) + (rand() % (1 << 20));
    };
};

// Space Server Regions
typedef std::vector<BoundingBox3f> BoundingBoxList;
struct SegmentationInfo {
    ServerID server;
    BoundingBoxList region;
};

struct ObjectHostID {
    ObjectHostID()
     : id(0)
    {
    }

    explicit ObjectHostID(uint64 _id)
     : id(_id)
    {
    }

    uint64 id;
};

ELYSIA_FUNCTION_EXPORT uint32 uint32_lexical_cast(const String& rhs);
ELYSIA_FUNCTION_EXPORT std::ostream& operator<<(std::ostream& os, const ObjectHostID& rhs);
ELYSIA_FUNCTION_EXPORT std::istream& operator>>(std::istream& is, ObjectHostID& rhs);

} // namespace Elysia

#if 0
template class std::unordered_map<Elysia::int32, Elysia::int32>;
template class std::unordered_map<Elysia::uint32, Elysia::uint32>;
template class std::unordered_map<Elysia::uint64, Elysia::uint64>;
template class std::unordered_map<Elysia::int64, Elysia::int64>;
template class std::unordered_map<Elysia::String, Elysia::String>;

template class std::unordered_map<Elysia::int32, void*>;
template class std::unordered_map<Elysia::uint32, void*>;
template class std::unordered_map<Elysia::uint64, void*>;
template class std::unordered_map<Elysia::int64, void*>;
template class std::unordered_map<Elysia::String, void*>;
template class std::map<Elysia::int32, Elysia::int32>;
template class std::map<Elysia::uint32, Elysia::uint32>;
template class std::map<Elysia::uint64, Elysia::uint64>;
template class std::map<Elysia::int64, Elysia::int64>;
template class std::map<Elysia::String, Elysia::String>;
template class std::map<Elysia::int32, void*>;
template class std::map<Elysia::uint32, void*>;
template class std::map<Elysia::uint64, void*>;
template class std::map<Elysia::int64, void*>;
template class std::map<Elysia::String, void*>;
template class std::vector<Elysia::String>;
template class std::vector<void*>;
template class std::vector<Elysia::int8>;
template class std::vector<Elysia::uint8>;
#endif
#endif //__cplusplus
#endif //_ELYSIA_PLATFORM_HPP_
