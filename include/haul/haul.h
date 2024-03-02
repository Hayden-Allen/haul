#pragma once
#define HAUL_ASSERT(x) \
	if (!(x)) __debugbreak();
#define HAUL_DCM(x)       \
	x(x const&) = delete; \
	x(x&&) = delete;
#define HAUL_CAST(T, x) static_cast<T>(x)
#define HAUL_PUN(T, x)	(*(T*)&x)

#include <inttypes.h>
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

#include "../../src/bdmap.h"
#include "../../src/typed.h"
#include "../../src/parent.h"
#include "../../src/point.h"
#include "../../src/range.h"
#include "../../src/rect.h"
#include "../../src/dimensional.h"
#include "../../src/file.h"
#include "../../src/serializable.h"
