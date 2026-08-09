// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/ui/components/scratchpad_drawer.h"

#include "ayu/data/scratchpad_manager.h"
#include "lang/lang_keys.h"
#include "styles/style_boxes.h"
#include "styles/style_layers.h"
#include "styles/style_settings.h"
#include "styles/style_widgets.h"
#include "ui/widgets/fields/input_field.h"
#include "ui/widgets/buttons.h"
#include "ui/painter.h"
#include "ui/toast/toast.h"
#include "window/window_session_controller.h"

#include <QGuiApplication>
#include <QClipboard>

namespace AyuUi {

ScratchpadDrawer::ScratchpadDrawer(QWidget *parent, not_null<Window::SessionController*> controller)
: RpWidget(parent)
, _controller(controller) {
	setupControls();
	hide();
}

ScratchpadDrawer::~ScratchpadDrawer() = default;

void ScratchpadDrawer::setupControls() {
	const auto &manager = AyuData::ScratchpadManager::Instance();

	_editor = Ui::CreateChild<Ui::InputField>(
		this,
		st::defaultInputField,
		rpl::single(QString("Type your quick notes or Markdown here...")),
		manager.getText());

	_editor->changes(
	) | rpl::on_next([=] {
		AyuData::ScratchpadManager::Instance().setText(_editor->getLastText());
	}, _lifetime);

	manager.changed(
	) | rpl::on_next([=] {
		if (_editor && _editor->getLastText() != AyuData::ScratchpadManager::Instance().getText()) {
			_editor->setText(AyuData::ScratchpadManager::Instance().getText());
		}
	}, _lifetime);
}

void ScratchpadDrawer::toggle() {
	if (isHidden()) {
		show();
		raise();
	} else {
		hide();
	}
}

void ScratchpadDrawer::showTab(int tabIndex) {
	_currentTab = tabIndex;
	update();
}

void ScratchpadDrawer::paintEvent(QPaintEvent *e) {
	Painter p(this);
	p.fillRect(rect(), st::windowBg);

	// Header background
	const auto headerRect = QRect(0, 0, width(), 48);
	p.fillRect(headerRect, st::windowBgOver);

	// Header text
	p.setFont(st::semiboldFont);
	p.setPen(st::windowFg);
	p.drawText(16, 30, QString("⚡ Quick Scratchpad & Notes"));

	// Bottom border
	p.setPen(st::shadowFg);
	p.drawLine(0, 48, width(), 48);
	p.drawLine(0, 0, 0, height());
}

void ScratchpadDrawer::resizeEvent(QResizeEvent *e) {
	RpWidget::resizeEvent(e);
	if (_editor) {
		const auto editorMargin = 12;
		_editor->resize(width() - (2 * editorMargin), height() - 60);
		_editor->moveToLeft(editorMargin, 54);
	}
}

} // namespace AyuUi
