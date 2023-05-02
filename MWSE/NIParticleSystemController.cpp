#include "NIParticleSystemController.h"

namespace NI {
	nonstd::span<PerParticleData> ParticleSystemController::getPerParticleData() {
		if (particleData == nullptr) {
			return {};
		}
		return { particleData, particleDataCount };
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ParticleSystemController)
