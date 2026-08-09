// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/data/scratchpad_manager.h"

#include "core/application.h"
#include "ayu/libs/json.hpp"
#include "base/unixtime.h"
#include "base/random.h"

#include <QtCore/QFile>
#include <fstream>

using json = nlohmann::json;

namespace AyuData {

namespace {

QString getScratchpadPath() {
	return cWorkingDir() + u"tdata/boltugram_scratchpad.json"_q;
}

} // namespace

ScratchpadManager &ScratchpadManager::Instance() {
	static ScratchpadManager instance;
	return instance;
}

ScratchpadManager::ScratchpadManager() {
	load();
	if (_templates.empty()) {
		// Populate initial handy templates for daily productivity
		_templates.push_back({
			.id = "tpl_1",
			.title = "Thank You",
			.content = "Thank you so much for your help! Really appreciate it. 🙏",
		});
		_templates.push_back({
			.id = "tpl_2",
			.title = "Let Me Check",
			.content = "Got it, let me check the details and get back to you shortly.",
		});
		_templates.push_back({
			.id = "tpl_3",
			.title = "Call Request",
			.content = "Hey, are you free for a quick call or voice chat right now?",
		});
		_templates.push_back({
			.id = "tpl_4",
			.title = "Meeting Link",
			.content = "Here is the link for our upcoming meeting: ",
		});
	}
}

void ScratchpadManager::load() {
	_templates.clear();
	_bookmarks.clear();
	_text = QString();

	const auto path = getScratchpadPath().toStdString();
	std::ifstream file(path);
	if (!file.good()) {
		return;
	}

	try {
		json root;
		file >> root;
		file.close();

		_text = QString::fromStdString(root.value("text", ""));

		if (root.contains("templates") && root["templates"].is_array()) {
			for (const auto &item : root["templates"]) {
				CannedTemplate t;
				t.id = QString::fromStdString(item.value("id", ""));
				t.title = QString::fromStdString(item.value("title", ""));
				t.content = QString::fromStdString(item.value("content", ""));
				if (!t.title.isEmpty() && !t.content.isEmpty()) {
					_templates.push_back(std::move(t));
				}
			}
		}

		if (root.contains("bookmarks") && root["bookmarks"].is_array()) {
			for (const auto &item : root["bookmarks"]) {
				SavedBookmark b;
				b.id = QString::fromStdString(item.value("id", ""));
				b.text = QString::fromStdString(item.value("text", ""));
				b.author = QString::fromStdString(item.value("author", ""));
				b.date = item.value("date", uint64(0));
				if (!b.text.isEmpty()) {
					_bookmarks.push_back(std::move(b));
				}
			}
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to parse scratchpad JSON"));
	}
}

void ScratchpadManager::save() {
	const auto path = getScratchpadPath().toStdString();
	json root;
	root["text"] = _text.toStdString();

	json templatesArray = json::array();
	for (const auto &t : _templates) {
		json item;
		item["id"] = t.id.toStdString();
		item["title"] = t.title.toStdString();
		item["content"] = t.content.toStdString();
		templatesArray.push_back(std::move(item));
	}
	root["templates"] = std::move(templatesArray);

	json bookmarksArray = json::array();
	for (const auto &b : _bookmarks) {
		json item;
		item["id"] = b.id.toStdString();
		item["text"] = b.text.toStdString();
		item["author"] = b.author.toStdString();
		item["date"] = b.date;
		bookmarksArray.push_back(std::move(item));
	}
	root["bookmarks"] = std::move(bookmarksArray);

	try {
		std::ofstream file(path);
		if (file.good()) {
			file << root.dump(4);
			file.close();
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to write scratchpad JSON"));
	}
}

QString ScratchpadManager::getText() const {
	return _text;
}

void ScratchpadManager::setText(const QString &text) {
	if (_text == text) {
		return;
	}
	_text = text;
	save();
	_changed.fire({});
}

std::vector<CannedTemplate> ScratchpadManager::getTemplates() const {
	return _templates;
}

void ScratchpadManager::addTemplate(const QString &title, const QString &content) {
	if (title.trimmed().isEmpty() || content.trimmed().isEmpty()) {
		return;
	}
	CannedTemplate t;
	t.id = QString("tpl_%1").arg(base::unixtime::now());
	t.title = title.trimmed();
	t.content = content.trimmed();
	_templates.push_back(std::move(t));
	save();
	_changed.fire({});
}

void ScratchpadManager::deleteTemplate(const QString &id) {
	const auto it = std::remove_if(_templates.begin(), _templates.end(), [&](const CannedTemplate &t) {
		return t.id == id;
	});
	if (it != _templates.end()) {
		_templates.erase(it, _templates.end());
		save();
		_changed.fire({});
	}
}

std::vector<SavedBookmark> ScratchpadManager::getBookmarks() const {
	return _bookmarks;
}

void ScratchpadManager::addBookmark(const QString &text, const QString &author) {
	if (text.trimmed().isEmpty()) {
		return;
	}
	SavedBookmark b;
	b.id = QString("bmk_%1").arg(base::unixtime::now());
	b.text = text.trimmed();
	b.author = author.trimmed();
	b.date = static_cast<uint64>(base::unixtime::now());
	_bookmarks.push_back(std::move(b));
	save();
	_changed.fire({});
}

void ScratchpadManager::deleteBookmark(const QString &id) {
	const auto it = std::remove_if(_bookmarks.begin(), _bookmarks.end(), [&](const SavedBookmark &b) {
		return b.id == id;
	});
	if (it != _bookmarks.end()) {
		_bookmarks.erase(it, _bookmarks.end());
		save();
		_changed.fire({});
	}
}

rpl::producer<> ScratchpadManager::changed() const {
	return _changed.events();
}

} // namespace AyuData
