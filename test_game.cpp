// simple_tests.cpp

#include <iostream>
#include <cassert>
#include "player.h"

// Test Case 1: Testing Player Initialization
void testPlayerInitialization() {
    std::cout << "Running testPlayerInitialization..." << std::endl;

    Player player;

    // Check initial values
    assert(player.score == 0);
    assert(player.goti_home == 4);
    assert(player.player_turn == false);

    std::cout << "testPlayerInitialization passed." << std::endl;
}

// Test Case 2: Testing Goti Position Setting
void testSetPosition() {
    std::cout << "Running testSetPosition..." << std::endl;

    Player player;

    // Set a position for goti 0
    player.setPosition(0, 100, 200);

    // Check if the position is set correctly
    assert(player.playerGoti[0][0] == 100);
    assert(player.playerGoti[0][1] == 200);

    std::cout << "testSetPosition passed." << std::endl;
}

// Test Case 3: Testing Goti Home Increment/Decrement
void testGotiHomeCount() {
    std::cout << "Running testGotiHomeCount..." << std::endl;

    Player player;

    // Decrement goti_home and check
    player.decGoti_home();
    assert(player.goti_home == 3);

    // Increment goti_home and check
    player.incGoti_home();
    assert(player.goti_home == 4);

    std::cout << "testGotiHomeCount passed." << std::endl;
}

// **Test Case 4: Testing Score Increment**
void testScoreIncrement() {
    std::cout << "Running testScoreIncrement..." << std::endl;

    Player player;

    // Initial score should be 0
    assert(player.score == 0);

    // Increment the score
    player.incScore();

    // Check if the score is incremented
    assert(player.score == 1);

    // Increment the score again
    player.incScore();

    // Check if the score is incremented to 2
    assert(player.score == 2);

    std::cout << "testScoreIncrement passed." << std::endl;
}

// Test Case 5: Testing Passed Function
void testPassed() {
    std::cout << "Running testPassed..." << std::endl;

    Player player;

    // Set all gotiPass to true
    for (int i = 0; i < 4; ++i) {
        player.gotiPass[i] = true;
    }

    // Check if passed() returns true
    assert(player.passed() == true);

    std::cout << "testPassed passed." << std::endl;
}

int main() {
    testPlayerInitialization();
    testSetPosition();
    testGotiHomeCount();
    testScoreIncrement();
    testPassed();

    std::cout << "All simple tests passed successfully!" << std::endl;
    return 0;
}

