# 🧱 Brick Breaker Game (Raylib – C++)

A classic **Brick Breaker** game built using **C++** and **Raylib**, featuring smooth paddle physics, angle-based ball reflection, multiple brick health levels, sound effects, and a complete game state system.

Built as a learning + showcase project with clean architecture and real-time collision handling.

---

## 🎮 Gameplay Preview

### ▶️ Gameplay
![Gameplay](Assets/Screenshots/gameplay.gif)

---

### 🏁 Win Screen
![Win Screen](Assets/Screenshots/win.png)

---

### 💀 Lose / Game Over Screen
![Game Over Screen](Assets/Screenshots/lose.jpeg)

---

### 🏠 Main Menu
![Menu Screen](Assets/Screenshots/menu.jpeg)

---

### ⏸️ Pause Screen
![Pause Screen](Assets/Screenshots/pause.jpeg)

---

## 🎮 Gameplay Features

- 🎯 **Angle-based ball reflection** depending on paddle hit position  
- 🧱 **Multi-health bricks** with color indication  
  - Red → 3 hits  
  - Orange → 2 hits  
  - Green → 1 hit  
- 🧠 **Pre-launch aiming system** with adjustable angle (using Z and X keys)
- 🔊 Sound effects for:
  - Brick hit
  - Game over
  - Winning the level  
- ❤️ Life system (5 lives)
- 🧾 Score tracking (per hit +10 score and for destroying +50 score)
- 🕹️ Full game state flow:
  - Menu
  - Playing
  - Pause
  - Game Over / Win

---

## 🕹️ Controls

| Action | Key |
|------|----|
Move Paddle Left | `A` or `←`
Move Paddle Right | `D` or `→`
Launch Ball | `SPACE`
Aim Left | `Z`
Aim Right | `X`
Pause / Resume | `P`
Start / Restart | `ENTER`

---

## 🧩 Game States

- **MENU** – Start screen  
- **PLAYING** – Active gameplay  
- **PAUSED** – Game paused  
- **GAMEOVER** – Win or loss screen  

State transitions are handled cleanly using a finite state machine.

---

## 🛠️ Technical Highlights

- Written in **modern C++**
- Uses **Raylib** for rendering, input, and audio
- Physics-based collision resolution:
  - Paddle collision uses **angle calculation**
  - Brick collision resolves using **overlap comparison**
- Uses `std::vector` for dynamic brick management
- Clean separation of:
  - Update logic
  - Render logic
  - Game state logic

---

## 🧪 Collision Logic Overview

- **Paddle collision**
  - Ball reflection angle depends on hit position
  - Prevents jitter by correcting ball position after collision
- **Brick collision**
  - Determines collision axis using minimum overlap
  - Applies damage based on brick health
  - Only one brick collision processed per frame

---

## 📦 Releases (Download & Play)

This game is also **released via GitHub Releases**, allowing players to **download and play the game directly without building from source**.

- Pre-built executable is available in the **Releases** section
- Simply download, extract, and run the game
- No additional setup required for players

👉 Check the **Releases tab** on this repository to get the latest playable version.

---

## 📁 Project Structure 

```bash
.
├── main.cpp
├── Assets/
│ ├── Sounds/
│ │ ├── Hitbrick.wav
│ │ ├── gameover.wav
│ │ └── win.wav
│ └── Screenshots/
│ ├── gameplay.gif
│ ├── win.png
│ ├── lose.png
│ ├── menu.png
│ └── paused.png
└── README.md
```

---

## 🚀 How to Build & Run (From Source)

### Prerequisites
- C++ compiler (GCC / MinGW / Clang)
- **Raylib** installed and configured

### Compile (example – MinGW on Windows)
```bash
g++ main.cpp -o BrickBreaker -lraylib -lopengl32 -lgdi32 -lwinmm
```
### Run
```bash
./BrickBreaker
```
---
### 🧠 What This Project Demonstrates

- Game loop design
- State machines in games
- Real-time collision handling
- Input-driven gameplay
- Audio lifecycle management
- Clean C++ class design
---
### 👨‍💻 Authors

- marvelboyop
- Dwip

---
📜 License

This project is open for learning and experimentation & under Mit License.

Feel free to fork, modify, and build upon it.