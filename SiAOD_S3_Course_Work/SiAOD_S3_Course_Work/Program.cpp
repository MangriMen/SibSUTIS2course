#include <iostream>
#include "Program.h"
#include "MainMenu.h"
#include <SFML/Graphics.hpp>

Program::Program()
{
}

void Program::Run() {
	mainMenu.RunThread();
	//visualTree.RunThread();
}

void Program::zoomViewAt(Vector2i pixel, RenderWindow& window, float zoom)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}
