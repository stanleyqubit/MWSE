#include "NIAnimationData.h"

namespace NI {
	const auto NI_FloatData_getKeyDataSize = reinterpret_cast<unsigned int(__cdecl*)(AnimationKey::KeyType)>(0x723A20);
	unsigned int FloatData::getKeyDataSize() const {
		return NI_FloatData_getKeyDataSize(keyType);
	}

	const auto NI_PosData_getKeyDataSize = reinterpret_cast<unsigned int(__cdecl*)(AnimationKey::KeyType)>(0x7239F0);
	unsigned int PosData::getKeyDataSize() const {
		return NI_PosData_getKeyDataSize(keyType);
	}

	nonstd::span<ColorKey*> ColorData::getKeys() const {
		return nonstd::span(keys, keyCount);
	};
}
