// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#pragma once

#include <QtCore/QString>
#include <QtWidgets/QWidget>
#include "rpl/producer.h"
#include "rpl/event_stream.h"

namespace AyuFeatures {

enum class BackdropMaterial {
	Auto = 0,
	Mica = 1,
	Acrylic = 2,
	MicaAlt = 3,
	None = 4,
};

class GlassEngine final {
public:
	static GlassEngine &Instance();

	[[nodiscard]] BackdropMaterial backdropMaterial() const;
	void setBackdropMaterial(BackdropMaterial material);

	[[nodiscard]] bool animatedAccents() const;
	void setAnimatedAccents(bool enabled);

	[[nodiscard]] int glassOpacity() const;
	void setGlassOpacity(int percent);

	void applyWindowBackdrop(QWidget *window);

	[[nodiscard]] rpl::producer<> changed() const;

private:
	GlassEngine();
	void load();
	void save();

	BackdropMaterial _material = BackdropMaterial::Mica;
	bool _animatedAccents = true;
	int _glassOpacity = 85; // 85% opacity
	rpl::event_stream<> _changed;
};

} // namespace AyuFeatures
