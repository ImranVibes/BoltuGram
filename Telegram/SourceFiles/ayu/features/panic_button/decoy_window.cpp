// This is the source code of BoltuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2026
#include "ayu/features/panic_button/decoy_window.h"

#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QApplication>

namespace AyuFeatures {

namespace {

DecoyWindow *activeDecoyInstance = nullptr;

} // namespace

DecoyWindow::DecoyWindow(DecoyType type, QWidget *parent)
: QWidget(parent)
, _type(type) {
	setAttribute(Qt::WA_DeleteOnClose);
	setMinimumSize(960, 640);
	resize(1180, 780);

	switch (_type) {
	case DecoyType::Excel:
		setWindowTitle("Q3_Financial_Budget_Forecast_2026.xlsx - Excel");
		break;
	case DecoyType::VSCode:
		setWindowTitle("main.cpp - BoltuEngine - Visual Studio Code");
		break;
	case DecoyType::Notepad:
		setWindowTitle("Meeting_Action_Items_Aug_2026.txt - Notepad");
		break;
	}
}

void DecoyWindow::ShowDecoy(DecoyType type) {
	if (activeDecoyInstance) {
		activeDecoyInstance->close();
		activeDecoyInstance = nullptr;
	}

	activeDecoyInstance = new DecoyWindow(type);
	activeDecoyInstance->show();
	activeDecoyInstance->raise();
	activeDecoyInstance->activateWindow();
}

void DecoyWindow::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_Escape) {
		close();
		activeDecoyInstance = nullptr;
	} else {
		QWidget::keyPressEvent(e);
	}
}

void DecoyWindow::mousePressEvent(QMouseEvent *e) {
	// Clicking bottom-right lock icon area closes decoy
	if (e->pos().x() > width() - 40 && e->pos().y() > height() - 40) {
		close();
		activeDecoyInstance = nullptr;
	} else {
		QWidget::mousePressEvent(e);
	}
}

void DecoyWindow::paintEvent(QPaintEvent *e) {
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing, true);

	switch (_type) {
	case DecoyType::Excel:
		paintExcelDecoy(p);
		break;
	case DecoyType::VSCode:
		paintVSCodeDecoy(p);
		break;
	case DecoyType::Notepad:
		paintNotepadDecoy(p);
		break;
	}
}

