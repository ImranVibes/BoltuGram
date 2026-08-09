// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#pragma once

#include <QtCore/QString>
#include <vector>
#include "rpl/producer.h"
#include "rpl/event_stream.h"

namespace AyuFeatures {

enum class VoiceFilterType {
	Normal = 0,
	DeepVoice = 1,
	HighPitch = 2,
	Radio = 3,
	Robot = 4,
	Echo = 5,
};

class VoiceStudioManager final {
public:
	static VoiceStudioManager &Instance();

	[[nodiscard]] VoiceFilterType currentFilter() const;
	void setFilter(VoiceFilterType filter);

	[[nodiscard]] float playbackSpeed() const;
	void setPlaybackSpeed(float speed);

	[[nodiscard]] bool noiseSuppressionEnabled() const;
	void setNoiseSuppression(bool enabled);

	[[nodiscard]] rpl::producer<> changed() const;

private:
	VoiceStudioManager();
	void load();
	void save();

	VoiceFilterType _filter = VoiceFilterType::Normal;
	float _playbackSpeed = 1.0f;
	bool _noiseSuppression = true;
	rpl::event_stream<> _changed;
};

} // namespace AyuFeatures
