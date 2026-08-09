// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#pragma once

#include "ui/layers/box_content.h"
#include "data/data_peer.h"

namespace Ui {
class InputField;
class FlatButton;
} // namespace Ui

namespace AyuUi {

class EditUserCrmBox final : public Ui::BoxContent {
public:
	EditUserCrmBox(QWidget *parent, not_null<PeerData*> peer);

protected:
	void prepare() override;
	void resizeEvent(QResizeEvent *e) override;

private:
	void save();
	void remove();

	const not_null<PeerData*> _peer;
	Ui::InputField *_tagInput = nullptr;
	Ui::InputField *_noteInput = nullptr;
	QString _selectedColorHex = "#3390ec";
	std::vector<QString> _palette;
};

} // namespace AyuUi
