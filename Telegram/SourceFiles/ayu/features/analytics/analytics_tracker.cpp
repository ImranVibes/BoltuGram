// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/features/analytics/analytics_tracker.h"

#include "core/application.h"
#include "ayu/libs/json.hpp"
#include "base/unixtime.h"

#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <fstream>
#include <algorithm>

using json = nlohmann::json;

namespace AyuFeatures {

namespace {

QString getAnalyticsPath() {
	return cWorkingDir() + u"tdata/boltugram_analytics.json"_q;
}

} // namespace

AnalyticsTracker &AnalyticsTracker::Instance() {
	static AnalyticsTracker instance;
	return instance;
}

AnalyticsTracker::AnalyticsTracker() {
	_hourlyHeatmap.fill(0);
	load();
}

void AnalyticsTracker::load() {
	const auto path = getAnalyticsPath().toStdString();
	std::ifstream file(path);
	if (!file.good()) {
		return;
	}

	try {
		json root;
		file >> root;
		file.close();

		_totalMessages = root.value("totalMessages", uint64(0));
		_totalMedia = root.value("totalMedia", uint64(0));

		if (root.contains("hourlyHeatmap") && root["hourlyHeatmap"].is_array()) {
			int i = 0;
			for (const auto &val : root["hourlyHeatmap"]) {
				if (i < 24) {
					_hourlyHeatmap[i++] = val.get<uint32_t>();
				}
			}
		}

		_topChats.clear();
		if (root.contains("topChats") && root["topChats"].is_array()) {
			for (const auto &item : root["topChats"]) {
				ChatStatsItem cs;
				cs.peerId = item.value("peerId", uint64(0));
				cs.peerName = QString::fromStdString(item.value("peerName", ""));
				cs.messageCount = item.value("messageCount", uint64(0));
				cs.mediaCount = item.value("mediaCount", uint64(0));
				cs.lastActiveTime = item.value("lastActiveTime", uint64(0));
				if (cs.peerId != 0) {
					_topChats.push_back(std::move(cs));
				}
			}
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to parse analytics JSON"));
	}
}

void AnalyticsTracker::save() {
	const auto path = getAnalyticsPath().toStdString();
	json root;
	root["totalMessages"] = _totalMessages;
	root["totalMedia"] = _totalMedia;

	json heatmap = json::array();
	for (int i = 0; i < 24; ++i) {
		heatmap.push_back(_hourlyHeatmap[i]);
	}
	root["hourlyHeatmap"] = std::move(heatmap);

	json chats = json::array();
	for (const auto &cs : _topChats) {
		json item;
		item["peerId"] = cs.peerId;
		item["peerName"] = cs.peerName.toStdString();
		item["messageCount"] = cs.messageCount;
		item["mediaCount"] = cs.mediaCount;
		item["lastActiveTime"] = cs.lastActiveTime;
		chats.push_back(std::move(item));
	}
	root["topChats"] = std::move(chats);

	try {
		std::ofstream file(path);
		if (file.good()) {
			file << root.dump(4);
			file.close();
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to save analytics JSON"));
	}
}

void AnalyticsTracker::recordMessage(uint64 peerId, const QString &peerName, bool isMedia) {
	_totalMessages++;
	if (isMedia) {
		_totalMedia++;
	}

	const auto currentHour = QDateTime::currentDateTime().time().hour();
	if (currentHour >= 0 && currentHour < 24) {
		_hourlyHeatmap[currentHour]++;
	}

	bool found = false;
	const auto now = static_cast<uint64>(base::unixtime::now());
	for (auto &cs : _topChats) {
		if (cs.peerId == peerId) {
			cs.messageCount++;
			if (isMedia) cs.mediaCount++;
			cs.lastActiveTime = now;
			if (!peerName.isEmpty()) cs.peerName = peerName;
			found = true;
			break;
		}
	}

	if (!found && peerId != 0) {
		ChatStatsItem cs;
		cs.peerId = peerId;
		cs.peerName = peerName;
		cs.messageCount = 1;
		cs.mediaCount = isMedia ? 1 : 0;
		cs.lastActiveTime = now;
		_topChats.push_back(std::move(cs));
	}

	// Sort top chats by message volume
	std::sort(_topChats.begin(), _topChats.end(), [](const ChatStatsItem &a, const ChatStatsItem &b) {
		return a.messageCount > b.messageCount;
	});

	if (_topChats.size() > 50) {
		_topChats.resize(50);
	}

	save();
	_changed.fire({});
}

uint64 AnalyticsTracker::totalMessagesSent() const {
	return _totalMessages;
}

uint64 AnalyticsTracker::totalMediaSent() const {
	return _totalMedia;
}

std::array<uint32_t, 24> AnalyticsTracker::hourlyHeatmap() const {
	return _hourlyHeatmap;
}

std::vector<ChatStatsItem> AnalyticsTracker::topChats() const {
	return _topChats;
}

rpl::producer<> AnalyticsTracker::changed() const {
	return _changed.events();
}

} // namespace AyuFeatures
