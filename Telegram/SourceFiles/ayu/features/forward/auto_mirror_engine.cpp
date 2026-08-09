// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/features/forward/auto_mirror_engine.h"

#include "core/application.h"
#include "ayu/libs/json.hpp"
#include "base/unixtime.h"

#include <QtCore/QRegularExpression>
#include <QtCore/QFile>
#include <fstream>

using json = nlohmann::json;

namespace AyuFeatures {

namespace {

QString getAutoMirrorPath() {
	return cWorkingDir() + u"tdata/boltugram_automirror.json"_q;
}

} // namespace

AutoMirrorEngine &AutoMirrorEngine::Instance() {
	static AutoMirrorEngine instance;
	return instance;
}

AutoMirrorEngine::AutoMirrorEngine() {
	load();
}

void AutoMirrorEngine::load() {
	_rules.clear();
	const auto path = getAutoMirrorPath().toStdString();
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
				MirrorRule rule;
				rule.id = QString::fromStdString(item.value("id", ""));
				rule.name = QString::fromStdString(item.value("name", ""));
				rule.sourcePeerId = item.value("sourcePeerId", uint64(0));
				rule.targetPeerId = item.value("targetPeerId", uint64(0));
				rule.cleanCopy = item.value("cleanCopy", true);
				rule.removeLinks = item.value("removeLinks", true);
				rule.removeMentions = item.value("removeMentions", true);
				rule.customHeader = QString::fromStdString(item.value("customHeader", ""));
				rule.customFooter = QString::fromStdString(item.value("customFooter", ""));
				rule.enabled = item.value("enabled", true);
				if (!rule.id.isEmpty()) {
					_rules.push_back(std::move(rule));
				}
			}
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to parse automirror JSON"));
	}
}

void AutoMirrorEngine::save() {
	const auto path = getAutoMirrorPath().toStdString();
	json root = json::array();

	for (const auto &rule : _rules) {
		json item;
		item["id"] = rule.id.toStdString();
		item["name"] = rule.name.toStdString();
		item["sourcePeerId"] = rule.sourcePeerId;
		item["targetPeerId"] = rule.targetPeerId;
		item["cleanCopy"] = rule.cleanCopy;
		item["removeLinks"] = rule.removeLinks;
		item["removeMentions"] = rule.removeMentions;
		item["customHeader"] = rule.customHeader.toStdString();
		item["customFooter"] = rule.customFooter.toStdString();
		item["enabled"] = rule.enabled;
		root.push_back(std::move(item));
	}

	try {
		std::ofstream file(path);
		if (file.good()) {
			file << root.dump(4);
			file.close();
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to save automirror JSON"));
	}
}

std::vector<MirrorRule> AutoMirrorEngine::getRules() const {
	return _rules;
}

void AutoMirrorEngine::addRule(const MirrorRule &rule) {
	_rules.push_back(rule);
	save();
	_changed.fire({});
}

void AutoMirrorEngine::deleteRule(const QString &id) {
	const auto it = std::remove_if(_rules.begin(), _rules.end(), [&](const MirrorRule &r) {
		return r.id == id;
	});
	if (it != _rules.end()) {
		_rules.erase(it, _rules.end());
		save();
		_changed.fire({});
	}
}

void AutoMirrorEngine::toggleRule(const QString &id, bool enabled) {
	for (auto &r : _rules) {
		if (r.id == id) {
			r.enabled = enabled;
			save();
			_changed.fire({});
			break;
		}
	}
}

QString AutoMirrorEngine::processText(const MirrorRule &rule, const QString &inputText) const {
	QString text = inputText;

	if (rule.removeLinks) {
		static const auto linkRegex = QRegularExpression(
			"(https?://[\\w\\d:#@%/;$()~_?\\+-=\\\\\\.&]+|t\\.me/[\\w\\d_+]+)",
			QRegularExpression::CaseInsensitiveOption);
		text.replace(linkRegex, "");
	}

	if (rule.removeMentions) {
		static const auto mentionRegex = QRegularExpression("@[a-zA-Z0-9_]{4,32}");
		text.replace(mentionRegex, "");
	}

	if (!rule.customHeader.isEmpty()) {
		text = rule.customHeader + "\n\n" + text;
	}

	if (!rule.customFooter.isEmpty()) {
		text = text + "\n\n" + rule.customFooter;
	}

	return text.trimmed();
}

rpl::producer<> AutoMirrorEngine::changed() const {
	return _changed.events();
}

} // namespace AyuFeatures
