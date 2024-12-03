// server.cpp

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <SFML/Network.hpp>
#include "player.h"
#include "Initial.h"

using namespace std;

// Structure to represent the game state
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
                playerPositions[p][g][0] = obj[p].playerGoti[g][0];
                playerPositions[p][g][1] = obj[p].playerGoti[g][1];
                gotiHome[p][g] = obj[p].gotis_home[g];
                gotiPass[p][g] = obj[p].gotiPass[g];
            }
        }
    }
};

// Overload the << operator for sf::Packet to send GameState
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

// Overload the >> operator for sf::Packet to receive GameState
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

// Function to broadcast the game state to all clients
void broadcastGameState(const std::vector<sf::TcpSocket*>& clients, const GameState& gameState) {
    sf::Packet packet;
    std::string messageType = "GAME_STATE";
    packet << messageType << gameState;
    for (auto client : clients) {
        if (client->send(packet) != sf::Socket::Done) {
            std::cerr << "Error sending game state to a client." << std::endl;
        }
    }
}

// Main game loop function
void runGameLoop(std::vector<sf::TcpSocket*>& clients, sf::SocketSelector& selector, GameState& gameState) {
    bool gameRunning = true;
    int currentPlayer = 0; // Start with player 0
    gameState.currentPlayer = currentPlayer;
    gameState.isTurnActive = false;

    // Send initial game state to all clients
    broadcastGameState(clients, gameState);

    while (gameRunning) {
        // Notify current player that it's their turn
        sf::Packet turnPacket;
        std::string messageType = "YOUR_TURN";
        turnPacket << messageType;
        clients[currentPlayer]->send(turnPacket);

        gameState.isTurnActive = true;
        bool turnEnded = false;
        int countSixes = 0;
        int playerRolls = 0;

        // Clear player's diceStore at the beginning of their turn
        gameState.diceStore[currentPlayer].clear();

        while (!turnEnded) {
            if (selector.wait()) {
                // Check if the current player has sent a message
                if (selector.isReady(*clients[currentPlayer])) {
                    sf::Packet packet;
                    if (clients[currentPlayer]->receive(packet) == sf::Socket::Done) {
                        // Process the packet
                        packet >> messageType;

                        if (messageType == "ROLL_DICE") {
                            playerRolls++;

                            // Server rolls the dice and sends the value to the client
                            gameState.diceRoll = rand() % 6 + 1;
                            sf::Packet dicePacket;
                            dicePacket << "DICE_ROLL" << gameState.diceRoll;
                            clients[currentPlayer]->send(dicePacket);

                            // If diceRoll is 6, add to diceStore and allow another roll
                            if (gameState.diceRoll == 6) {
                                gameState.diceStore[currentPlayer].push_back(gameState.diceRoll);
                                countSixes++;
                                if (countSixes == 3) {
                                    // Remove the last three sixes from diceStore
                                    for (int i = 0; i < 3; ++i) {
                                        gameState.diceStore[currentPlayer].pop_back();
                                    }
                                    // End turn
                                    turnEnded = true;
                                    gameState.diceRoll = 0;
                                    gameState.isTurnActive = false;
                                    // Move to the next player
                                    currentPlayer = (currentPlayer + 1) % clients.size();
                                    gameState.currentPlayer = currentPlayer;
                                }
                            } else {
                                // If not a six, add to diceStore and end dice rolling
                                gameState.diceStore[currentPlayer].push_back(gameState.diceRoll);
                                countSixes = 0;
                                // Player cannot roll again
                            }

                            // Broadcast the updated game state
                            broadcastGameState(clients, gameState);
                        }
                        else if (messageType == "MOVE_TOKEN") {
                            // Client wants to move a token
                            int tokenIndex;
                            int selectedDiceValue;
                            packet >> tokenIndex >> selectedDiceValue;

                            // Validate and perform the move
                            bool validMove = true; // Assume move is valid for now

                            // Check if the selected dice value is in diceStore
                            auto& diceStore = gameState.diceStore[currentPlayer];
                            auto it = std::find(diceStore.begin(), diceStore.end(), selectedDiceValue);
                            if (it == diceStore.end()) {
                                // Invalid dice value selected
                                validMove = false;
                            }

                            if (validMove) {
                                // Check if token is at home and dice value is 6
                                if (obj[currentPlayer].gotis_home[tokenIndex]) {
                                    if (selectedDiceValue == 6) {
                                        // Move token out of home
                                        int* points = returnHome(currentPlayer);
                                        obj[currentPlayer].decGoti_home();
                                        obj[currentPlayer].gotis_home[tokenIndex] = false;
                                        obj[currentPlayer].setPosition(tokenIndex, points[0], points[1]);
                                    } else {
                                        // Cannot move token out of home without rolling a 6
                                        validMove = false;
                                    }
                                } else {
                                    // Token is on the board
                                    // Move the token based on selectedDiceValue
                                    int& curx = obj[currentPlayer].playerGoti[tokenIndex][0];
                                    int& cury = obj[currentPlayer].playerGoti[tokenIndex][1];

                                    if (canGoHome(selectedDiceValue, currentPlayer, tokenIndex) == false) {
                                        for (int i = 0; i < selectedDiceValue; ++i) {
                                            move_goti(currentPlayer, curx, cury);
                                            if (getSavePoint(curx, cury) == false) {
                                                check_collision(currentPlayer, tokenIndex);
                                            }
                                        }
                                    } else {
                                        // Move towards home
                                        for (int i = 0; i < selectedDiceValue; ++i) {
                                            if (goingHome(false, currentPlayer, curx, cury) == true) {
                                                if (boxes_to_home(currentPlayer, tokenIndex) >= selectedDiceValue) {
                                                    goingHome(true, currentPlayer, curx, cury, selectedDiceValue);
                                                }
                                            } else {
                                                move_goti(currentPlayer, curx, cury);
                                                if (getSavePoint(curx, cury) == false) {
                                                    check_collision(currentPlayer, tokenIndex);
                                                }
                                            }

                                            if (checkHome(currentPlayer, tokenIndex) == true) {
                                                obj[currentPlayer].gotiPass[tokenIndex] = true;
                                                if (obj[currentPlayer].passed()) {
                                                    // Player has won, handle accordingly
                                                    std::cout << "Player " << currentPlayer << " has won!" << std::endl;
                                                    // You can remove the player from the game or mark as finished
                                                }
                                            }
                                        }
                                    }
                                }

                                // Remove the used dice value from diceStore
                                diceStore.erase(it);

                                // Broadcast the updated game state
                                // Update positions in gameState
                                for (int g = 0; g < 4; g++) {
                                    gameState.playerPositions[currentPlayer][g][0] = obj[currentPlayer].playerGoti[g][0];
                                    gameState.playerPositions[currentPlayer][g][1] = obj[currentPlayer].playerGoti[g][1];
                                    gameState.gotiHome[currentPlayer][g] = obj[currentPlayer].gotis_home[g];
                                    gameState.gotiPass[currentPlayer][g] = obj[currentPlayer].gotiPass[g];
                                }

                                broadcastGameState(clients, gameState);

                                // After moving a token, check if diceStore is empty
                                if (diceStore.empty()) {
                                    turnEnded = true;
                                    gameState.diceRoll = 0;
                                    gameState.isTurnActive = false;
                                    // Move to the next player
                                    currentPlayer = (currentPlayer + 1) % clients.size();
                                    gameState.currentPlayer = currentPlayer;
                                }
                            } else {
                                // Send an invalid move message to the client
                                sf::Packet invalidPacket;
                                invalidPacket << "INVALID_MOVE";
                                clients[currentPlayer]->send(invalidPacket);
                            }
                        }
                    }
                }
            }
        }

        // After turn ends, reset diceStore for the player
        gameState.diceStore[currentPlayer].clear();
        // Broadcast the updated game state
        broadcastGameState(clients, gameState);

        // Check for game end conditions
        // Implement your game over logic here
    }
}

