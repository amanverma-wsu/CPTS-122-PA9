 #include"player.h"
 Player::Player()
    {
        goti_home = 4;
        player_turn = false;
    }

    Player::Player(int no, sf::Color col, int **Goti_cods, float rad)
    {
        player_no = no;
        radius = rad;
        color = col;
        goti_home = 4;
        for (int i = 0; i < 4; i++)
        {
            gotiPass[i] = false;
            for (int j = 0; j < 2; j++)
            {
                playerGoti[i][j] = Goti_cods[i][j];
            }
        }
        score = 0;
        player_turn = false;
    }
    // Initialization function
    void Player::init(int no, sf::Color col, int **Goti_cods, float rad, int *player_home)
    {
        player_no = no;
        player_turn = false;
        radius = rad;
        color = col;
        home_centre[0] = player_home[0];
        home_centre[1] = player_home[1];
        for (int i = 0; i < 4; i++)
        {
            gotiPass[i] = false;
            gotis_home[i] = true;
            for (int j = 0; j < 2; j++)
            {
                playerGoti[i][j] = Goti_cods[i][j];
            }
        }
        score = 0;
        goti_home = 4;
    }
    // Increment particular goti by value x and y
    void Player::increment_Goti(int Goti_no, int x, int y)
    {
        playerGoti[Goti_no][0] += x;
        playerGoti[Goti_no][1] += y;
    }

    // Increment Goti X
    void Player::increment_X(int Goti_no)
    {
        playerGoti[Goti_no][0] += 1;
    }

    // Increment Goti Y
    void Player::increment_Y(int Goti_no)
    {
        playerGoti[Goti_no][1] += 1;
    }

    // Set Particular goti position
    void Player::setPosition(int Goti_no, int Goti_x, int Goti_y)
    {
        playerGoti[Goti_no][0] = Goti_x;
        playerGoti[Goti_no][1] = Goti_y;
    }

    // Check whether there is a goti in user x and user y or not
    bool Player::checkGoti(int user_x, int user_y, int &goti)
    {
        for (int i = 0; i < 4; i++)
        {
            if (gotiPass[i] == false)
            {
                if ((user_x >= playerGoti[i][0] && (user_x <= playerGoti[i][0] + (radius * 2))) && (user_y >= playerGoti[i][1] && (user_y <= playerGoti[i][1] + (radius * 2))))
                {
                    goti = i;
                    return true;
                }
            }
        }
        return false;
    }

    // Function to set turn of player
    void Player::setTurn(bool turn)
    {
        player_turn = turn;
    }

    // Functino to draw player Gotis.
    void Player::draw(sf::RenderWindow &Window)
    {
        if (player_turn == true)
        {
            sf::CircleShape goti(radius);
            goti.setFillColor(color);
            goti.setPosition(home_centre[0], home_centre[1]);
            Window.draw(goti);
        }
        if (gotiPass[0] == false)
        {

            sf::CircleShape goti(radius);
            goti.setFillColor(color);
            goti.setPosition(playerGoti[0][0], playerGoti[0][1]);
            Window.draw(goti);
        }

        if (gotiPass[1] == false)
        {
            sf::CircleShape goti(radius);
            goti.setFillColor(color);
            goti.setPosition(playerGoti[1][0], playerGoti[1][1]);
            Window.draw(goti);
        }

        if (gotiPass[2] == false)
        {
            sf::CircleShape goti(radius);
            goti.setFillColor(color);
            goti.setPosition(playerGoti[2][0], playerGoti[2][1]);
            Window.draw(goti);
        }

        if (gotiPass[3] == false)
        {
            sf::CircleShape goti(radius);
            goti.setFillColor(color);
            goti.setPosition(playerGoti[3][0], playerGoti[3][1]);
            Window.draw(goti);
        }
    }

    // Increments if a goti has come back home
    void Player::incGoti_home()
    {
        goti_home += 1;
    }

    // Decrements if a goti is out on board
    void Player::decGoti_home()
    {
        goti_home -= 1;
    }

    // Increment score of player
    void Player::incScore()
    {
        score += 1;
    }

    // Check if all gotis have passed or not.
    bool Player::passed()
    {
        for (int i = 0; i < 4; i++)
        {
            if (gotiPass[i] == false)
            {
                return false;
            }
        }
        return true;
    }
