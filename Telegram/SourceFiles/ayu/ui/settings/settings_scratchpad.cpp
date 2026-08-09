// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/ui/settings/settings_scratchpad.h"

#include "ayu/data/scratchpad_manager.h"
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
#include "ui/boxes/confirm_box.h"
#include "window/window_session_controller.h"

#include <QGuiApplication>
#include <QClipboard>

namespace Settings {

using namespace Builder;
using namespace AyuBuilder;

namespace {

void BuildScratchpadSection(SectionBuilder &builder) {
	const auto controller = builder.controller();
	auto &manager = AyuData::ScratchpadManager::Instance();

	builder.addSubsectionTitle(rpl::single(QString("Quick Templates & Canned Responses")));
	builder.addSkip();

	const auto templates = manager.getTemplates();
	for (const auto &tpl : templates) {
		builder.addButton({
			.id = QString("tpl/%1").arg(tpl.id),
			.title = rpl::single(tpl.title),
			.st = &st::settingsButtonNoIcon,
			.label = rpl::single(tpl.content),
			.onClick = [=] {
				QGuiApplication::clipboard()->setText(tpl.content);
				Ui::Toast::Show(QString("Template copied to clipboard: %1").arg(tpl.title));
			},
		});
	}

	builder.addSkip();
	builder.addDividerText(rpl::single(QString("Click any template to copy it instantly to your clipboard.\nUse Ctrl+Alt+N to toggle the Quick Notes & Scratchpad side drawer anywhere in the app.")));
	builder.addSkip();

	builder.addSubsectionTitle(rpl::single(QString("Saved Bookmarks")));
	const auto bookmarks = manager.getBookmarks();
	if (bookmarks.empty()) {
		builder.addDividerText(rpl::single(QString("No saved bookmarks yet.\nRight-click on any message in a chat and select 'Save to Scratchpad' to bookmark it.")));
	} else {
		for (const auto &bmk : bookmarks) {
			builder.addButton({
				.id = QString("bmk/%1").arg(bmk.id),
				.title = rpl::single(bmk.author.isEmpty() ? QString("Saved Snippet") : bmk.author),
				.st = &st::settingsButtonNoIcon,
				.label = rpl::single(bmk.text),
				.onClick = [=] {
					QGuiApplication::clipboard()->setText(bmk.text);
					Ui::Toast::Show(QString("Bookmark copied to clipboard"));
				},
			});
		}
	}

	builder.addSkip();
}

const auto kMeta = BuildHelper({
	.id = AyuScratchpad::Id(),
	.parentId = AyuMain::Id(),
	.title = [] { return rpl::single(QString("Scratchpad & Templates")); },
	.icon = &st::menuIconEdit,
}, [](SectionBuilder &builder) {
	builder.addSkip();
	BuildScratchpadSection(builder);
});

} // namespace

rpl::producer<QString> AyuScratchpad::title() {
	return rpl::single(QString("Scratchpad & Templates"));
}

AyuScratchpad::AyuScratchpad(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

void AyuScratchpad::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kMeta.build);
	Ui::ResizeFitChild(this, content);
}

Type AyuScratchpadId() {
	return AyuScratchpad::Id();
}

} // namespace Settings
