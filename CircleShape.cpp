// CircleShape.cpp

#include "CircleShape.h"

CircleShape::CircleShape(float radius, sf::Color color, float posX, float posY) {
    circle.setRadius(radius);
    circle.setFillColor(color);
    circle.setPosition(posX, posY);
}

void CircleShape::draw(sf::RenderWindow& window) {
    window.draw(circle);
}

