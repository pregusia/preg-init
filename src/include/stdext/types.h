
#ifndef STDEXT_TYPES_H
#define STDEXT_TYPES_H

#include <cstdint>
#include <cstddef>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;

typedef int64 ticks_t;
typedef uint_fast32_t refcount_t;

using std::size_t;
using std::ptrdiff_t;

#endif
