#pragma once
#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "VisualTree.h"

using namespace sf;

class Program
{
private:
	RenderWindow window;
public:
	Program();

	void Run();
};

