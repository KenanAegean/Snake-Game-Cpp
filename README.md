
# 🐍 Snake Game with AI Modes
A C++ console-based Snake game featuring multiple play modes, AI-controlled snakes, and dynamic level progression.

---

## 🎮 Game Modes
The game supports **five different play modes**:
1. **One Player:** Control a single snake and reach the target score.
2. **Two Player Versus:** Two players compete to reach the target score first.
3. **Two Player Cooperative:** Work together to reach a combined target score.
4. **Player vs AI:** Play against an AI-controlled snake.
5. **Player and AI:** A cooperative mode where you and an AI snake work together.

---

## 🧠 AI Behavior
- The AI uses **A* pathfinding** to navigate towards apples while avoiding walls and its own body.
- AI waits for the second apple to appear before making its first move to avoid initial placement conflicts.
- The AI considers **wall penalties** and avoids dangerous areas.

---

## 🕹️ Controls
### Player 1:
- **Arrow Keys:** Control movement.
- `↑` - Move Up  
- `↓` - Move Down  
- `←` - Move Left  
- `→` - Move Right  

### Player 2 (if applicable):
- **WASD Keys:**
- `W` - Move Up  
- `S` - Move Down  
- `A` - Move Left  
- `D` - Move Right  

### Menu Navigation:
- **Up/Down Arrows:** Navigate through menu options.  
- **Enter:** Select the highlighted option.

---

## 🧱 Walls and Obstacles
- Walls are loaded dynamically from the `level1.txt`, `level2.txt` and `level3.txt` files.
- Colliding with walls triggers a **Game Over**.
- Levels are cleared when the player progresses, and walls are reloaded from the next level.

---

## 🍎 Apple Spawning and Growth
- An apple spawns randomly in an unoccupied position after being eaten.
- The snake grows by one segment when eating an apple.
- The first apple is auto-deactivated and replaced to prevent unwanted initial behavior.

---

## 📜 How to Play
1. **Launch the Game:**
   ```
   SnakeGame.exe
   ```
2. **Select Game Mode:**
   - Use the arrow keys to select a mode and press `Enter`.
3. **Play and Reach the Target Score:**
   - Collect apples to grow the snake and earn points.
4. **Game Over Conditions:**
   - Hitting a wall.
   - Self-collision.
   - Opponent achieving the target score (in versus mode).

---

## 📝 Project Structure
```
/SnakeGame
├── /assets
│   └── level1.txt        # Level data with wall positions
├── Apple.cpp             # Apple logic
├── Apple.h
├── GameMode.h            # Game mode definitions
├── GameObject.h
├── GameOverState.cpp     # Game over logic and UI
├── GameOverState.h
├── GamePlayState.cpp     # Main game logic and state
├── GamePlayState.h
├── GameState.h
├── MainMenuState.cpp     # Main menu logic
├── MainMenuState.h
├── Snake.cpp             # Snake class and movement logic
├── Snake.h
├── SnakeAI.cpp           # AI snake behavior and pathfinding
├── SnakeAI.h
├── SnakeGraphics.cpp     # Rendering and graphics
├── SnakeGraphics.h
├── SnakeInput.cpp        # Input handling
├── SnakeInput.h
├── SnakeGame.cpp         # Main game loop and state management
├── StateMachine.h        # State transition management
├── Wall.cpp              # Wall collision logic
├── Wall.h
├── World.cpp             # Manages all game objects
└── World.h
```

---

## 🎁 Features
✅ A* Pathfinding for AI Snakes  
✅ Multiple Game Modes with Unique Goals  
✅ Dynamic Level Progression  
✅ Real-time Collision and Object Management  
✅ Replay and Main Menu Transition  

---


## 👨‍💻 Author
**Kenan Ege**  
Passionate Game Developer 

---

## 📚 License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
