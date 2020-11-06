#pragma once
#include <SFML/Graphics.hpp>
#include "Employee.h"

class TreeVertex
{
public:
	TreeVertex* right;
	TreeVertex* left;
	int balance;
	Employee data;
	sf::CircleShape circle;
	sf::VertexArray Left;
	sf::VertexArray Right;

	TreeVertex();
};