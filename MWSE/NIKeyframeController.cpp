#include "NIKeyframeController.h"

namespace NI {
	//
	// KeyframeData
	//

	sol::object KeyframeData::getRotationKeys_lua(sol::this_state L) {
		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		switch (rotationType) {
		case AnimationKey::KeyType::NoInterp:
		case AnimationKey::KeyType::Linear:
			return sol::make_object(L, nonstd::span(rotationKeys.asRotKey, rotationKeyCount));
		case AnimationKey::KeyType::Bezier:
			return sol::make_object(L, nonstd::span(rotationKeys.asBezRotKey, rotationKeyCount));
		case AnimationKey::KeyType::TCB:
			return sol::make_object(L, nonstd::span(rotationKeys.asTCBRotKey, rotationKeyCount));
		case AnimationKey::KeyType::Euler:
			return sol::make_object(L, nonstd::span(rotationKeys.asEulerRotKey, rotationKeyCount));
		default:
			throw std::runtime_error("Invalid position type found. Report to MWSE developers.");
		}
	}

	sol::object KeyframeData::getPositionKeys_lua(sol::this_state L) {
		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		switch (positionType) {
		case AnimationKey::KeyType::NoInterp:
		case AnimationKey::KeyType::Linear:
			return sol::make_object(L, nonstd::span(positionKeys.asPosKey, positionKeyCount));
		case AnimationKey::KeyType::Bezier:
			return sol::make_object(L, nonstd::span(positionKeys.asBezPosKey, positionKeyCount));
		case AnimationKey::KeyType::TCB:
			return sol::make_object(L, nonstd::span(positionKeys.asTCBPosKey, positionKeyCount));
		default:
			throw std::runtime_error("Invalid position type found. Report to MWSE developers.");
		}
	}

	sol::object KeyframeData::getScaleKeys_lua(sol::this_state L) {
		// Make sure we're looking at the main state.
		L = sol::main_thread(L);

		switch (scaleType) {
		case AnimationKey::KeyType::NoInterp:
		case AnimationKey::KeyType::Linear:
			return sol::make_object(L, nonstd::span(scaleKeys.asFloatKey, scaleKeyCount));
		case AnimationKey::KeyType::Bezier:
			return sol::make_object(L, nonstd::span(scaleKeys.asBezFloatKey, scaleKeyCount));
		case AnimationKey::KeyType::TCB:
			return sol::make_object(L, nonstd::span(scaleKeys.asTCBFloatKey, scaleKeyCount));
		default:
			throw std::runtime_error("Invalid position type found. Report to MWSE developers.");
		}
	}

	void KeyframeData::updateDerivedValues() {
		if (positionKeys.asPosKey) {
			const auto fn = AnimationKey::getFillDerivedValuesFunction(AnimationKey::ContentType::Position, positionType);
			fn(positionKeys.asPosKey, positionKeyCount, positionType);
		}

		if (rotationKeys.asRotKey) {
			const auto fn = AnimationKey::getFillDerivedValuesFunction(AnimationKey::ContentType::Rotation, rotationType);
			fn(rotationKeys.asRotKey, rotationKeyCount, rotationType);
		}

		if (scaleKeys.asFloatKey) {
			const auto fn = AnimationKey::getFillDerivedValuesFunction(AnimationKey::ContentType::Float, scaleType);
			fn(scaleKeys.asFloatKey, scaleKeyCount, scaleType);
		}
	}

	const auto TES3_KeyframeData_replaceScaleData = reinterpret_cast<void(__thiscall*)(KeyframeData*,FloatKey*, unsigned int, AnimationKey::KeyType)>(0x70D420);
	void KeyframeData::replaceScaleData(FloatKey* keys, unsigned int numKeys, AnimationKey::KeyType keyType) {
		TES3_KeyframeData_replaceScaleData(this, keys, numKeys, keyType);
	}
}
