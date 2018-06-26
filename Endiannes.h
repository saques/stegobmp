#include <stdint.h>

namespace Endiannes {
	void SwapEndiannes(uint32_t& littleEndian)
	{
		uint32_t bigEndian = 0;
		uint8_t * temp = reinterpret_cast<uint8_t *>(&bigEndian);
		for (int i = 0; i < 4; i++) {
			temp[i] = static_cast<uint8_t>(littleEndian >> ((3 - i) * 8));
		}

		littleEndian = bigEndian;
	}
}
