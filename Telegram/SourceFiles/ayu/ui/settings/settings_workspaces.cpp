// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/ui/settings/settings_workspaces.h"

#include "ayu/data/workspaces_manager.h"
#include "ayu/ui/settings/ayu_builder.h"
#include "ayu/ui/settings/settings_main.h"
#include "lang/lang_keys.h"
#include "settings/settings_builder.h"
#include "settings/settings_common.h"
#include "styles/style_ayu_icons.h"
#include "styles/style_menu_icons.h"
#include "styles/style_settings.h"
#include "ui/toast/toast.h"
#include "ui/widgets/buttons.h"
#include "ui/wrap/vertical_layout.h"
#include "window/window_session_controller.h"

namespace Settings {

using namespace Builder;
using namespace AyuBuilder;

namespace {

void BuildWorkspacesList(SectionBuilder &builder) {
	const auto controller = builder.controller();
	auto &manager = AyuData::WorkspacesManager::Instance();

	builder.addSubsectionTitle(rpl::single(QString("Workspaces & Profile Tabs")));
	builder.addSkip();

	const auto workspaces = manager.getWorkspaces();
	for (const auto &ws : workspaces) {
		const auto isCurrent = (ws.id == manager.activeWorkspaceId());
		const auto title = QString("%1 %2%3").arg(ws.iconEmoji, ws.name, isCurrent ? QString("  ✓ (Active)") : QString());

		builder.addButton({
			.id = QString("ws/%1").arg(ws.id),
			.title = rpl::single(title),
			.st = &st::settingsButtonNoIcon,
			.label = rpl::single(QString("Switch Workspace")),
			.onClick = [=] {
				AyuData::WorkspacesManager::Instance().setActiveWorkspaceId(ws.id);
				Ui::Toast::Show(QString("Switched to workspace: %1").arg(ws.name));
			},
		});
	}

	builder.addSkip();
	builder.addDividerText(rpl::single(QString("Workspaces allow you to group your accounts, chat folders, and dedicated proxy connections.\nUse Ctrl+1, Ctrl+2, and Ctrl+3 to quickly switch workspaces anywhere.")));
	builder.addSkip();
}

const auto kMeta = BuildHelper({
	.id = AyuWorkspaces::Id(),
	.parentId = AyuMain::Id(),
	.title = u"Workspaces & Profiles"_q,
	.icon = &st::menuIconAddToFolder,
}, [](SectionBuilder &builder) {
	builder.addSkip();
	BuildWorkspacesList(builder);
});

} // namespace

rpl::producer<QString> AyuWorkspaces::title() {
	return rpl::single(QString("Workspaces & Profiles"));
}

AyuWorkspaces::AyuWorkspaces(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

void AyuWorkspaces::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kMeta.build);
	Ui::ResizeFitChild(this, content);
}

Type AyuWorkspacesId() {
	return AyuWorkspaces::Id();
}

} // namespace Settings
