// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/data/workspaces_manager.h"

#include "core/application.h"
#include "ayu/libs/json.hpp"
#include "base/unixtime.h"

#include <QtCore/QFile>
#include <fstream>

using json = nlohmann::json;

namespace AyuData {

namespace {

QString getWorkspacesPath() {
	return cWorkingDir() + u"tdata/boltugram_workspaces.json"_q;
}

} // namespace

WorkspacesManager &WorkspacesManager::Instance() {
	static WorkspacesManager instance;
	return instance;
}

WorkspacesManager::WorkspacesManager() {
	load();
	if (_workspaces.empty()) {
		_workspaces.push_back({
			.id = "ws_personal",
			.name = "Personal",
			.iconEmoji = "🏠",
			.colorHex = "#3390ec",
		});
		_workspaces.push_back({
			.id = "ws_work",
			.name = "Work",
			.iconEmoji = "💼",
			.colorHex = "#43a047",
		});
		_workspaces.push_back({
			.id = "ws_crypto",
			.name = "Crypto & Trading",
			.iconEmoji = "📈",
			.colorHex = "#fb8c00",
		});
		_activeWorkspaceId = "ws_personal";
	}
}

void WorkspacesManager::load() {
	_workspaces.clear();
	const auto path = getWorkspacesPath().toStdString();
	std::ifstream file(path);
	if (!file.good()) {
		return;
	}

	try {
		json root;
		file >> root;
		file.close();

		_activeWorkspaceId = QString::fromStdString(root.value("activeWorkspaceId", ""));

		if (root.contains("workspaces") && root["workspaces"].is_array()) {
			for (const auto &item : root["workspaces"]) {
				WorkspaceItem ws;
				ws.id = QString::fromStdString(item.value("id", ""));
				ws.name = QString::fromStdString(item.value("name", ""));
				ws.iconEmoji = QString::fromStdString(item.value("iconEmoji", "📁"));
				ws.colorHex = QString::fromStdString(item.value("colorHex", "#3390ec"));
				ws.accountUserId = item.value("accountUserId", uint64(0));
				ws.proxyConfig = QString::fromStdString(item.value("proxyConfig", ""));
				if (!ws.id.isEmpty() && !ws.name.isEmpty()) {
					_workspaces.push_back(std::move(ws));
				}
			}
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to parse workspaces JSON"));
	}
}

void WorkspacesManager::save() {
	const auto path = getWorkspacesPath().toStdString();
	json root;
	root["activeWorkspaceId"] = _activeWorkspaceId.toStdString();

	json arr = json::array();
	for (const auto &ws : _workspaces) {
		json item;
		item["id"] = ws.id.toStdString();
		item["name"] = ws.name.toStdString();
		item["iconEmoji"] = ws.iconEmoji.toStdString();
		item["colorHex"] = ws.colorHex.toStdString();
		item["accountUserId"] = ws.accountUserId;
		item["proxyConfig"] = ws.proxyConfig.toStdString();
		arr.push_back(std::move(item));
	}
	root["workspaces"] = std::move(arr);

	try {
		std::ofstream file(path);
		if (file.good()) {
			file << root.dump(4);
			file.close();
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to save workspaces JSON"));
	}
}

std::vector<WorkspaceItem> WorkspacesManager::getWorkspaces() const {
	return _workspaces;
}

void WorkspacesManager::addWorkspace(const QString &name, const QString &iconEmoji, const QString &colorHex, uint64 accountUserId) {
	if (name.trimmed().isEmpty()) {
		return;
	}
	WorkspaceItem ws;
	ws.id = QString("ws_%1").arg(base::unixtime::now());
	ws.name = name.trimmed();
	ws.iconEmoji = iconEmoji.trimmed().isEmpty() ? QString("📁") : iconEmoji.trimmed();
	ws.colorHex = colorHex.trimmed().isEmpty() ? QString("#3390ec") : colorHex.trimmed();
	ws.accountUserId = accountUserId;
	_workspaces.push_back(std::move(ws));
	save();
	_changed.fire({});
}

void WorkspacesManager::deleteWorkspace(const QString &id) {
	const auto it = std::remove_if(_workspaces.begin(), _workspaces.end(), [&](const WorkspaceItem &ws) {
		return ws.id == id;
	});
	if (it != _workspaces.end()) {
		_workspaces.erase(it, _workspaces.end());
		if (_activeWorkspaceId == id && !_workspaces.empty()) {
			_activeWorkspaceId = _workspaces.front().id;
		}
		save();
		_changed.fire({});
	}
}

void WorkspacesManager::updateWorkspace(const WorkspaceItem &item) {
	for (auto &ws : _workspaces) {
		if (ws.id == item.id) {
			ws = item;
			save();
			_changed.fire({});
			break;
		}
	}
}

QString WorkspacesManager::activeWorkspaceId() const {
	return _activeWorkspaceId;
}

void WorkspacesManager::setActiveWorkspaceId(const QString &id) {
	if (_activeWorkspaceId == id) {
		return;
	}
	_activeWorkspaceId = id;
	save();
	_changed.fire({});
}

rpl::producer<> WorkspacesManager::changed() const {
	return _changed.events();
}

} // namespace AyuData
