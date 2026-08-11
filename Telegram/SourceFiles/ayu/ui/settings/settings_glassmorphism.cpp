// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/ui/settings/settings_glassmorphism.h"

#include "ayu/features/glassmorphism/glass_engine.h"
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

void BuildGlassmorphismSection(SectionBuilder &builder, AyuSectionBuilder &ayu) {
	const auto controller = builder.controller();
	auto &engine = AyuFeatures::GlassEngine::Instance();

	builder.addSubsectionTitle(rpl::single(QString("Windows 11 Mica & Glassmorphism Theme")));
	builder.addSkip();

	const auto materials = std::vector{
		QString("Auto (System Default)"),
		QString("Mica Material"),
		QString("Acrylic Blur (Translucent)"),
		QString("Mica Alt (Tabbed Surface)"),
		QString("Solid (Disabled)"),
	};

	ayu.addChooseButton({
		.id = u"ayu/glassMaterial"_q,
		.title = rpl::single(QString("Window Backdrop Blur")),
		.boxTitle = rpl::single(QString("Choose Glass Backdrop")),
		.initialSelection = static_cast<int>(engine.backdropMaterial()),
		.options = materials,
		.setter = [=](int i) {
			AyuFeatures::GlassEngine::Instance().setBackdropMaterial(
				static_cast<AyuFeatures::BackdropMaterial>(i));
			Ui::Toast::Show(QString("Backdrop material updated"));
		},
		.icon = { &st::menuIconPalette },
	});

	builder.addSkip();

	ayu.addToggle({
		.id = u"ayu/glassAnimatedAccents"_q,
		.title = rpl::single(QString("Glowing Animated Accent Borders")),
		.getter = [=] { return AyuFeatures::GlassEngine::Instance().animatedAccents(); },
		.setter = [=](bool v) { AyuFeatures::GlassEngine::Instance().setAnimatedAccents(v); },
		.icon = { &st::menuIconShowInChat },
	});

	builder.addSkip();
	builder.addDividerText(rpl::single(QString("Mica and Acrylic materials use native Windows 11 DWM hardware acceleration to blend the desktop wallpaper smoothly behind BoltuGram.")));
	builder.addSkip();
}

const auto kMeta = BuildHelper({
	.id = AyuGlassmorphism::Id(),
	.parentId = AyuMain::Id(),
	.title = u"Mica & Glassmorphism"_q,
	.icon = &st::menuIconPalette,
}, [](SectionBuilder &builder) {
	auto ayu = AyuSectionBuilder(builder);
	builder.addSkip();
	BuildGlassmorphismSection(builder, ayu);
});

} // namespace

rpl::producer<QString> AyuGlassmorphism::title() {
	return rpl::single(QString("Mica & Glassmorphism"));
}

AyuGlassmorphism::AyuGlassmorphism(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

void AyuGlassmorphism::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kMeta.build);
	Ui::ResizeFitChild(this, content);
}

Type AyuGlassmorphismId() {
	return AyuGlassmorphism::Id();
}

} // namespace Settings
