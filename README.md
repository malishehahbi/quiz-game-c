# Quiz Game Project

Welcome to the Quiz Game project! This project is a client-server application designed for interactive multiplayer quizzes. Developed in **C** using the Winsock2 library, it allows players to join a quiz session, answer questions, and compete for the highest score. This README file provides a detailed overview of the project, setup instructions, and key features.

---

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [How It Works](#how-it-works)
- [Setup Instructions](#setup-instructions)
- [Screenshots](#screenshots)
- [Usage](#usage)
- [Future Improvements](#future-improvements)
- [Contributors](#contributors)

---

## Introduction
The Quiz Game project is an educational tool and a fun game designed as part of a university project. It features a client-server architecture where multiple players can connect to the server, answer quiz questions, and receive their scores. This project demonstrates basic networking concepts and multithreading in C.

---

## Features
- **Multiplayer Support:** Up to 5 clients can connect simultaneously.
- **Interactive Quiz:** Players answer a series of 5 multiple-choice questions.
- **Real-time Feedback:** Instant feedback on answers (Correct/Wrong).
- **Final Scores:** Broadcast of scores to all players after the quiz.
- **Thread-Safe Communication:** Uses multithreading for handling multiple clients.

---

## How It Works
1. **Server:**
   - Hosts the quiz and handles multiple client connections using threads.
   - Sends questions to clients and evaluates their responses.
   - Tracks scores and broadcasts the final leaderboard.

2. **Client:**
   - Connects to the server.
   - Receives questions and sends answers.
   - Displays feedback and final scores.

---

## Setup Instructions

### Prerequisites
- Windows OS (required for Winsock2 library).
- GCC compiler (MinGW recommended).

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/malishehahbi/quiz-game-c.git
   cd quiz-game-c
   ```

2. Compile the server code:
   ```bash
   gcc -o server.exe server.c -lws2_32 -lpthread
   ```

3. Compile the client code:
   ```bash
   gcc -o client.exe client.c -lws2_32
   ```

4. Start the server:
   ```bash
   ./server.exe
   ```

5. Run the client (repeat for multiple clients):
   ```bash
   ./client.exe
   ```

---

## Screenshots
### Server
![Server Console](https://i.ibb.co/L1NwSDW/Screenshot-2024-12-18-145452.jpg)
*Screenshot of the server handling clients and broadcasting scores.*

### Client
![Client Console](https://i.ibb.co/bR0hjJg/Screenshot-2024-12-18-145033.jpg)
*Screenshot of the client answering a quiz question.*

---

## Usage
1. Launch the server first.
2. Start the clients and enter player names when prompted.
3. Answer the questions displayed on the client.
4. View the final scores after the quiz.

---

## Future Improvements
- Add support for customizable questions loaded from a file.
- Enhance user interface with a GUI.
- Add support for more players.
- Implement secure communication with encryption.
- Develop a cross-platform version.

---

## Contributors
- **[MOHAMAD ALI ALCHIHABI](https://github.com/malishehahbi)** -2210206509- Developer
- **[SALEH MUBARAK](https://github.com/)** -211020625- Developer

---

Thank you for checking out our project! Feel free to contribute by submitting pull requests or reporting issues.
