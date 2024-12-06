// SquareShape.h

#ifndef SQUARESHAPE_H
#define SQUARESHAPE_H

#include "Shape.h"

class SquareShape : public Shape {
private:
    sf::RectangleShape square;
public:
    SquareShape(float size, sf::Color color, float posX, float posY);
    virtual void draw(sf::RenderWindow& window) override;
};

#endif // SQUARESHAPE_H