int main() {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Initialize the server
    sf::TcpListener listener;
    unsigned short port = 53000;
    if (listener.listen(port) != sf::Socket::Done) {
        std::cerr << "Error listening on port " << port << std::endl;
        return -1;
    }

    std::cout << "Server is listening to port " << port << ", waiting for connections..." << std::endl;

    // Accept up to 4 client connections
    std::vector<sf::TcpSocket*> clients;
    sf::SocketSelector selector;
    int playerIDs[4]; // Map sockets to player IDs

    // Game state variables
    const int maxPlayers = 4;
    int numPlayersConnected = 0;
    bool gameStarted = false;

    // Initialize the game state
    initialize(); // Call the initialize function from Initial.h
    GameState gameState;

    // Accept clients
    while (numPlayersConnected < maxPlayers) {
        sf::TcpSocket* client = new sf::TcpSocket;
        if (listener.accept(*client) == sf::Socket::Done) {
            clients.push_back(client);
            selector.add(*client);
            int playerID = numPlayersConnected; // 0 to 3
            playerIDs[numPlayersConnected] = playerID;
            numPlayersConnected++;

            std::cout << "New client connected! Total clients: " << numPlayersConnected << std::endl;

            // Send the assigned player ID to the client
            sf::Packet packet;
            std::string messageType = "ASSIGN_PLAYER_ID";
            packet << messageType << playerID;
            client->send(packet);
        }
        else {
            delete client;
            std::cerr << "Failed to accept a client connection." << std::endl;
        }
    }

    std::cout << "All players connected. Starting the game..." << std::endl;
    gameStarted = true;

    // Run the main game loop
    runGameLoop(clients, selector, gameState);

    // Clean up
    for (auto client : clients) {
        client->disconnect();
        delete client;
    }

    return 0;
}

