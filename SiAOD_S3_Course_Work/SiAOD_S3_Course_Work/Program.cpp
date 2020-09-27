#include "Program.h"
#include "MainMenu.h"
#include <SFML/Graphics.hpp>

void Program::Run() {
	while (window.isOpen()) {
		mainMenu.Run(window);
	}
}