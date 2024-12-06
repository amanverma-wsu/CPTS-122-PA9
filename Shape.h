// Shape.h

#ifndef SHAPE_H
#define SHAPE_H

#include <SFML/Graphics.hpp>

class Shape {
public:
    virtual ~Shape() {}
    virtual void draw(sf::RenderWindow& window) = 0; // Pure virtual function
};

#endif // SHAPE_H

