// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/ui/settings/settings_crm.h"

#include "ayu/data/user_crm.h"
#include "ayu/ui/boxes/edit_user_crm_box.h"
#include "ayu/ui/settings/ayu_builder.h"
#include "ayu/ui/settings/settings_main.h"
#include "data/data_session.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
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

void BuildCrmList(SectionBuilder &builder) {
	const auto controller = builder.controller();
	const auto session = &controller->session();

	builder.addSubsectionTitle(rpl::single(QString("Contact CRM & Private Notes")));
	builder.addSkip();

	const auto entries = AyuData::UserCrm::Instance().getAll();

	if (entries.empty()) {
		builder.addDividerText(rpl::single(QString("No private tags or notes yet.\nRight-click on any user or group and select 'Tag & Private Note' to add one.")));
	} else {
		for (const auto &info : entries) {
			const auto peer = session->data().peerLoaded(info.peerId);
			const auto title = peer ? peer->name() : QString("User #%1").arg(info.peerId);
			const auto subtitle = info.tag.isEmpty() ? info.note : QString("[%1] %2").arg(info.tag, info.note);

			builder.addButton({
				.id = QString("crm/%1").arg(info.peerId),
				.title = rpl::single(title),
				.st = &st::settingsButtonNoIcon,
				.label = rpl::single(subtitle),
				.onClick = [=] {
					if (peer) {
						controller->show(Box<AyuUi::EditUserCrmBox>(peer));
					}
				},
			});
		}
	}

	builder.addSkip();
}

const auto kMeta = BuildHelper({
	.id = AyuCrm::Id(),
	.parentId = AyuMain::Id(),
	.title = [] { return rpl::single(QString("Contact CRM & Notes")); },
	.icon = &st::menuIconAddToFolder,
}, [](SectionBuilder &builder) {
	builder.addSkip();
	BuildCrmList(builder);
});

} // namespace

rpl::producer<QString> AyuCrm::title() {
	return rpl::single(QString("Contact CRM & Notes"));
}

AyuCrm::AyuCrm(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

void AyuCrm::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kMeta.build);
	Ui::ResizeFitChild(this, content);
}

Type AyuCrmId() {
	return AyuCrm::Id();
}

} // namespace Settings
