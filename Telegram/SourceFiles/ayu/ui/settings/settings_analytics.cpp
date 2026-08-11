// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/ui/settings/settings_analytics.h"

#include "ayu/features/analytics/analytics_tracker.h"
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

void BuildAnalyticsSummary(SectionBuilder &builder) {
	const auto controller = builder.controller();
	auto &tracker = AyuFeatures::AnalyticsTracker::Instance();

	builder.addSubsectionTitle(rpl::single(QString("Personal Chat Statistics")));
	builder.addSkip();

	const auto msgCount = tracker.totalMessagesSent();
	const auto mediaCount = tracker.totalMediaSent();

	builder.addButton({
		.id = u"stats/totalSent"_q,
		.title = rpl::single(QString("Total Messages Tracked")),
		.st = &st::settingsButtonNoIcon,
		.label = rpl::single(QString::number(msgCount)),
	});

	builder.addButton({
		.id = u"stats/totalMedia"_q,
		.title = rpl::single(QString("Media Files Exchanged")),
		.st = &st::settingsButtonNoIcon,
		.label = rpl::single(QString::number(mediaCount)),
	});

	builder.addSkip();
	builder.addSubsectionTitle(rpl::single(QString("Most Active Chats")));
	builder.addSkip();

	const auto topChats = tracker.topChats();
	if (topChats.empty()) {
		builder.addDividerText(rpl::single(QString("Messaging statistics will automatically accumulate as you send messages and media.")));
	} else {
		for (const auto &cs : topChats) {
			const auto title = cs.peerName.isEmpty() ? QString("Chat #%1").arg(cs.peerId) : cs.peerName;
			const auto subtitle = QString("%1 messages (%2 media)").arg(cs.messageCount).arg(cs.mediaCount);

			builder.addButton({
				.id = QString("chat/%1").arg(cs.peerId),
				.title = rpl::single(title),
				.st = &st::settingsButtonNoIcon,
				.label = rpl::single(subtitle),
			});
		}
	}

	builder.addSkip();
}

const auto kMeta = BuildHelper({
	.id = AyuAnalytics::Id(),
	.parentId = AyuMain::Id(),
	.title = u"Chat Analytics & Stats"_q,
	.icon = &st::menuIconShowInChat,
}, [](SectionBuilder &builder) {
	builder.addSkip();
	BuildAnalyticsSummary(builder);
});

} // namespace

rpl::producer<QString> AyuAnalytics::title() {
	return rpl::single(QString("Chat Analytics & Stats"));
}

AyuAnalytics::AyuAnalytics(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

void AyuAnalytics::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kMeta.build);
	Ui::ResizeFitChild(this, content);
}

Type AyuAnalyticsId() {
	return AyuAnalytics::Id();
}

} // namespace Settings
