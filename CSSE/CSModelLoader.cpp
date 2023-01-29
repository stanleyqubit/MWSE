#include "CSModelLoader.h"

namespace se::cs {
	NI::Node* ModelLoader::loadNIF(const char* path) {
		const auto ModelLoader_loadNIF = reinterpret_cast<NI::Node*(__thiscall*)(ModelLoader*, const char*)>(0x403477);
		return ModelLoader_loadNIF(this, path);
	}

	bool ModelLoader::releaseNIF(NI::AVObject* nif) {
		const auto ModelLoader_releaseNIF = reinterpret_cast<bool(__thiscall*)(ModelLoader*, NI::AVObject*)>(0x401B09);
		return ModelLoader_releaseNIF(this, nif);
	}
}
