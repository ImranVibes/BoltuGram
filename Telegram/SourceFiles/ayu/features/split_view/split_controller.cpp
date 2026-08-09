// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/features/split_view/split_controller.h"

#include "window/window_session_controller.h"
#include "window/window_controller.h"
#include "core/application.h"

namespace AyuFeatures {

void SplitController::OpenInSplitView(
		not_null<Window::SessionController*> controller,
		not_null<PeerData*> peer) {
	// Open the chosen peer in a dedicated split pane / companion window
	controller->showInNewWindow(Window::SeparateId(peer));
}

} // namespace AyuFeatures
