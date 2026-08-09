// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/features/voice_studio/voice_studio_manager.h"

#include "core/application.h"
#include "ayu/libs/json.hpp"

#include <QtCore/QFile>
#include <fstream>

using json = nlohmann::json;

namespace AyuFeatures {

namespace {

QString getVoiceStudioPath() {
	return cWorkingDir() + u"tdata/boltugram_voicestudio.json"_q;
}

} // namespace

VoiceStudioManager &VoiceStudioManager::Instance() {
	static VoiceStudioManager instance;
	return instance;
}

VoiceStudioManager::VoiceStudioManager() {
	load();
}

void VoiceStudioManager::load() {
	const auto path = getVoiceStudioPath().toStdString();
	std::ifstream file(path);
	if (!file.good()) {
		return;
	}

	try {
		json root;
		file >> root;
		file.close();

		_filter = static_cast<VoiceFilterType>(root.value("filter", 0));
		_playbackSpeed = root.value("playbackSpeed", 1.0f);
		_noiseSuppression = root.value("noiseSuppression", true);
	} catch (...) {
		LOG(("BoltuGram: Failed to parse voicestudio config"));
	}
}

void VoiceStudioManager::save() {
	const auto path = getVoiceStudioPath().toStdString();
	json root;
	root["filter"] = static_cast<int>(_filter);
	root["playbackSpeed"] = _playbackSpeed;
	root["noiseSuppression"] = _noiseSuppression;

	try {
		std::ofstream file(path);
		if (file.good()) {
			file << root.dump(4);
			file.close();
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to save voicestudio config"));
	}
}

VoiceFilterType VoiceStudioManager::currentFilter() const {
	return _filter;
}

void VoiceStudioManager::setFilter(VoiceFilterType filter) {
	if (_filter == filter) {
		return;
	}
	_filter = filter;
	save();
	_changed.fire({});
}

float VoiceStudioManager::playbackSpeed() const {
	return _playbackSpeed;
}

void VoiceStudioManager::setPlaybackSpeed(float speed) {
	if (_playbackSpeed == speed) {
		return;
	}
	_playbackSpeed = speed;
	save();
	_changed.fire({});
}

bool VoiceStudioManager::noiseSuppressionEnabled() const {
	return _noiseSuppression;
}

void VoiceStudioManager::setNoiseSuppression(bool enabled) {
	if (_noiseSuppression == enabled) {
		return;
	}
	_noiseSuppression = enabled;
	save();
	_changed.fire({});
}

rpl::producer<> VoiceStudioManager::changed() const {
	return _changed.events();
}

} // namespace AyuFeatures
