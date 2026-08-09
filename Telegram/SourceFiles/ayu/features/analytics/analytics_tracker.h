// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#pragma once

#include <QtCore/QString>
#include <vector>
#include <array>
#include "rpl/producer.h"
#include "rpl/event_stream.h"

namespace AyuFeatures {

struct ChatStatsItem {
	uint64 peerId = 0;
	QString peerName;
	uint64 messageCount = 0;
	uint64 mediaCount = 0;
	uint64 lastActiveTime = 0;
};

class AnalyticsTracker final {
public:
	static AnalyticsTracker &Instance();

	void recordMessage(uint64 peerId, const QString &peerName, bool isMedia = false);

	[[nodiscard]] uint64 totalMessagesSent() const;
	[[nodiscard]] uint64 totalMediaSent() const;
	[[nodiscard]] std::array<uint32_t, 24> hourlyHeatmap() const;
	[[nodiscard]] std::vector<ChatStatsItem> topChats() const;

	[[nodiscard]] rpl::producer<> changed() const;

private:
	AnalyticsTracker();
	void load();
	void save();

	uint64 _totalMessages = 0;
	uint64 _totalMedia = 0;
	std::array<uint32_t, 24> _hourlyHeatmap{};
	std::vector<ChatStatsItem> _topChats;
	rpl::event_stream<> _changed;
};

} // namespace AyuFeatures
