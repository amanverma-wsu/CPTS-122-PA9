// client.cpp

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "player.h"
#include "Initial.h"

using namespace std;

// Structure to represent the game state (same as in server)
struct GameState {
    int currentPlayer; // Player number (0-3)
    int diceRoll; // Current dice value (latest rolled value)
    int playerScores[4]; // Scores of each player
    int playerPositions[4][4][2]; // Positions of each player's tokens [player][token][x/y]
    bool gotiHome[4][4]; // Whether each token is at home
    bool gotiPass[4][4]; // Whether each token has reached the end
    vector<int> diceStore[4]; // Dice values stored per player
    bool isTurnActive; // Whether the current player's turn is active

    GameState() {
        currentPlayer = 0;
        diceRoll = 0;
        isTurnActive = false;
        for (int p = 0; p < 4; p++) {
            playerScores[p] = 0;
            diceStore[p].clear();
            for (int g = 0; g < 4; g++) {
                playerPositions[p][g][0] = 0;
                playerPositions[p][g][1] = 0;
                gotiHome[p][g] = true;
                gotiPass[p][g] = false;
            }
        }
    }
};

// Overload the << and >> operators for sf::Packet
sf::Packet& operator <<(sf::Packet& packet, const GameState& gameState) {
    packet << gameState.currentPlayer << gameState.diceRoll << gameState.isTurnActive;
    for (int p = 0; p < 4; p++) {
        packet << gameState.playerScores[p];
        // Send the size of diceStore for each player
        packet << static_cast<sf::Uint32>(gameState.diceStore[p].size());
        for (auto diceValue : gameState.diceStore[p]) {
            packet << diceValue;
        }
        for (int g = 0; g < 4; g++) {
            packet << gameState.playerPositions[p][g][0] << gameState.playerPositions[p][g][1];
            packet << gameState.gotiHome[p][g];
            packet << gameState.gotiPass[p][g];
        }
    }
    return packet;
}

sf::Packet& operator >>(sf::Packet& packet, GameState& gameState) {
    packet >> gameState.currentPlayer >> gameState.diceRoll >> gameState.isTurnActive;
    for (int p = 0; p < 4; p++) {
        packet >> gameState.playerScores[p];
        // Receive the size of diceStore for each player
        sf::Uint32 diceStoreSize;
        packet >> diceStoreSize;
        gameState.diceStore[p].clear();
        for (sf::Uint32 i = 0; i < diceStoreSize; ++i) {
            int diceValue;
            packet >> diceValue;
            gameState.diceStore[p].push_back(diceValue);
        }
        for (int g = 0; g < 4; g++) {
            packet >> gameState.playerPositions[p][g][0] >> gameState.playerPositions[p][g][1];
            packet >> gameState.gotiHome[p][g];
            packet >> gameState.gotiPass[p][g];
        }
    }
    return packet;
}

// Global variables
int playerID; // Assigned by the server
GameState gameState;
sf::TcpSocket socket;
std::mutex gameStateMutex;
bool isMyTurn = false;
bool gameRunning = true;
sf::RenderWindow window(sf::VideoMode(1060, 750), "LUDO");

// Load resources
sf::Sprite backimg; // Background image
sf::Sprite menus;   // Menu page sprite
sf::Font font;

// Function to receive updates from the server
void receiveUpdates() {
    while (gameRunning) {
        sf::Packet packet;
        if (socket.receive(packet) == sf::Socket::Done) {
            std::string messageType;
            packet >> messageType;

            if (messageType == "GAME_STATE") {
                std::lock_guard<std::mutex> lock(gameStateMutex);
                packet >> gameState;

                // Update the local game state and player objects
                for (int p = 0; p < 4; p++) {
                    obj[p].score = gameState.playerScores[p];
                    for (int g = 0; g < 4; g++) {
                        obj[p].playerGoti[g][0] = gameState.playerPositions[p][g][0];
                        obj[p].playerGoti[g][1] = gameState.playerPositions[p][g][1];
                        obj[p].gotis_home[g] = gameState.gotiHome[p][g];
                        obj[p].gotiPass[g] = gameState.gotiPass[p][g];
                    }
                }
            }
            else if (messageType == "YOUR_TURN") {
                isMyTurn = true;
                std::cout << "It's your turn!" << std::endl;
            }
            else if (messageType == "DICE_ROLL") {
                packet >> gameState.diceRoll;
                std::cout << "You rolled a " << gameState.diceRoll << std::endl;
            }
            else if (messageType == "INVALID_MOVE") {
                std::cout << "Invalid move. Please try again." << std::endl;
            }
            // Handle other message types as needed
        }
    }
}

