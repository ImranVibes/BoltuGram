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

struct MirrorRule {
	QString id;
	QString name;
	uint64 sourcePeerId = 0;
	uint64 targetPeerId = 0;
	bool cleanCopy = true; // strip forwarded badge
	bool removeLinks = true; // remove http/https links
	bool removeMentions = true; // remove @usernames
	QString customHeader;
	QString customFooter;
	bool enabled = true;
};

class AutoMirrorEngine final {
public:
	static AutoMirrorEngine &Instance();

	[[nodiscard]] std::vector<MirrorRule> getRules() const;
	void addRule(const MirrorRule &rule);
	void deleteRule(const QString &id);
	void toggleRule(const QString &id, bool enabled);

	[[nodiscard]] QString processText(const MirrorRule &rule, const QString &inputText) const;

	[[nodiscard]] rpl::producer<> changed() const;

private:
	AutoMirrorEngine();
	void load();
	void save();

	std::vector<MirrorRule> _rules;
	rpl::event_stream<> _changed;
};

} // namespace AyuFeatures
