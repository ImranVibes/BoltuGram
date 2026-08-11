// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/ui/boxes/edit_user_crm_box.h"

#include "ayu/data/user_crm.h"
#include "lang/lang_keys.h"
#include "styles/style_boxes.h"
#include "styles/style_layers.h"
#include "styles/style_widgets.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/painter.h"
#include "ui/toast/toast.h"

namespace AyuUi {

EditUserCrmBox::EditUserCrmBox(QWidget *, not_null<PeerData*> peer)
: _peer(peer) {
	_palette = {
		"#3390ec", // Blue
		"#43a047", // Green
		"#e53935", // Red
		"#fb8c00", // Orange
		"#8e24aa", // Purple
		"#00acc1", // Cyan
		"#e91e63", // Pink
		"#607d8b"  // Slate
	};
}

void EditUserCrmBox::prepare() {
	setTitle(rpl::single(QString("Private Note & Tag")));

	const auto existing = AyuData::UserCrm::Instance().get(_peer->id.value);
	const auto initialTag = existing ? existing->tag : QString();
	const auto initialNote = existing ? existing->note : QString();
	if (existing && !existing->colorHex.isEmpty()) {
		_selectedColorHex = existing->colorHex;
	}

	_tagInput = Ui::CreateChild<Ui::InputField>(
		this,
		st::defaultInputField,
		rpl::single(QString("Tag (e.g. VIP, Client, Friend, Dev)")),
		initialTag);

	_noteInput = Ui::CreateChild<Ui::InputField>(
		this,
		st::defaultInputField,
		rpl::single(QString("Private Note (only visible to you)")),
		initialNote);

	addButton(rpl::single(QString("Save")), [=] { save(); });
	addButton(tr::lng_cancel(), [=] { closeBox(); });

	if (existing) {
		addLeftButton(rpl::single(QString("Delete")), [=] { remove(); });
	}

	setDimensions(st::boxWidth, 230);
}

void EditUserCrmBox::resizeEvent(QResizeEvent *e) {
	BoxContent::resizeEvent(e);

	const auto inputWidth = width() - st::boxPadding.left() - st::boxPadding.right();
	const auto left = st::boxPadding.left();

	if (_tagInput) {
		_tagInput->resize(inputWidth, _tagInput->height());
		_tagInput->moveToLeft(left, st::boxPadding.top() + 10);
	}

	if (_noteInput) {
		_noteInput->resize(inputWidth, _noteInput->height());
		_noteInput->moveToLeft(left, st::boxPadding.top() + 65);
	}
}

void EditUserCrmBox::save() {
	const auto tag = _tagInput ? _tagInput->getLastText().trimmed() : QString();
	const auto note = _noteInput ? _noteInput->getLastText().trimmed() : QString();

	if (tag.isEmpty() && note.isEmpty()) {
		AyuData::UserCrm::Instance().remove(_peer->id.value);
	} else {
		AyuData::UserCrm::Instance().set(_peer->id.value, tag, _selectedColorHex, note);
	}

	Ui::Toast::Show(QString("Tag & Note saved successfully"));
	closeBox();
}

void EditUserCrmBox::remove() {
	AyuData::UserCrm::Instance().remove(_peer->id.value);
	Ui::Toast::Show(QString("Tag & Note removed"));
	closeBox();
}

} // namespace AyuUi
