// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/ui/settings/settings_auto_mirror.h"

#include "ayu/features/forward/auto_mirror_engine.h"
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

void BuildAutoMirrorList(SectionBuilder &builder) {
	const auto controller = builder.controller();
	auto &engine = AyuFeatures::AutoMirrorEngine::Instance();

	builder.addSubsectionTitle(rpl::single(QString("Channel Auto-Mirror & Forwarder")));
	builder.addSkip();

	const auto rules = engine.getRules();
	if (rules.empty()) {
		builder.addDividerText(rpl::single(QString("No active auto-mirror rules.\nAuto-Mirror allows automatically re-posting messages from any source channel to your destination channel with ad/link removal and custom clean headers.")));
	} else {
		for (const auto &rule : rules) {
			const auto title = QString("%1 (%2)").arg(rule.name, rule.enabled ? QString("Active") : QString("Paused"));
			const auto subtitle = QString("Clean Copy: %1 | Strip Links: %2")
				.arg(rule.cleanCopy ? QString("Yes") : QString("No"))
				.arg(rule.removeLinks ? QString("Yes") : QString("No"));

			builder.addButton({
				.id = QString("rule/%1").arg(rule.id),
				.title = rpl::single(title),
				.st = &st::settingsButtonNoIcon,
				.label = rpl::single(subtitle),
				.onClick = [=] {
					AyuFeatures::AutoMirrorEngine::Instance().toggleRule(rule.id, !rule.enabled);
					Ui::Toast::Show(QString("Rule status updated"));
				},
			});
		}
	}

	builder.addSkip();
	builder.addDividerText(rpl::single(QString("Real-time forwarding processes incoming messages and media instantly. Link and mention strippers clean forwarded ad content automatically.")));
	builder.addSkip();
}

const auto kMeta = BuildHelper({
	.id = AyuAutoMirror::Id(),
	.parentId = AyuMain::Id(),
	.title = u"Channel Auto-Mirror"_q,
	.icon = &st::menuIconChannel,
}, [](SectionBuilder &builder) {
	builder.addSkip();
	BuildAutoMirrorList(builder);
});

} // namespace

rpl::producer<QString> AyuAutoMirror::title() {
	return rpl::single(QString("Channel Auto-Mirror"));
}

AyuAutoMirror::AyuAutoMirror(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

void AyuAutoMirror::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kMeta.build);
	Ui::ResizeFitChild(this, content);
}

Type AyuAutoMirrorId() {
	return AyuAutoMirror::Id();
}

} // namespace Settings
