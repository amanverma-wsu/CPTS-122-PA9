# Multiplayer Ludo Game - README

## Video Demonstration

[Watch the demonstration](https://youtu.be/K7mSVq3CRgo?si=3SRF1Gdkjrm5a8jn)

[Learn how to play Ludo](https://youtu.be/xYXj9DwKHr4?si=MBgQe5E853CsJg8M)

---

## Project Overview

This project is a multiplayer **Ludo game** built using **C++** and **SFML (Simple and Fast Multimedia Library)**. It brings the classic board game experience into the digital realm, allowing multiple players to join and interact in real-time. The server-client architecture ensures smooth communication and gameplay.

---

## Team Members

- **Aman Verma**
- **Tony Cao**
- **Lyon Manono**

---

## Setup Instructions

### 1. Prerequisites

Ensure you have the following installed:

- **g++** (GNU Compiler Collection)
- **SFML Library** (including system, network, graphics, window, and audio modules)
- **pthread** library

### 2. Build the Server and Client

#### Compile the Server

Run the following command in your terminal:

```bash
make server: g++ player.cpp server.cpp -o server -lsfml-system -lsfml-network -lsfml-graphics -lsfml-window -lsfml-audio -lpthread
```

#### Compile the Client

Run the following command:

```bash
make client: g++ player.cpp client.cpp -o client -lsfml-system -lsfml-network -lsfml-graphics -lsfml-window -lsfml-audio -lpthread
```

---

## Running the Game

1. Open **five terminal windows**.
2. Execute the following commands:
   - **Terminal 1:** Start the server by running:
     ```bash
     ./server
     ```
   - **Terminal 2-5:** Start the clients by running:
     ```bash
     ./client
     ```
3. The game will automatically start once **four clients** successfully connect to the server.

---

## Game Features

- **Multiplayer Functionality:** Supports up to 4 players for an interactive gaming experience.
- **Classic Gameplay:** Brings the traditional Ludo rules into a digital format.
- **Real-time Interaction:** Ensures a responsive and engaging experience for all players.

---

## Notes

- Ensure all terminals are in the same directory containing the compiled files.
- For a smooth gaming experience, ensure a stable network connection.

---

## Acknowledgments

This project was created as a collaborative effort by the team members listed above. Special thanks to the contributors of the SFML library for providing tools essential to game development.

