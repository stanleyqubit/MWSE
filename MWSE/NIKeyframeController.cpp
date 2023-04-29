#include "NIKeyframeController.h"

namespace NI {

	//
	// AnimationKey
	//

	template <typename T>
	struct AnimationKeyFunctions {
		struct InterpolationType {
			union {
				struct {
					T none;
					T linear;
					T bezier;
					T TCB;
					T euler;
				};
				T functions[5];
			};

			T& operator[](AnimationKey::KeyType type) {
				return functions[static_cast<unsigned int>(type)];
			}
		};

		union {
			struct {
				InterpolationType float_;
				InterpolationType position;
				InterpolationType rotation;
				InterpolationType color;
				InterpolationType text;
				InterpolationType vis;
			};
			InterpolationType types[6];
		};

		InterpolationType& operator[](AnimationKey::ContentType type) {
			return types[static_cast<unsigned int>(type)];
		}
	};
	static_assert(sizeof(AnimationKeyFunctions<AnimationKey::FillDerivedValuesFunction>) == 0x78, "NI::AnimationKeyFunctions failed size validation");

	AnimationKey::FillDerivedValuesFunction AnimationKey::getFillDerivedValuesFunction(AnimationKey::ContentType content, AnimationKey::KeyType key) {
		auto& fns = *reinterpret_cast<AnimationKeyFunctions<FillDerivedValuesFunction>*>(0x7DF868);
		return fns[content][key];
	}

	//
	// EulerRotKey
	//

	std::reference_wrapper<decltype(EulerRotKey::numKeys)> EulerRotKey::getNumKeys_lua() {
		return std::ref(numKeys);
	}

	std::reference_wrapper<decltype(EulerRotKey:: keyTypes)> EulerRotKey::getKeyTypes_lua() {
		return std::ref(keyTypes);
	}

	sol::table EulerRotKey::getKeys_lua(sol::this_state ts) {
		sol::state_view state = ts;
		auto table = state.create_table(3);
		for (auto i = 0; i < 3; ++i) {
			if (keys[i].asFloatKey) {
				switch (keyTypes[i]) {
				case AnimationKey::KeyType::NoInterp:
				case AnimationKey::KeyType::Linear:
					table[i + 1] = nonstd::span(keys[i].asFloatKey, numKeys[i]);
					break;
				case AnimationKey::KeyType::Bezier:
					table[i + 1] = nonstd::span(keys[i].asBezFloatKey, numKeys[i]);
					break;
				case AnimationKey::KeyType::TCB:
					table[i + 1] = nonstd::span(keys[i].asTCBFloatKey, numKeys[i]);
					break;
				default:
					throw std::runtime_error("Invalid euler rotation key type found. Report to MWSE developers.");
				}
			}
		}
		return table;
	}

	std::reference_wrapper<decltype(EulerRotKey::lastIndices)> EulerRotKey::getLastIndices_lua() {
		return std::ref(lastIndices);
	}

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
