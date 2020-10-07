#include "Program.h"
#include "MainMenu.h"
#include <SFML/Graphics.hpp>

Program::Program()
{
	window.create(VideoMode(1024, 768), "Course");
}

void Program::Run() {
	while (window.isOpen()) {
		mainMenu.Run(window);
	}
}