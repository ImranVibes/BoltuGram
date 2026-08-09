// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#pragma once

#include "ui/rp_widget.h"
#include "base/object_ptr.h"
#include "rpl/lifetime.h"

namespace Window {
class SessionController;
} // namespace Window

namespace Ui {
class InputField;
class FlatButton;
class PlainShadow;
} // namespace Ui

namespace AyuUi {

class ScratchpadDrawer final : public Ui::RpWidget {
public:
	ScratchpadDrawer(QWidget *parent, not_null<Window::SessionController*> controller);
	~ScratchpadDrawer();

	void toggle();
	void showTab(int tabIndex);

protected:
	void paintEvent(QPaintEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;

private:
	void setupControls();
	void refreshTemplatesList();
	void refreshBookmarksList();

	const not_null<Window::SessionController*> _controller;
	int _currentTab = 0; // 0: Scratchpad, 1: Templates, 2: Bookmarks

	Ui::InputField *_editor = nullptr;
	QWidget *_templatesContainer = nullptr;
	QWidget *_bookmarksContainer = nullptr;
	std::vector<Ui::RpWidget*> _tabButtons;
	rpl::lifetime _lifetime;
};

} // namespace AyuUi
