// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/ui/settings/settings_voice_studio.h"

#include "ayu/features/voice_studio/voice_studio_manager.h"
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

void BuildVoiceStudioSection(SectionBuilder &builder, AyuSectionBuilder &ayu) {
	const auto controller = builder.controller();
	auto &manager = AyuFeatures::VoiceStudioManager::Instance();

	builder.addSubsectionTitle(rpl::single(QString("Voice Effects & Pitch Modulator")));
	builder.addSkip();

	const auto options = std::vector{
		QString("Original (No effect)"),
		QString("Deep Voice (Pitch -4)"),
		QString("Chipmunk / High Pitch (Pitch +4)"),
		QString("Radio / Walkie-Talkie"),
		QString("Robot Modulation"),
		QString("Echo Chamber"),
	};

	ayu.addChooseButton({
		.id = u"ayu/voiceFilterPreset"_q,
		.title = rpl::single(QString("Outgoing Voice Filter")),
		.boxTitle = rpl::single(QString("Choose Voice Filter")),
		.initialSelection = static_cast<int>(manager.currentFilter()),
		.options = options,
		.setter = [](int i) {
			AyuFeatures::VoiceStudioManager::Instance().setFilter(
				static_cast<AyuFeatures::VoiceFilterType>(i));
			Ui::Toast::Show(QString("Voice filter updated"));
		},
		.icon = { &st::menuIconMicrophone },
	});

	builder.addSkip();

	const auto speedOptions = std::vector{
		QString("0.75x (Slower)"),
		QString("1.0x (Normal)"),
		QString("1.25x"),
		QString("1.5x (Fast)"),
		QString("1.75x"),
		QString("2.0x (Double Speed)"),
		QString("3.0x (Ultra Fast)"),
	};

	ayu.addChooseButton({
		.id = u"ayu/voicePlaybackSpeedPreset"_q,
		.title = rpl::single(QString("Voice Playback Speed")),
		.boxTitle = rpl::single(QString("Default Voice Speed")),
		.initialSelection = (manager.playbackSpeed() == 0.75f) ? 0
			: (manager.playbackSpeed() == 1.0f) ? 1
			: (manager.playbackSpeed() == 1.25f) ? 2
			: (manager.playbackSpeed() == 1.5f) ? 3
			: (manager.playbackSpeed() == 1.75f) ? 4
			: (manager.playbackSpeed() == 2.0f) ? 5 : 6,
		.options = speedOptions,
		.setter = [](int i) {
			const float speeds[] = { 0.75f, 1.0f, 1.25f, 1.5f, 1.75f, 2.0f, 3.0f };
			AyuFeatures::VoiceStudioManager::Instance().setPlaybackSpeed(speeds[i]);
		},
		.icon = { &st::menuIconPlay },
	});

	builder.addSkip();
	builder.addDividerText(rpl::single(QString("Selected voice filter will be applied when recording and sending outgoing voice messages and video notes in chats.")));
	builder.addSkip();
}

const auto kMeta = BuildHelper({
	.id = AyuVoiceStudio::Id(),
	.parentId = AyuMain::Id(),
	.title = [] { return rpl::single(QString("Voice Studio & Modulator")); },
	.icon = &st::menuIconMicrophone,
}, [](SectionBuilder &builder) {
	auto ayu = AyuSectionBuilder(builder);
	builder.addSkip();
	BuildVoiceStudioSection(builder, ayu);
});

} // namespace

rpl::producer<QString> AyuVoiceStudio::title() {
	return rpl::single(QString("Voice Studio & Modulator"));
}

AyuVoiceStudio::AyuVoiceStudio(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

void AyuVoiceStudio::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kMeta.build);
	Ui::ResizeFitChild(this, content);
}

Type AyuVoiceStudioId() {
	return AyuVoiceStudio::Id();
}

} // namespace Settings
