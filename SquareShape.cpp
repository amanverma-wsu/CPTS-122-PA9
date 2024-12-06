// SquareShape.cpp

#include "SquareShape.h"

SquareShape::SquareShape(float size, sf::Color color, float posX, float posY) {
    square.setSize(sf::Vector2f(size, size));
    square.setFillColor(color);
    square.setPosition(posX, posY);
}

void SquareShape::draw(sf::RenderWindow& window) {
    window.draw(square);
}

