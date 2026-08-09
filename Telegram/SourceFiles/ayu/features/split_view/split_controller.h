// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#pragma once

#include "data/data_peer.h"

namespace Window {
class SessionController;
} // namespace Window

namespace AyuFeatures {

class SplitController final {
public:
	static void OpenInSplitView(
		not_null<Window::SessionController*> controller,
		not_null<PeerData*> peer);
};

} // namespace AyuFeatures
