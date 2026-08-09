// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/features/panic_button/panic_controller.h"

#include "core/application.h"
#include "window/window_controller.h"
#include "media/audio/media_audio.h"
#include <QtWidgets/QApplication>

namespace AyuFeatures {

PanicController &PanicController::Instance() {
	static PanicController instance;
	return instance;
}

PanicController::PanicController() = default;

void PanicController::setDecoyType(DecoyType type) {
	_decoyType = type;
}

DecoyType PanicController::getDecoyType() const {
	return _decoyType;
}

void PanicController::triggerPanic() {
	// 1. Mute and stop audio / media playback
	Media::Audio::StopAndClear();

	// 2. Hide and minimize BoltuGram windows
	for (auto widget : QApplication::topLevelWidgets()) {
		if (widget && !widget->inherits("AyuFeatures::DecoyWindow")) {
			widget->hide();
		}
	}

	// 3. Spawn the realistic decoy workstation window
	DecoyWindow::ShowDecoy(_decoyType);
}

} // namespace AyuFeatures
