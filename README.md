# El Calabozo del Arcángel

> Proyecto de estructuras de datos en C++ — Juego de exploración de calabozos por turnos

---

## Descripción

El Calabozo del Arcángel es un juego en C++ de exploración por turnos ambientado en las tierras de Erresire. El jugador controla a un joven orco que debe atravesar 10 pisos de un calabozo generado aleatoriamente, enfrentando enemigos, recogiendo cofres y reclutando aliados, con el objetivo final de derrotar al legendario Arcángel en el piso 10.

El proyecto utiliza **listas enlazadas** como estructura de datos principal para representar el mapa del calabozo, y soporta **guardado y carga de partida** mediante archivos de texto.

---

## Estructuras de Datos

| Estructura | Descripción |
|---|---|
| `Celda` | Nodo de la lista enlazada. Representa una casilla del mapa con sus atributos (enemigo, cofre, taberna, punto de guardado) |
| `Jugador` | Estado del jugador: posición, salud, ataque y equipo de reclutas |
| `Recluta` | Aliado del jugador con nombre, salud y poder de ataque |
| `Arcangel` | Jefe final con atributos fijos (15 de salud, 10 de ataque) |

---

## Funcionalidades

### Generación del Calabozo
- El mapa es una cuadrícula de 10x10 celdas (columnas A-J, filas 1-10)
- Cada celda se genera aleatoriamente y puede contener: enemigo, punto de guardado, taberna o cofre
- Al completar un piso, se genera un nuevo calabozo para el siguiente nivel

### Sistema de Movimiento
- El jugador lanza dos dados al inicio de cada turno (suma = pasos disponibles)
- Se mueve en 4 direcciones: W (arriba), A (izquierda), S (abajo), D (derecha)
- Limite de 15 tiradas de dados por piso antes de perder

### Elementos del Mapa

| Simbolo | Elemento | Efecto |
|---|---|---|
| `[x]` | Jugador | Posicion actual |
| `[.]` | Visitada | Celda ya explorada |
| `[E]` | Enemigo | Inicia combate por turnos |
| `[S]` | Punto de guardado | Guarda la partida |
| `[T]` | Taberna | Recluta un aliado aleatorio |
| `[C]` | Cofre | Otorga objeto aleatorio |
| `[ ]` | Vacia | Sin contenido |

### Sistema de Combate
- Combate por turnos entre el jugador (y sus reclutas) contra el enemigo
- Los reclutas atacan primero, luego el jugador, luego el enemigo
- El enemigo contraataca al jugador y sus reclutas
- Si el jugador muere, termina el juego

### Cofres
- **Tipo 1 — Arma:** +5 ataque al jugador, +2 ataque a cada recluta
- **Tipo 2 — Aumento de vida:** +1 salud al jugador y a cada recluta
- **Tipo 3 — Pocion:** Recupera el 10% de la salud actual del jugador (mínimo 1)

### Reclutas
- Se reclutan en tabernas de forma aleatoria
- El equipo tiene un máximo de 3 reclutas
- Participan en el combate automáticamente

### Guardado y Carga
- El estado del mapa se guarda en `celdas.txt`
- El estado del jugador (salud, ataque, posición, equipo) se guarda en `jugador.txt`
- Se puede reanudar desde el menú principal

---

## Compilación y Ejecución

### Requisitos
- **Visual Studio 2017 o superior** (recomendado)
- Compilador compatible con **C++11**

### Con Visual Studio
1. Abrir `El calabozo del arcángel.sln`
2. Seleccionar configuración **Debug** o **Release**
3. Presionar **Ctrl + F5** para compilar y ejecutar

### Con g++ desde terminal
```bash
g++ -std=c++11 -o calabozo "El calabozo del arcángel.cpp"
./calabozo
```

---

## Ejemplo de Partida

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

## Estructura del Proyecto

```
El-calabozo-del-arcángel/
├── El calabozo del arcángel.cpp      # Codigo fuente principal
├── El calabozo del arcángel.vcxproj  # Proyecto de Visual Studio
├── El calabozo del arcángel.sln      # Solucion de Visual Studio
├── celdas.txt                        # Archivo de guardado del mapa
├── jugador.txt                       # Archivo de guardado del jugador
└── README.md                         # Este archivo
```

---


Desarrollado como proyecto universitario para el curso de **Estructuras de Datos** — ITCR.
