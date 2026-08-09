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

namespace AyuData {

struct CannedTemplate {
	QString id;
	QString title;
	QString content;
};

struct SavedBookmark {
	QString id;
	QString text;
	QString author;
	uint64 date = 0;
};

class ScratchpadManager final {
public:
	static ScratchpadManager &Instance();

	[[nodiscard]] QString getText() const;
	void setText(const QString &text);

	[[nodiscard]] std::vector<CannedTemplate> getTemplates() const;
	void addTemplate(const QString &title, const QString &content);
	void deleteTemplate(const QString &id);

	[[nodiscard]] std::vector<SavedBookmark> getBookmarks() const;
	void addBookmark(const QString &text, const QString &author);
	void deleteBookmark(const QString &id);

	[[nodiscard]] rpl::producer<> changed() const;

private:
	ScratchpadManager();
	void load();
	void save();

	QString _text;
	std::vector<CannedTemplate> _templates;
	std::vector<SavedBookmark> _bookmarks;
	rpl::event_stream<> _changed;
};

} // namespace AyuData
