
#include "math.h"
#include <random>

namespace stdext {

	// ************************************************************************************
	uint32_t adler32(const uint8_t *buffer, size_t size) {
		register size_t a = 1, b = 0, tlen;
		while(size > 0) {
			tlen = size > 5552 ? 5552 : size;
			size -= tlen;
			do {
				a += *buffer++;
				b += a;
			} while (--tlen);

			a %= 65521;
			b %= 65521;
		}
		return (b << 16) | a;
	}

	// ************************************************************************************
	long randomRange(long min, long max) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<long> dis(0, 2147483647);
		return min + (dis(gen) % (max - min + 1));
	}

	// ************************************************************************************
	float randomRange(float min, float max) {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_real_distribution<float> dis(0.0, 1.0);
		return min + (max - min)*dis(gen);
	}

}
