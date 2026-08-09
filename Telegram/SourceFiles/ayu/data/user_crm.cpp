// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/data/user_crm.h"

#include "core/application.h"
#include "ayu/libs/json.hpp"
#include "base/unixtime.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <fstream>

using json = nlohmann::json;

namespace AyuData {

namespace {

QString getCrmStoragePath() {
	return cWorkingDir() + u"tdata/boltugram_crm.json"_q;
}

} // namespace

UserCrm &UserCrm::Instance() {
	static UserCrm instance;
	return instance;
}

UserCrm::UserCrm() {
	load();
}

void UserCrm::load() {
	_entries.clear();
	const auto path = getCrmStoragePath().toStdString();
	std::ifstream file(path);
	if (!file.good()) {
		return;
	}

	try {
		json root;
		file >> root;
		file.close();

		if (root.is_array()) {
			for (const auto &item : root) {
				UserTagInfo info;
				info.peerId = item.value("peerId", uint64(0));
				info.tag = QString::fromStdString(item.value("tag", ""));
				info.colorHex = QString::fromStdString(item.value("colorHex", "#3390ec"));
				info.note = QString::fromStdString(item.value("note", ""));
				info.updatedAt = item.value("updatedAt", uint64(0));
				if (info.peerId != 0) {
					_entries.push_back(std::move(info));
				}
			}
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to parse user_crm.json"));
	}
}

void UserCrm::save() {
	const auto path = getCrmStoragePath().toStdString();
	json root = json::array();

	for (const auto &info : _entries) {
		json item;
		item["peerId"] = info.peerId;
		item["tag"] = info.tag.toStdString();
		item["colorHex"] = info.colorHex.toStdString();
		item["note"] = info.note.toStdString();
		item["updatedAt"] = info.updatedAt;
		root.push_back(std::move(item));
	}

	try {
		std::ofstream file(path);
		if (file.good()) {
			file << root.dump(4);
			file.close();
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to save user_crm.json"));
	}
}

std::optional<UserTagInfo> UserCrm::get(uint64 peerId) const {
	for (const auto &info : _entries) {
		if (info.peerId == peerId) {
			return info;
		}
	}
	return std::nullopt;
}

void UserCrm::set(uint64 peerId, const QString &tag, const QString &colorHex, const QString &note) {
	if (peerId == 0) {
		return;
	}

	auto found = false;
	const auto now = static_cast<uint64>(base::unixtime::now());

	for (auto &info : _entries) {
		if (info.peerId == peerId) {
			info.tag = tag.trimmed();
			info.colorHex = colorHex.trimmed().isEmpty() ? QString("#3390ec") : colorHex.trimmed();
			info.note = note;
			info.updatedAt = now;
			found = true;
			break;
		}
	}

	if (!found) {
		UserTagInfo info;
		info.peerId = peerId;
		info.tag = tag.trimmed();
		info.colorHex = colorHex.trimmed().isEmpty() ? QString("#3390ec") : colorHex.trimmed();
		info.note = note;
		info.updatedAt = now;
		_entries.push_back(std::move(info));
	}

	save();
	_updated.fire_copy(peerId);
}

void UserCrm::remove(uint64 peerId) {
	const auto it = std::remove_if(_entries.begin(), _entries.end(), [=](const UserTagInfo &info) {
		return info.peerId == peerId;
	});
	if (it != _entries.end()) {
		_entries.erase(it, _entries.end());
		save();
		_updated.fire_copy(peerId);
	}
}

std::vector<UserTagInfo> UserCrm::getAll() const {
	return _entries;
}

rpl::producer<uint64> UserCrm::updated() const {
	return _updated.events();
}

} // namespace AyuData
