# El Calabozo del Arcángel

> Data structures project in C++ — Turn-based dungeon exploration game

---

## Description

El Calabozo del Arcángel is a turn-based dungeon exploration game written in C++, set in the lands of Erresire. The player controls a young orc who must traverse 10 floors of a randomly generated dungeon, fighting enemies, collecting chests, and recruiting allies, with the ultimate goal of defeating the legendary Archangel on the 10th floor.

The project uses **linked lists** as its primary data structure to represent the dungeon map, and supports **save and load** functionality through text files.

---

## Data Structures

| Structure | Description |
|---|---|
| `Celda` | Linked list node. Represents a map tile with its attributes (enemy, chest, tavern, save point) |
| `Jugador` | Player state: position, health, attack power, and recruit team |
| `Recluta` | Player ally with name, health, and attack power |
| `Arcangel` | Final boss with fixed attributes (15 health, 10 attack) |

---

## Features

### Dungeon Generation
- The map is a 10x10 grid of cells (columns A-J, rows 1-10)
- Each cell is randomly generated and may contain: an enemy, a save point, a tavern, or a chest
- Upon completing a floor, a new dungeon is generated for the next level

### Movement System
- The player rolls two dice at the start of each turn (sum = available steps)
- Movement in 4 directions: W (up), A (left), S (down), D (right)
- Limit of 15 dice rolls per floor before losing

### Map Elements

| Symbol | Element | Effect |
|---|---|---|
| `[x]` | Jugador | Current position |
| `[.]` | Visitada | Already explored tile |
| `[E]` | Enemigo | Triggers turn-based combat |
| `[S]` | Punto de guardado | Saves the game |
| `[T]` | Taberna | Recruits a random ally |
| `[C]` | Cofre | Grants a random item |
| `[ ]` | Vacía | No content |

### Combat System
- Turn-based combat between the player (and recruits) against the enemy
- Recruits attack first, then the player, then the enemy counterattacks
- The enemy attacks both the player and the recruits
- If the player dies, the game ends

### Chests
- **Tipo 1 — Arma:** +5 attack to the player, +2 attack to each recruit
- **Tipo 2 — Aumento de vida:** +1 health to the player and each recruit
- **Tipo 3 — Pocion:** Recovers 10% of the player's current health (minimum 1)

### Recruits
- Recruited randomly at taverns
- The team has a maximum of 3 recruits (`equipo.size() < 3`)
- Participate in combat automatically

### Save and Load
- The dungeon map state is saved to `celdas.txt`
- The player state (health, attack, position, team) is saved to `jugador.txt`
- A saved game can be resumed from the main menu

---

## Build and Run

### Requirements
- **Visual Studio 2017 or later** (recommended)
- Compiler with **C++11** support

### With Visual Studio
1. Open `El calabozo del arcángel.sln`
2. Select **Debug** or **Release** configuration
3. Press **Ctrl + F5** to build and run

### With g++ from terminal
```bash
g++ -std=c++11 -o calabozo "El calabozo del arcángel.cpp"
./calabozo
```

---

## Gameplay Example

```
Bienvenido al juego del Calabozo!
1. Iniciar nueva partida
2. Cargar partida guardada
3. Salir

Calabozo - Estado del Piso 1:
   A   B   C   D   E   F   G   H   I   J
1  [x] [ ] [E] [ ] [ ] [C] [ ] [ ] [ ] [ ]
2  [ ] [S] [ ] [ ] [E] [ ] [ ] [ ] [ ] [ ]
...

Presiona Enter para lanzar los dados...
Lanzaste los dados. Puedes avanzar 7 pasos.
Elige una direccion para moverte (W, A, S, D):
```

---

## Project Structure

```
El-calabozo-del-arcángel/
├── El calabozo del arcángel.cpp      # Main source code
├── El calabozo del arcángel.vcxproj  # Visual Studio project file
├── El calabozo del arcángel.sln      # Visual Studio solution file
├── celdas.txt                        # Dungeon map save file
├── jugador.txt                       # Player state save file
└── README.md                         # This file
```

---

## Author

Developed as a university project for the **Data Structures** course — ITCR.
