// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/ui/settings/settings_panic.h"

#include "ayu/features/panic_button/panic_controller.h"
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

void BuildPanicSection(SectionBuilder &builder, AyuSectionBuilder &ayu) {
	const auto controller = builder.controller();

	builder.addSubsectionTitle(rpl::single(QString("Stealth Mode & Boss Lock")));
	builder.addSkip();

	const auto options = std::vector{
		QString("Excel Spreadsheet (Financial Report)"),
		QString("Visual Studio Code (C++ Workspace)"),
		QString("Notepad (Meeting Action Items)"),
	};

	ayu.addChooseButton({
		.id = u"ayu/panicDecoyType"_q,
		.title = rpl::single(QString("Decoy Screen Type")),
		.boxTitle = rpl::single(QString("Choose Decoy Screen")),
		.initialSelection = static_cast<int>(AyuFeatures::PanicController::Instance().getDecoyType()),
		.options = options,
		.setter = [](int i) {
			AyuFeatures::PanicController::Instance().setDecoyType(static_cast<AyuFeatures::DecoyType>(i));
		},
		.icon = { &st::menuIconLock },
	});

	builder.addSkip();
	builder.addButton({
		.id = u"ayu/testPanicAction"_q,
		.title = rpl::single(QString("Test Stealth Decoy Now")),
		.icon = { &st::menuIconShowInChat },
		.onClick = [=] {
			AyuFeatures::PanicController::Instance().triggerPanic();
		},
	});

	builder.addSkip();
	builder.addDividerText(rpl::single(QString("Triggering Stealth Mode instantly mutes all audio/video playback, hides BoltuGram, and opens a full realistic decoy window (e.g. Excel spreadsheet).\nPress Esc at any time inside the decoy screen to unlock and restore BoltuGram.")));
	builder.addSkip();
}

const auto kMeta = BuildHelper({
	.id = AyuPanic::Id(),
	.parentId = AyuMain::Id(),
	.title = [] { return rpl::single(QString("Panic Button & Stealth Lock")); },
	.icon = &st::menuIconLock,
}, [](SectionBuilder &builder) {
	auto ayu = AyuSectionBuilder(builder);
	builder.addSkip();
	BuildPanicSection(builder, ayu);
});

} // namespace

rpl::producer<QString> AyuPanic::title() {
	return rpl::single(QString("Panic Button & Stealth Lock"));
}

AyuPanic::AyuPanic(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

void AyuPanic::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kMeta.build);
	Ui::ResizeFitChild(this, content);
}

Type AyuPanicId() {
	return AyuPanic::Id();
}

} // namespace Settings
