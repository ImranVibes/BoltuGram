// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/features/glassmorphism/glass_engine.h"

#include "core/application.h"
#include "ayu/libs/json.hpp"

#ifdef Q_OS_WIN
#include <windows.h>
#include <dwmapi.h>
#endif

#include <QtCore/QFile>
#include <fstream>

using json = nlohmann::json;

namespace AyuFeatures {

namespace {

QString getGlassEnginePath() {
	return cWorkingDir() + u"tdata/boltugram_glassmorphism.json"_q;
}

} // namespace

GlassEngine &GlassEngine::Instance() {
	static GlassEngine instance;
	return instance;
}

GlassEngine::GlassEngine() {
	load();
}

void GlassEngine::load() {
	const auto path = getGlassEnginePath().toStdString();
	std::ifstream file(path);
	if (!file.good()) {
		return;
	}

	try {
		json root;
		file >> root;
		file.close();

		_material = static_cast<BackdropMaterial>(root.value("material", 1));
		_animatedAccents = root.value("animatedAccents", true);
		_glassOpacity = root.value("glassOpacity", 85);
	} catch (...) {
		LOG(("BoltuGram: Failed to parse glassmorphism config"));
	}
}

void GlassEngine::save() {
	const auto path = getGlassEnginePath().toStdString();
	json root;
	root["material"] = static_cast<int>(_material);
	root["animatedAccents"] = _animatedAccents;
	root["glassOpacity"] = _glassOpacity;

	try {
		std::ofstream file(path);
		if (file.good()) {
			file << root.dump(4);
			file.close();
		}
	} catch (...) {
		LOG(("BoltuGram: Failed to save glassmorphism config"));
	}
}

BackdropMaterial GlassEngine::backdropMaterial() const {
	return _material;
}

void GlassEngine::setBackdropMaterial(BackdropMaterial material) {
	if (_material == material) {
		return;
	}
	_material = material;
	save();
	_changed.fire({});
}

bool GlassEngine::animatedAccents() const {
	return _animatedAccents;
}

void GlassEngine::setAnimatedAccents(bool enabled) {
	if (_animatedAccents == enabled) {
		return;
	}
	_animatedAccents = enabled;
	save();
	_changed.fire({});
}

int GlassEngine::glassOpacity() const {
	return _glassOpacity;
}

void GlassEngine::setGlassOpacity(int percent) {
	if (_glassOpacity == percent) {
		return;
	}
	_glassOpacity = percent;
	save();
	_changed.fire({});
}

void GlassEngine::applyWindowBackdrop(QWidget *window) {
#ifdef Q_OS_WIN
	if (!window) {
		return;
	}
	const auto hwnd = reinterpret_cast<HWND>(window->winId());
	if (!hwnd) {
		return;
	}

	// DWMWA_SYSTEMBACKDROP_TYPE = 38
	constexpr DWORD DWMWA_SYSTEMBACKDROP_TYPE_VAL = 38;
	DWORD backdrop = 0;
	switch (_material) {
	case BackdropMaterial::Auto: backdrop = 1; break;
	case BackdropMaterial::Mica: backdrop = 2; break;
	case BackdropMaterial::Acrylic: backdrop = 3; break;
	case BackdropMaterial::MicaAlt: backdrop = 4; break;
	case BackdropMaterial::None: backdrop = 1; break;
	}

	DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE_VAL, &backdrop, sizeof(backdrop));
#endif
}

rpl::producer<> GlassEngine::changed() const {
	return _changed.events();
}

} // namespace AyuFeatures
