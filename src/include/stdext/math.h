
#ifndef STDEXT_MATH_H
#define STDEXT_MATH_H

#include <algorithm>
#include "types.h"

namespace stdext {

	uint32_t adler32(const uint8_t *buffer, size_t size);

	long randomRange(long min, long max);
	float randomRange(float min, float max);

	inline uint16_t readULE16(const uchar *addr) { return (uint16_t)addr[1] << 8 | addr[0]; }
	inline uint32_t readULE32(const uchar *addr) { return (uint32_t)readULE16(addr + 2) << 16 | readULE16(addr); }
	inline uint64_t readULE64(const uchar *addr) { return (uint64_t)readULE32(addr + 4) << 32 | readULE32(addr); }

	inline void writeULE16(uchar *addr, uint16_t value) { addr[1] = value >> 8; addr[0] = (uint8_t)value; }
	inline void writeULE32(uchar *addr, uint32_t value) { writeULE16(addr + 2, value >> 16); writeULE16(addr, (uint16_t)value); }
	inline void writeULE64(uchar *addr, uint64_t value) { writeULE32(addr + 4, value >> 32); writeULE32(addr, (uint32_t)value); }

	inline int16_t readSLE16(const uchar *addr) { return (int16_t)addr[1] << 8 | addr[0]; }
	inline int32_t readSLE32(const uchar *addr) { return (int32_t)readSLE16(addr + 2) << 16 | readSLE16(addr); }
	inline int64_t readSLE64(const uchar *addr) { return (int64_t)readSLE32(addr + 4) << 32 | readSLE32(addr); }

	inline void writeSLE16(uchar *addr, int16_t value) { addr[1] = value >> 8; addr[0] = (int8_t)value; }
	inline void writeSLE32(uchar *addr, int32_t value) { writeSLE16(addr + 2, value >> 16); writeSLE16(addr, (int16_t)value); }
	inline void writeSLE64(uchar *addr, int64_t value) { writeSLE32(addr + 4, value >> 32); writeSLE32(addr, (int32_t)value); }

	template<typename T, typename F>
	inline void flagChange(T& out, F flag, bool enabled) {
		if (enabled) {
			out = out | (T)flag;
		} else {
			out = out & ~(T)flag;
		}
	}

	template<typename T, typename F>
	inline bool flagIsSet(const T& v, const F& flag) {
		return (v & (T)flag) != 0 ? true : false;
	}

}

#endif