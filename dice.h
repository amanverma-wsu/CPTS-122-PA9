#include<iostream>
#include "SFML/Graphics.hpp"

class Dice{
public:

    
int dice_roll;
Dice();



void draw(sf::RenderWindow &window,int &val);

void reset();

};