void DecoyWindow::paintExcelDecoy(QPainter &p) {
	// Excel Header Green Bar
	p.fillRect(0, 0, width(), 40, QColor(0x10, 0x7C, 0x41));
	p.setPen(Qt::white);
	p.setFont(QFont("Segoe UI", 10, QFont::DemiBold));
	p.drawText(20, 26, "AutoSave [ON]   Q3_Financial_Budget_Forecast_2026.xlsx - Excel");

	// Ribbon Tabs Bar
	p.fillRect(0, 40, width(), 35, QColor(0xF3, 0xF2, 0xF1));
	p.setPen(QColor(0x32, 0x31, 0x30));
	p.setFont(QFont("Segoe UI", 9));
	const QStringList tabs = { "File", "Home", "Insert", "Page Layout", "Formulas", "Data", "Review", "View", "Automate" };
	int tx = 20;
	for (const auto &tab : tabs) {
		if (tab == "Home") {
			p.fillRect(tx - 6, 40, 52, 35, Qt::white);
			p.setPen(QColor(0x10, 0x7C, 0x41));
			p.drawText(tx, 62, tab);
			p.fillRect(tx - 6, 73, 52, 2, QColor(0x10, 0x7C, 0x41));
			p.setPen(QColor(0x32, 0x31, 0x30));
		} else {
			p.drawText(tx, 62, tab);
		}
		tx += 68;
	}

	// Formula Bar
	p.fillRect(0, 75, width(), 32, QColor(0xF9, 0xF9, 0xF9));
	p.setPen(QColor(0xD2, 0xD0, 0xCE));
	p.drawLine(0, 107, width(), 107);
	p.setPen(QColor(0x60, 0x5E, 0x5C));
	p.drawText(15, 96, "B4");
	p.drawLine(45, 78, 45, 104);
	p.drawText(55, 96, "fx  =SUM(C4:G4) * $K$2");

	// Spreadsheet Grid
	const auto colWidth = 105;
	const auto rowHeight = 24;
	const auto topGrid = 108;

	// Column Headers (A, B, C, D...)
	p.fillRect(0, topGrid, 40, rowHeight, QColor(0xE1, 0xDF, 0xDD));
	p.fillRect(40, topGrid, width() - 40, rowHeight, QColor(0xF3, 0xF2, 0xF1));
	p.setPen(QColor(0xD2, 0xD0, 0xCE));
	p.drawLine(0, topGrid + rowHeight, width(), topGrid + rowHeight);

	p.setPen(QColor(0x32, 0x31, 0x30));
	p.setFont(QFont("Segoe UI", 9, QFont::DemiBold));
	const QStringList colNames = { "A: Category", "B: Q1 Actual", "C: Q2 Actual", "D: Q3 Forecast", "E: Variance", "F: Growth %", "G: Status" };
	int cx = 40;
	for (const auto &cname : colNames) {
		p.drawText(cx + 8, topGrid + 16, cname);
		p.drawLine(cx, topGrid, cx, height() - 25);
		cx += colWidth;
	}

	// Spreadsheet Sample Data Rows
	struct RowData { QString cat; QString q1; QString q2; QString q3; QString var; QString grow; QString status; };
	const std::vector<RowData> data = {
		{ "Cloud Infrastructure", "$14,850.00", "$16,200.00", "$15,900.00", "+$1,050", "+7.2%", "Optimal" },
		{ "Developer Tooling", "$8,400.00", "$8,950.00", "$9,100.00", "+$700", "+8.3%", "Active" },
		{ "Security & Compliance", "$12,300.00", "$12,300.00", "$12,500.00", "+$200", "+1.6%", "Compliant" },
		{ "API & Data Pipeline", "$6,750.00", "$7,100.00", "$7,450.00", "+$700", "+10.3%", "Growing" },
		{ "QA & Test Automation", "$5,200.00", "$5,400.00", "$5,350.00", "+$150", "+2.8%", "Normal" },
		{ "Marketing & Outreach", "$21,000.00", "$19,800.00", "$22,500.00", "+$1,500", "+7.1%", "Approved" },
		{ "Customer Support Ops", "$11,200.00", "$11,500.00", "$11,800.00", "+$600", "+5.3%", "Staffed" },
		{ "Hardware & Workstations", "$9,800.00", "$4,200.00", "$6,500.00", "-$3,300", "-33.6%", "Planned" },
		{ "Legal & Trademark", "$3,500.00", "$3,500.00", "$3,500.00", "$0", "0.0%", "Retained" },
		{ "TOTAL OPERATING EXP", "$93,000.00", "$88,950.00", "$94,600.00", "+$1,600", "+1.7%", "Balanced" }
	};

	p.setFont(QFont("Segoe UI", 9));
	int ry = topGrid + rowHeight;
	int rowNum = 1;

	for (const auto &item : data) {
		// Row Header
		p.fillRect(0, ry, 40, rowHeight, QColor(0xF3, 0xF2, 0xF1));
		p.setPen(QColor(0x60, 0x5E, 0x5C));
		p.drawText(QRect(0, ry, 36, rowHeight), Qt::AlignRight | Qt::AlignVCenter, QString::number(rowNum++));

		// Row Grid Lines
		p.setPen(QColor(0xEA, 0xE8, 0xE6));
		p.drawLine(40, ry + rowHeight, width(), ry + rowHeight);

		// Cell text
		p.setPen(rowNum == 11 ? QColor(0x10, 0x7C, 0x41) : QColor(0x32, 0x31, 0x30));
		if (rowNum == 11) p.setFont(QFont("Segoe UI", 9, QFont::Bold));

		int x = 40;
		p.drawText(x + 8, ry + 16, item.cat); x += colWidth;
		p.drawText(x + 8, ry + 16, item.q1); x += colWidth;
		p.drawText(x + 8, ry + 16, item.q2); x += colWidth;
		p.drawText(x + 8, ry + 16, item.q3); x += colWidth;
		p.drawText(x + 8, ry + 16, item.var); x += colWidth;
		p.drawText(x + 8, ry + 16, item.grow); x += colWidth;
		p.drawText(x + 8, ry + 16, item.status);

		ry += rowHeight;
	}

	// Status Bar
	p.fillRect(0, height() - 25, width(), 25, QColor(0x10, 0x7C, 0x41));
	p.setPen(Qt::white);
	p.setFont(QFont("Segoe UI", 8));
	p.drawText(15, height() - 8, "Ready   |   Sheet1   |   Average: $13,514   Count: 10   Sum: $94,600.00   |   Press Esc to unlock");
}

