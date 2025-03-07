#include "AboutWindow.h"

#include <Editor/Windows/Interface/QtInput.h>

#include <Manager/Manager.h>
#include <Manager/EventSystem.h>

// QT
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>
#include <QPlainTextEdit>

AboutWindow::AboutWindow()
{
	LoadStyleSheet("about-window.css");
	setWindowTitle("About");
	SetIcon("chat.png");
	SetOnTop();

	setFixedSize(300, 150);

	InitUI();
}

void AboutWindow::InitUI()
{
	qtLayout->setSpacing(5);
	qtLayout->setContentsMargins(5, 5, 5, 5);
	qtLayout->setAlignment(Qt::AlignTop);

}