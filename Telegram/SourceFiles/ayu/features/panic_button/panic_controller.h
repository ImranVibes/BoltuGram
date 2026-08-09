// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#pragma once

#include "ayu/features/panic_button/decoy_window.h"

namespace AyuFeatures {

class PanicController final {
public:
	static PanicController &Instance();

	void triggerPanic();
	void setDecoyType(DecoyType type);
	[[nodiscard]] DecoyType getDecoyType() const;

private:
	PanicController();
	DecoyType _decoyType = DecoyType::Excel;
};

} // namespace AyuFeatures
