# Multiplayer Ludo Game - README

## Video Demonstration

[Watch the  explained demonstration](https://youtu.be/RyOx62pDgrI)

[Watch the demonstration](https://youtu.be/K7mSVq3CRgo?si=lPu3qpgogi3bVlSB)

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

### 2. Installing SFML

There are three approaches to install SFML on Linux:

1. **Install it from your distribution's package repository (Preferred)**
   - If the desired version of SFML is available in the official repository, you can install it using your package manager. For example, on Debian-based distributions, run:
     ```bash
     sudo apt-get install libsfml-dev
     ```

### 3. Build the Server and Client

#### Compile the Server

Run the following command in your terminal:

```bash
g++ player.cpp server.cpp -o server -lsfml-system -lsfml-network -lsfml-graphics -lsfml-window -lsfml-audio -lpthread
```

#### Compile the Client

Run the following command:

```bash
g++ player.cpp client.cpp CircleShape.cpp SquareShape.cpp -o client -lsfml-system -lsfml-network -lsfml-graphics -lsfml-window -lsfml-audio -lpthread
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

