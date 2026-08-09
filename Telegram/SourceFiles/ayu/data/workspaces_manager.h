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

struct WorkspaceItem {
	QString id;
	QString name;
	QString iconEmoji;
	QString colorHex;
	uint64 accountUserId = 0;
	QString proxyConfig;
};

class WorkspacesManager final {
public:
	static WorkspacesManager &Instance();

	[[nodiscard]] std::vector<WorkspaceItem> getWorkspaces() const;
	void addWorkspace(const QString &name, const QString &iconEmoji, const QString &colorHex, uint64 accountUserId = 0);
	void deleteWorkspace(const QString &id);
	void updateWorkspace(const WorkspaceItem &item);

	[[nodiscard]] QString activeWorkspaceId() const;
	void setActiveWorkspaceId(const QString &id);

	[[nodiscard]] rpl::producer<> changed() const;

private:
	WorkspacesManager();
	void load();
	void save();

	std::vector<WorkspaceItem> _workspaces;
	QString _activeWorkspaceId;
	rpl::event_stream<> _changed;
};

} // namespace AyuData
