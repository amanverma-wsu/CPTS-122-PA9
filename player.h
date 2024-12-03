/* player.h contains the Player Class and its functions*/
#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <string>
#include "SFML/Graphics.hpp"

class Player
{

public:
    int score;        // Player score
    bool player_turn; // Check whether player turn or not
    int player_no;    // Check which player
    float radius;     // Radius of the goti
    sf::Color color;  // Color of the goti

    int goti_home;        // Number of gotis which are home
    bool gotis_home[4];   // check whether a goti is home or not
    int playerGoti[4][2]; // Coordinates of player goti
    bool gotiPass[4];     // Check whether goti is passed or not
    int home_centre[2];   // Centre of Particular home

    Player();
    Player(int no, sf::Color col, int **Goti_cods, float rad);
    // Initialization function
    void init(int no, sf::Color col, int **Goti_cods, float rad, int *player_home);
    // Increment particular goti by value x and y
    void increment_Goti(int Goti_no, int x, int y);

    // Increment Goti X
    void increment_X(int Goti_no);

    // Increment Goti Y
    void increment_Y(int Goti_no);

    // Set Particular goti position
    void setPosition(int Goti_no, int Goti_x, int Goti_y);

    // Check whether there is a goti in user x and user y or not
    bool checkGoti(int user_x, int user_y, int &goti);

    // Function to set turn of player
    void setTurn(bool turn);
    // Functino to draw player Gotis.
    void draw(sf::RenderWindow &Window);
    // Increments if a goti has come back home
    void incGoti_home();

    // Decrements if a goti is out on board
    void decGoti_home();

    // Increment score of player
    void incScore();
    // Check if all gotis have passed or not.
    bool passed();
};
#endif