int main()
{
    bool menu = true; // Variable for menu

    // Initialize the game logic
    initialize();

    // Load font
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load arial.ttf" << std::endl;
        return -1;
    }

    // Load menu background
    sf::Texture menuBackground;
    if (!menuBackground.loadFromFile("Home.png")) {
        std::cerr << "Failed to load Home.png" << std::endl;
        return -1;
    }
    menus.setTexture(menuBackground);

    // Load game board background
    sf::Texture gameBackground;
    if (!gameBackground.loadFromFile("BOARD_FINAL.jpg")) {
        std::cerr << "Failed to load BOARD_FINAL.jpg" << std::endl;
        return -1;
    }
    backimg.setTexture(gameBackground);
    backimg.setScale(0.5f, 0.5f);

    // Connect to the server
    if (socket.connect("127.0.0.1", 53000) != sf::Socket::Done) {
        std::cerr << "Failed to connect to the server." << std::endl;
        return -1;
    }
    std::cout << "Connected to the server." << std::endl;

    // Receive the assigned player ID from the server
    sf::Packet packet;
    if (socket.receive(packet) == sf::Socket::Done) {
        std::string messageType;
        packet >> messageType;
        if (messageType == "ASSIGN_PLAYER_ID") {
            packet >> playerID;
            std::cout << "Assigned player ID: " << playerID << std::endl;
        }
    }

    // Start the thread to receive updates from the server
    std::thread receiverThread(receiveUpdates);

    // Display menu
    while (window.isOpen() && menu == true)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);
                if ((position.x >= 392 && position.x <= 607) && (position.y >= 401 && position.y <= 557)) {
                    window.clear();
                    menu = false;
                }
            }
        }

        window.clear();
        window.draw(menus);
        window.display();
    }

    // Main game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                gameRunning = false;
                window.close();
                socket.disconnect();
                if (receiverThread.joinable()) {
                    receiverThread.join();
                }
                return 0;
            }

            if (event.type == sf::Event::MouseButtonPressed && isMyTurn)
            {
                sf::Vector2i position = sf::Mouse::getPosition(window);

                // The following checks which dice value the user has selected
                std::lock_guard<std::mutex> lock(gameStateMutex);
                for (size_t i = 0; i < gameState.diceStore[playerID].size(); ++i) {
                    int diceXStart = 776 + i * 54;
                    int diceXEnd = diceXStart + 50;
                    int diceYStart = 554;
                    int diceYEnd = diceYStart + 50;

                    if ((position.x >= diceXStart && position.x <= diceXEnd) && (position.y >= diceYStart && position.y <= diceYEnd)) {
                        int selectedDiceValue = gameState.diceStore[playerID][i];

                        // Send MOVE_TOKEN message after selecting a token
                        int goti_no = -1;
                        // Wait for the player to click on a token
                        bool tokenSelected = false;
                        while (!tokenSelected && window.isOpen())
                        {
                            sf::Event tokenEvent;
                            while (window.pollEvent(tokenEvent))
                            {
                                if (tokenEvent.type == sf::Event::MouseButtonPressed)
                                {
                                    sf::Vector2i tokenPosition = sf::Mouse::getPosition(window);
                                    if (obj[playerID].checkGoti(tokenPosition.x, tokenPosition.y, goti_no)) {
                                        // Send MOVE_TOKEN message
                                        sf::Packet movePacket;
                                        movePacket << std::string("MOVE_TOKEN") << goti_no << selectedDiceValue;
                                        socket.send(movePacket);
                                        // Remove the selected dice value from diceStore
                                        gameState.diceStore[playerID].erase(gameState.diceStore[playerID].begin() + i);
                                        //isMyTurn = false; // Commented out to allow multiple moves in the same turn if there are more dice values
                                        tokenSelected = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }

                // Handle dice roll click
                if ((position.x >= 590 && position.x <= 700) && (position.y >= 50 && position.y <= 155))
                {
                    // Send ROLL_DICE request to the server
                    sf::Packet rollPacket;
                    rollPacket << std::string("ROLL_DICE");
                    socket.send(rollPacket);
                }
            }
        }

        window.clear();

        // Draw background image
        window.draw(backimg);

        // Draw players
        obj[0].draw(window);
        obj[1].draw(window);
        obj[2].draw(window);
        obj[3].draw(window);

        // Draw rectangles for the dice values
        for (size_t i = 0; i < gameState.diceStore[playerID].size(); ++i) {
            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(50, 50));
            rectangle.setOutlineColor(sf::Color::Red);
            rectangle.setOutlineThickness(1);
            rectangle.setPosition(776 + i * 54, 553);
            window.draw(rectangle);

            // Display dice value
            sf::Text text;
            text.setFont(font);
            text.setString(to_string(gameState.diceStore[playerID][i]));
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::Red);
            text.setPosition(778.f + i * 54, 573.f);
            window.draw(text);
        }

        // Display score of each player
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);

        for (int p = 0; p < 4; p++) {
            text.setString("Player " + to_string(p + 1) + " Score : " + to_string(obj[p].score));
            text.setPosition(778.f, 97.f + 41.f * p);
            window.draw(text);
        }

        // Display current player's turn
        if (gameState.currentPlayer == playerID) {
            text.setString("Your turn");
        }
        else {
            text.setString("Player " + to_string(gameState.currentPlayer + 1) + " turn");
        }
        text.setPosition(778.f, 261.f);
        window.draw(text);

        // Display selected dice value (if any)
        text.setString("Dice Selected: ");
        text.setPosition(778.f, 302.f);
        window.draw(text);

        // Draw the dice based on gameState.diceRoll
        // Implement drawing of dice faces if needed

        window.display();
    }

    // Cleanup
    gameRunning = false;
    socket.disconnect();
    if (receiverThread.joinable()) {
        receiverThread.join();
    }

    return 0;
}

