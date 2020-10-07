#pragma once
#include <SFML/Graphics.hpp>
#include "MainMenu.h"

using namespace sf;

class Program
{
private:
	RenderWindow window;
	MainMenu mainMenu;
public:
	Program();

	void Run();
};

