#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include "window.h"

namespace polaris
{
	class MainWindow : public Window
	{
	public:
		MainWindow();
		~MainWindow();
		void Draw() override;
	};
}

static polaris::MainWindow* gpMainWindow;

#endif // MAINWINDOW_H
