#pragma once

namespace TES3 {
	namespace AIBehavior {
		enum AIBehavior : unsigned char {
			Hello,
			Wait,
			Idle,
			Attack,
			Avoid,
			Behavior5,
			Flee,
			Behavior7,
			Walk,
			Alarmed,
			Behavior10,
			Behavior11,
			Greet,
			Behavior13,
			CorpseFloatingUp,
			CorpseFalling,
			Behavior16,
			Undecided = 0xFF,
		};
	}
}