void DecoyWindow::paintVSCodeDecoy(QPainter &p) {
	// VS Code Dark Theme Background
	p.fillRect(rect(), QColor(0x1E, 0x1E, 0x1E));

	// Sidebar Activity Bar
	p.fillRect(0, 0, 48, height(), QColor(0x33, 0x33, 0x33));

	// Editor Tab Bar
	p.fillRect(48, 0, width() - 48, 35, QColor(0x25, 0x25, 0x26));
	p.fillRect(48, 0, 140, 35, QColor(0x1E, 0x1E, 0x1E));
	p.setPen(Qt::white);
	p.setFont(QFont("Segoe UI", 9));
	p.drawText(65, 22, "main.cpp");

	// Line numbers & Code
	p.setFont(QFont("Consolas", 10));
	const QStringList code = {
		"#include <iostream>",
		"#include <vector>",
		"#include <memory>",
		"#include <algorithm>",
		"",
		"namespace Engine {",
		"",
		"class TaskPipeline final {",
		"public:",
		"    explicit TaskPipeline(size_t workerThreads)",
		"        : _workerCount(workerThreads) {",
		"        initWorkers();",
		"    }",
		"",
		"    void enqueueJob(std::function<void()> job) {",
		"        std::lock_guard<std::mutex> lock(_queueMutex);",
		"        _taskQueue.push_back(std::move(job));",
		"        _cv.notify_one();",
		"    }",
		"",
		"private:",
		"    size_t _workerCount{ 8 };",
		"    std::mutex _queueMutex;",
		"    std::condition_variable _cv;",
		"};",
		"",
		"} // namespace Engine",
		"",
		"int main(int argc, char* argv[]) {",
		"    Engine::TaskPipeline pipeline(16);",
		"    std::cout << \"Service operational. Processing batches...\\n\";",
		"    return 0;",
		"}"
	};

	int ly = 60;
	int lineNum = 1;
	for (const auto &line : code) {
		p.setPen(QColor(0x85, 0x85, 0x85));
		p.drawText(55, ly, QString::number(lineNum++).rightJustified(3, ' '));

		p.setPen(line.startsWith("#") ? QColor(0xC5, 0x86, 0xC0)
			: line.contains("class") || line.contains("namespace") || line.contains("int ") ? QColor(0x56, 0x9C, 0xD6)
			: line.contains("\"") ? QColor(0xCE, 0x91, 0x78) : QColor(0xD4, 0xD4, 0xD4));
		p.drawText(100, ly, line);
		ly += 20;
	}

	// Status Bar
	p.fillRect(0, height() - 22, width(), 22, QColor(0x00, 0x7A, 0xCC));
	p.setPen(Qt::white);
	p.setFont(QFont("Segoe UI", 8));
	p.drawText(15, height() - 6, "master*   Ln 32, Col 1   Spaces: 4   UTF-8   C++   [Press Esc to unlock]");
}

void DecoyWindow::paintNotepadDecoy(QPainter &p) {
	p.fillRect(rect(), Qt::white);

	// Menu Bar
	p.fillRect(0, 0, width(), 25, QColor(0xF0, 0xF0, 0xF0));
	p.setPen(QColor(0x30, 0x30, 0x30));
	p.setFont(QFont("Segoe UI", 9));
	p.drawText(10, 17, "File    Edit    Format    View    Help");

	// Text area
	p.setFont(QFont("Consolas", 10));
	p.setPen(Qt::black);
	const QStringList notes = {
		"MEETING ACTION ITEMS - PRODUCTIVITY & SPRINT REVIEW",
		"==================================================",
		"Date: August 9, 2026",
		"Attendees: Product Lead, Architecture Team, DevOps",
		"",
		"1. Review Q3 infrastructure performance metrics.",
		"2. Complete final testing on release builds.",
		"3. Finalize data synchronization pipeline benchmarks.",
		"4. Schedule security audit for upcoming quarterly sign-off.",
		"",
		"Notes: Next sync scheduled for Tuesday at 10:00 AM.",
		"",
		"[Press Esc to unlock]"
	};

	int y = 50;
	for (const auto &n : notes) {
		p.drawText(15, y, n);
		y += 22;
	}
}

} // namespace AyuFeatures
