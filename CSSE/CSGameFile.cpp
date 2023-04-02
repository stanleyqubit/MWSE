#include "CSGameFile.h"

namespace se::cs {
	bool GameFile::getToLoadFlag() const {
		const auto GameFile_getToLoadFlag = reinterpret_cast<bool(__thiscall*)(const GameFile*)>(0x40148D);
		return GameFile_getToLoadFlag(this);
	}

	void GameFile::setToLoadFlag(bool state) {
		const auto GameFile_setToLoadFlag = reinterpret_cast<void(__thiscall*)(GameFile*, bool)>(0x401D84);
		GameFile_setToLoadFlag(this, state);
	}
}
