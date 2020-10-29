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