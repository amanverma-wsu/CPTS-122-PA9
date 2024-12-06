// CircleShape.h

#ifndef CIRCLESHAPE_H
#define CIRCLESHAPE_H

#include "Shape.h"

class CircleShape : public Shape {
private:
    sf::CircleShape circle;
public:
    CircleShape(float radius, sf::Color color, float posX, float posY);
    virtual void draw(sf::RenderWindow& window) override;
};

#endif // CIRCLESHAPE_H

