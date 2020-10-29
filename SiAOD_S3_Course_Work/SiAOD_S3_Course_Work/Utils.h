#pragma once
#include <SFML/Graphics.hpp>
#include "Employee.h"

using namespace sf;

int BinarySearch(Employee** employersI, short int key, size_t size);

void cp866_to_utf8(sf::String& out, const char* in, size_t size);

void zoomViewAt(Vector2i pixel, RenderWindow& window, float zoom);
