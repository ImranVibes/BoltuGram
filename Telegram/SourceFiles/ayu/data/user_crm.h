// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#pragma once

#include <QtCore/QString>
#include <vector>
#include <optional>
#include "rpl/producer.h"
#include "rpl/event_stream.h"

namespace AyuData {

struct UserTagInfo {
	uint64 peerId = 0;
	QString tag;
	QString colorHex; // e.g. #3390ec, #e53935, #43a047, #fb8c00, #8e24aa, #00acc1, #e91e63, #607d8b
	QString note;
	uint64 updatedAt = 0;
};

class UserCrm final {
public:
	static UserCrm &Instance();

	[[nodiscard]] std::optional<UserTagInfo> get(uint64 peerId) const;
	void set(uint64 peerId, const QString &tag, const QString &colorHex, const QString &note);
	void remove(uint64 peerId);
	[[nodiscard]] std::vector<UserTagInfo> getAll() const;
	[[nodiscard]] rpl::producer<uint64> updated() const;

private:
	UserCrm();
	void load();
	void save();

	std::vector<UserTagInfo> _entries;
	rpl::event_stream<uint64> _updated;
};

} // namespace AyuData
