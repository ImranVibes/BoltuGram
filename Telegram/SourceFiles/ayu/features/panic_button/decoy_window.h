// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#pragma once

#include <QtWidgets/QWidget>

namespace AyuFeatures {

enum class DecoyType {
	Excel = 0,
	VSCode = 1,
	Notepad = 2,
};

class DecoyWindow final : public QWidget {
public:
	explicit DecoyWindow(DecoyType type, QWidget *parent = nullptr);

	static void ShowDecoy(DecoyType type);

protected:
	void paintEvent(QPaintEvent *e) override;
	void keyPressEvent(QKeyEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;

private:
	void paintExcelDecoy(QPainter &p);
	void paintVSCodeDecoy(QPainter &p);
	void paintNotepadDecoy(QPainter &p);

	DecoyType _type = DecoyType::Excel;
};

} // namespace AyuFeatures
