#include "Program.h"
#include "MainMenu.h"
#include <SFML/Graphics.hpp>

Program::Program()
{
	window.create(VideoMode(1024, 800), "Course");
}

void Program::Run() {
	window.setVerticalSyncEnabled(true);
	while (window.isOpen()) {
		mainMenu.Run(window);
	}
}