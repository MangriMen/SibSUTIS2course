#pragma once
#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "VisualTree.h"

using namespace sf;

class Program
{
private:
	RenderWindow window;
	MainMenu mainMenu;
	VisualTree visualTree;
public:
	Program();

	void Run();

	static void zoomViewAt(Vector2i pixel, RenderWindow& window, float zoom);
};

