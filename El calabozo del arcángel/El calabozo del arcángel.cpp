#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm> // Para std::max
#include <fstream>
#include <string>  

int pisoCalabozo = 1;
int numEnemies = 0;
bool juego = true;
int numDiceThrows = 0; // Contador de tiradas de dados

struct Celda {
    int piso;              // Número de piso en el que se encuentra la celda
    char column;           // Columna de la celda (A-J)
    int row;               // Fila de la celda (1-10)
    bool hasPlayer;        // Indica si la celda tiene un jugador
    bool visited;          // Indica si la celda ha sido visitada por el jugador
    bool hasEnemy;         // Indica si la celda tiene un enemigo
    bool hasSavePoint;     // Indica si la celda tiene un punto de guardado
    bool hasTavern;        // Indica si la celda tiene una taberna
    bool hasChest;         // Indica si la celda tiene un cofre
    int enemyHealth;       // Salud del enemigo presente en la celda
    int enemyAttack;       // Poder de ataque del enemigo presente en la celda
    int chestContent;      // Contenido del cofre (0 si está vacío)
    Celda* siguiente;      // Puntero a la siguiente celda en la lista
};

struct Recluta {
    std::string nombre;    // Nombre de la recluta
    int health;            // Puntos de vida de la recluta
    int attackPower;       // Poder de ataque de la recluta
};

struct Arcangel {
    std::string nombre = "Arcangel";   // Nombre fijo del Arcángel
    int health = 15;                    // Puntos de vida del Arcángel
    int attackPower = 10;               // Poder de ataque del Arcángel
};

struct Jugador {
    Celda* posicion;                 // Puntero a la celda donde se encuentra el jugador
    int health;                      // Puntos de vida del jugador
    int attackPower;                 // Poder de ataque del jugador
    std::vector<Recluta> equipo;    // Vector que almacena las reclutas en el equipo

    Jugador() : posicion(nullptr), health(3), attackPower(3) {}

    /**
     * Agrega una recluta al equipo del jugador.
     * nuevoRecluta La recluta a agregar al equipo.
     * return true si se pudo reclutar (espacio disponible), false si el equipo está lleno.
     */
    bool reclutarPersonas(const Recluta& nuevoRecluta) {
        if (equipo.size() < 3) {
            equipo.push_back(nuevoRecluta);
            return true;
        }
        return false; // No se puede reclutar más de tres Reclutas
    }
};


/**
 * Crea una nueva instancia de la estructura Celda con valores iniciales especificados.
 * param col Carácter que representa la columna de la celda.
 * param r Entero que representa la fila de la celda.
 * return Puntero a la nueva instancia de Celda creada dinámicamente.
 */
Celda* crearCelda(char col, int r) {
    Celda* newCell = new Celda;
    newCell->piso = pisoCalabozo;
    newCell->column = col;
    newCell->row = r;
    newCell->visited = false;
    newCell->hasEnemy = false;
    newCell->hasSavePoint = false;
    newCell->hasTavern = false;
    newCell->hasChest = false;
    newCell->enemyHealth = 0;
    newCell->enemyAttack = 0;
    newCell->chestContent = 0;
    newCell->siguiente = nullptr;
    return newCell;
}

/**
 * Libera la memoria ocupada por una lista enlazada de celdas.
 * param cabeza Referencia al puntero que apunta al inicio de la lista.
 * Después de la liberación, este puntero se establece en nullptr.
 */
void liberarLista(Celda*& cabeza) {
    Celda* current = cabeza;
    while (current) {
        Celda* temp = current;
        current = current->siguiente;
        delete temp;
    }
    cabeza = nullptr;
}

/**
 * Inserta una nueva celda al final de una lista enlazada de celdas.
 * param cabeza Referencia al puntero que apunta al inicio de la lista.
 * param col Columna de la nueva celda.
 * param row Fila de la nueva celda.
 * param numEnemies Referencia al contador de enemigos para controlar la cantidad máxima.
 */
void insertarCelda(Celda*& cabeza, char col, int row, int& numEnemies) {
    Celda* newCell = crearCelda(col, row);

    if (rand() % 10 == 0 && numEnemies < 10) {
        newCell->hasEnemy = true;
        newCell->enemyHealth = newCell->piso + 1;
        newCell->enemyAttack = newCell->piso;
        ++numEnemies;
    }

    if (rand() % 10 == 0) {
        newCell->hasSavePoint = true;
    }

    if (rand() % 10 == 0) {
        newCell->hasTavern = true;
    }

    if (rand() % 4 == 0) {
        newCell->hasChest = true;
        newCell->chestContent = rand() % 3 + 1;
    }

    if (!cabeza) {
        cabeza = newCell;
    }
    else {
        Celda* current = cabeza;
        while (current->siguiente) {
            current = current->siguiente;
        }
        current->siguiente = newCell;
    }
}

/**
 * Guarda una lista de celdas en un archivo de texto.
 * param cabeza Puntero al inicio de la lista de celdas a guardar.
 */
void guardarCeldasEnArchivo(const Celda* cabeza) {
    std::ofstream archivo("celdas.txt");
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo 'celdas.txt' para guardar las celdas." << std::endl;
        return;
    }

    const Celda* current = cabeza;
    while (current) {
        archivo << current->piso << " "
            << current->column << " "
            << current->row << " "
            << current->visited << " "
            << current->hasEnemy << " "
            << current->hasSavePoint << " "
            << current->hasTavern << " "
            << current->hasChest << " "
            << current->enemyHealth << " "
            << current->enemyAttack << " "
            << current->chestContent << std::endl;
        current = current->siguiente;
    }

    archivo.close();
    std::cout << "Lista de celdas guardada correctamente en 'celdas.txt'." << std::endl;
}

/**
 * Carga una lista de celdas desde un archivo de texto.
 * param cabeza Referencia al puntero que apunta al inicio de la lista de celdas a cargar.
 */
void cargarCeldasDesdeArchivo(Celda*& cabeza) {
    std::ifstream archivo("celdas.txt");
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo 'celdas.txt' para cargar las celdas." << std::endl;
        return;
    }

    liberarLista(cabeza); // Liberar la lista actual de celdas

    int piso, row, enemyHealth, enemyAttack, chestContent;
    char column;
    bool visited, hasEnemy, hasSavePoint, hasTavern, hasChest;

    while (archivo >> piso >> column >> row >> visited >> hasEnemy
        >> hasSavePoint >> hasTavern >> hasChest >> enemyHealth >> enemyAttack >> chestContent) {
        pisoCalabozo = piso;
        Celda* newCell = crearCelda(column, row);
        newCell->piso = piso;
        newCell->hasPlayer = false;
        newCell->visited = visited;
        newCell->hasEnemy = hasEnemy;
        newCell->hasSavePoint = hasSavePoint;
        newCell->hasTavern = hasTavern;
        newCell->hasChest = hasChest;
        newCell->enemyHealth = enemyHealth;
        newCell->enemyAttack = enemyAttack;
        newCell->chestContent = chestContent;
        newCell->siguiente = nullptr;

        if (!cabeza) {
            cabeza = newCell;
        }
        else {
            Celda* current = cabeza;
            while (current->siguiente) {
                current = current->siguiente;
            }
            current->siguiente = newCell;
        }
    }

    archivo.close();
    std::cout << "Lista de celdas cargada correctamente desde 'celdas.txt'." << std::endl;
}

/**
 * Crea un calabozo generando una cuadrícula de celdas y las inserta en una lista enlazada.
 *        Cada celda tiene una probabilidad de contener enemigos, puntos de guardado, tabernas o cofres.
 * param cabeza Referencia al puntero que apunta al inicio de la lista de celdas del calabozo.
 * param numEnemies Referencia al contador de enemigos generados en el calabozo.
 */
void crearCalabozo(Celda*& cabeza, int& numEnemies) {
    for (char col = 'A'; col <= 'J'; ++col) {
        for (int row = 1; row <= 10; ++row) {
            insertarCelda(cabeza, col, row, numEnemies);
        }
    }
}

/**
 * Coloca al jugador en una celda específica del calabozo.
 *        La celda inicial del jugador se encuentra en la columna 'A' y fila 1.
 *
 * param cabeza Referencia al puntero que apunta al inicio de la lista de celdas del calabozo.
 * param jugador Referencia al objeto Jugador que representa al jugador del juego.
 */
void colocarJugador(Celda*& cabeza, Jugador& jugador) {
    Celda* current = cabeza;
    while (current) {
        if (current->column == 'A' && current->row == 1) {
            jugador.posicion = current;
            current->visited = true; // Marcar la celda como visitada
            // Mantener la salud actual y el poder de ataque del jugador
            jugador.health = jugador.health;
            jugador.attackPower = jugador.attackPower;
            break;
        }
        current = current->siguiente;
    }
}

/**
 * Guarda la información del jugador en un archivo de texto.
 * param jugador Referencia constante al objeto Jugador que contiene la información a guardar.
 */
void guardarInformacionJugador(const Jugador& jugador) {
    std::ofstream archivo("jugador.txt");

    if (archivo.is_open()) {
        archivo << jugador.health << std::endl;
        archivo << jugador.attackPower << std::endl;
        archivo << jugador.posicion->column << jugador.posicion->row << std::endl;
        archivo << jugador.equipo.size() << std::endl;

        for (const auto& recluta : jugador.equipo) {
            archivo << recluta.nombre << std::endl;
            archivo << recluta.health << std::endl;
            archivo << recluta.attackPower << std::endl;
        }

        archivo.close();
        std::cout << "Informacion del jugador guardado correctamente en 'jugador.txt'." << std::endl;
    }
    else {
        std::cerr << "Error: No se pudo abrir el archivo 'jugador.txt' para guardar la información." << std::endl;
    }
}

/**
 * Carga la información del jugador desde un archivo de texto.
 * param jugador Referencia al objeto Jugador donde se cargarán los datos.
 * param cabeza Puntero a la cabeza de la lista de celdas del calabozo.
 * return true si la carga fue exitosa, false si hubo algún error al abrir el archivo.
 */
bool cargarInformacionJugador(Jugador& jugador, Celda*& cabeza) {
    std::ifstream archivo("jugador.txt");
    if (!archivo.is_open()) {
        return false;
    }

    archivo >> jugador.health;
    archivo >> jugador.attackPower;

    char col;
    int row;
    archivo >> col >> row;

    // Buscar la celda correspondiente en la lista, aquí colocamos de una vez al jugador en la casilla
    Celda* current = cabeza;
    while (current) {
        if (current->column == col && current->row == row) {
            jugador.posicion = current;

            current->hasPlayer = true; // Marcar la celda con el jugador
            current->visited = true;   // Marcar la celda como visitada
            break;
        }
        current = current->siguiente;
    }

    int numReclutas;
    archivo >> numReclutas;
    jugador.equipo.clear();

    for (int i = 0; i < numReclutas; ++i) {
        Recluta recluta;
        archivo >> recluta.nombre;
        archivo >> recluta.health;
        archivo >> recluta.attackPower;
        jugador.equipo.push_back(recluta);
    }

    archivo >> numDiceThrows; // Cargar el número de tiradas de dados

    archivo.close();
    std::cout << "Informacion del jugador cargado correctamente desde 'jugador.txt'." << std::endl;
    return true;
}

/**
 * Inicia la pelea final con el Arcangel.
 * param jugador Referencia al objeto Jugador.
 * param arcangel Referencia al objeto Arcangel.
 */
void pelearConArcangel(Jugador& jugador, Arcangel& arcangel) {
    std::cout << "\nHas encontrado al Arcangel! Preparate para la batalla final!" << std::endl;

    // Mostrar información inicial de la pelea
    std::cout << "Arcangel - Salud: " << arcangel.health << " | Poder de Ataque: " << arcangel.attackPower << std::endl;
    std::cout << "Jugador - Salud: " << jugador.health << " | Poder de Ataque: " << jugador.attackPower << std::endl;

    bool turnoJugador = (rand() % 2 == 0); // Decidir aleatoriamente quién comienza primero
    int turnos = 1;

    while (jugador.health > 0 && arcangel.health > 0) {
        std::cout << "\n--- Turno " << turnos++ << " ---" << std::endl;

        if (turnoJugador) {
            // Turno del jugador y sus reclutas
            int totalAttack = jugador.attackPower;

            // Sumar el ataque de todos los reclutas en el equipo del jugador
            for (const auto& recluta : jugador.equipo) {
                totalAttack += recluta.attackPower;
            }

            arcangel.health -= totalAttack; // Atacar al Arcángel
            std::cout << "Atacas al Arcangel causando " << totalAttack << " puntos de dano." << std::endl;
        }
        else {
            // Turno del Arcángel
            std::vector<int> objetivos; // Índices para seleccionar objetivo
            objetivos.push_back(0); // El objetivo puede ser el jugador primero
            for (int i = 0; i < jugador.equipo.size(); ++i) {
                objetivos.push_back(i + 1); // Agregar reclutas como objetivos
            }

            int objetivoSeleccionado = objetivos[rand() % objetivos.size()]; // Elegir aleatoriamente un objetivo

            if (objetivoSeleccionado == 0) {
                // El Arcángel ataca al jugador
                int damage = arcangel.attackPower;
                jugador.health -= damage;
                std::cout << "El Arcangel te ataca y causa " << damage << " puntos de dano." << std::endl;
            }
            else {
                // El Arcángel ataca a un recluta
                int reclutaIndex = objetivoSeleccionado - 1;
                int damage = arcangel.attackPower;
                jugador.equipo[reclutaIndex].health -= damage;
                std::cout << "El Arcangel ataca a " << jugador.equipo[reclutaIndex].nombre << " y causa " << damage << " puntos de dano." << std::endl;

                // Verificar si el recluta ha sido derrotado
                if (jugador.equipo[reclutaIndex].health <= 0) {
                    std::cout << jugador.equipo[reclutaIndex].nombre << " ha sido derrotado." << std::endl;
                    jugador.equipo.erase(jugador.equipo.begin() + reclutaIndex); // Eliminar recluta derrotado
                }
            }
        }

        // Mostrar estado actual después del turno
        std::cout << "Arcangel - Salud: " << arcangel.health << " | Poder de Ataque: " << arcangel.attackPower << std::endl;
        std::cout << "Jugador - Salud: " << jugador.health << " | Poder de Ataque: " << jugador.attackPower << std::endl;

        turnoJugador = !turnoJugador; // Cambiar turno
    }

    // Mostrar resultado final de la pelea
    if (jugador.health > 0) {
        std::cout << "\nHas derrotado al Arcangel! Felicidades, has completado el calabozo." << std::endl;
    }
    else {
        std::cout << "\nEl Arcangel te ha derrotado! Intenta nuevamente." << std::endl;
    }

    juego = false;
}

/**
 * Realiza un combate con un enemigo.
 * param jugador Referencia al objeto Jugador.
 * param enemigo Puntero a la celda que contiene al enemigo.
 */
void combatirEnemigo(Jugador& jugador, Celda* enemigo) {
    std::cout << "Te has encontrado con un enemigo! ¡Preparate para el combate!" << std::endl;

    bool turnoJugador = (rand() % 2 == 1); // Decidir aleatoriamente si el jugador comienza primero

    while (jugador.health > 0 && enemigo->enemyHealth > 0) {
        if (turnoJugador) {
            // Turno del jugador y sus reclutas
            int totalAttack = jugador.attackPower;

            // Sumar el ataque de todos los reclutas en el equipo del jugador
            for (const auto& recluta : jugador.equipo) {
                totalAttack += recluta.attackPower;
            }

            enemigo->enemyHealth -= totalAttack;
            std::cout << "Has infligido " << totalAttack << " puntos de dano al enemigo." << std::endl;

            if (enemigo->enemyHealth <= 0) {
                std::cout << "Has derrotado al enemigo!" << std::endl;
                enemigo->hasEnemy = false; // Eliminar al enemigo de la celda
                return;
            }
        }
        else {
            // Turno del enemigo
            // Seleccionar aleatoriamente un objetivo dentro de la party del jugador
            std::vector<int> objetivos;
            objetivos.push_back(0); // El objetivo puede ser el jugador primero
            for (int i = 0; i < jugador.equipo.size(); ++i) {
                objetivos.push_back(i + 1); // Agregar reclutas como objetivos
            }

            int objetivoSeleccionado = objetivos[rand() % objetivos.size()]; // Elegir aleatoriamente un objetivo
            int damage = enemigo->enemyAttack;

            if (objetivoSeleccionado == 0) {
                // El enemigo ataca al jugador
                jugador.health -= damage;
                std::cout << "El enemigo te ha infligido " << damage << " puntos de dano." << std::endl;
            }
            else {
                // El enemigo ataca a un recluta
                int reclutaIndex = objetivoSeleccionado - 1;
                jugador.equipo[reclutaIndex].health -= damage;
                std::cout << "El enemigo ha infligido " << damage << " puntos de dano a " << jugador.equipo[reclutaIndex].nombre << "." << std::endl;

                // Verificar si el recluta ha sido derrotado
                if (jugador.equipo[reclutaIndex].health <= 0) {
                    std::cout << jugador.equipo[reclutaIndex].nombre << " ha sido derrotado." << std::endl;
                    jugador.equipo.erase(jugador.equipo.begin() + reclutaIndex); // Eliminar recluta derrotado
                }
            }

            if (jugador.health <= 0) {
                std::cout << "El enemigo te ha derrotado! ¡Intenta nuevamente!" << std::endl;
                juego = false; // Terminar el juego

                return;
            }
        }

        turnoJugador = !turnoJugador; // Cambiar turno
    }
}

/**
 * Añade un recluta aleatorio al equipo del jugador.
 * param jugador Referencia al objeto Jugador.
 */
void anadirReclutaAleatorioAJugador(Jugador& jugador) {
    // Lista de reclutas disponibles
    std::vector<Recluta> reclutasDisponibles = {
        {"Recluta1", 5, 5},
        {"Recluta2", 6, 4},
        {"Recluta3", 1, 1},
        {"Recluta4", 4, 6},
        {"Recluta5", 2, 4}
    };

    // Verificar si el jugador puede reclutar más reclutas
    if (jugador.equipo.size() < 3) {
        // Generar un índice aleatorio para seleccionar un recluta
        int indiceAleatorio = rand() % reclutasDisponibles.size();

        // Añadir el recluta seleccionado al equipo del jugador
        jugador.equipo.push_back(reclutasDisponibles[indiceAleatorio]);

        std::cout << "Has reclutado a " << reclutasDisponibles[indiceAleatorio].nombre << " en tu equipo!" << std::endl;
    }
    else {
        std::cout << "No puedes reclutar mas Reclutas. Tu equipo esta completo." << std::endl;
    }
}

/**
 * Verifica y procesa los eventos de una celda específica.
 * param cabeza Puntero a la cabeza de la lista de celdas.
 * param jugador Referencia al objeto Jugador.
 * param arcangel Referencia al objeto Arcangel.
 */
void verificarCelda(Celda*& cabeza, Jugador& jugador, Arcangel& arcangel) {
    Celda* current = jugador.posicion;

    if (current->hasEnemy) {
        // Realizar combate con el enemigo en la celda actual
        combatirEnemigo(jugador, current);
        current->hasEnemy = false;
    }

    if (current->hasSavePoint) {
        std::cout << "Has encontrado un punto de salvado. Se ha Guardado tu progreso aqui." << std::endl;
        guardarInformacionJugador(jugador); // Guardar la información del jugador en un archivo
        guardarCeldasEnArchivo(current);
        current->visited = true;
    }

    if (current->hasTavern) {
        std::cout << "Has encontrado una taberna. Descansa y recluta a alguien." << std::endl;
        anadirReclutaAleatorioAJugador(jugador);
        current->hasTavern = false;
    }

    if (current->hasChest) {
        std::cout << "Has encontrado un cofre. Quizás contenga algo útil." << std::endl;
        switch (current->chestContent) {

        case 1: //Aumenta el ataque (jugador y reclutas)
            std::cout << "Has encontrado un arma en el cofre! Aumenta tu poder de ataque." << std::endl;

            jugador.attackPower += 5;

            for (auto& recluta : jugador.equipo) {
                recluta.attackPower = recluta.attackPower + 2;
            }

            break;

        case 2: // Recuperar vida para todos los personajes (jugador y reclutas)

            std::cout << "Has encontrado un aumento en los puntos de salud en el cofre!" << std::endl;

            jugador.health += 1;
            for (auto& recluta : jugador.equipo) {
                recluta.health += 1;
            }
            break;

        case 3: // Recuperar el 10 % de los PS, nunca menos de 1 PS.

            std::cout << "Has encontrado un objeto para recuperar puntos de salud en el cofre!" << std::endl;
            int recoveryAmount = static_cast<int>(jugador.health * 0.1);
            jugador.health = std::max(jugador.health + recoveryAmount, 1);
            break;
        }
        current->hasChest = false; // Eliminar el cofre de la celda
    }
}

/**
 * Muesta las caracteristicas del jugador.
 * param jugador Referencia al objeto Jugador.
 */
void mostrarCaracteristicas(const Jugador& jugador) {
    std::cout << "\nEstado del jugador:\n";
    std::cout << " - Salud: " << jugador.health << std::endl;
    std::cout << " - Poder de ataque: " << jugador.attackPower << std::endl;
    std::cout << " - Equipo:" << std::endl;
    for (const auto& recluta : jugador.equipo) {
        std::cout << "   * Nombre: " << recluta.nombre << ", Salud: " << recluta.health << ", Poder de ataque: " << recluta.attackPower << std::endl;
    }
    std::cout << " - Tiradas de dados realizadas: " << numDiceThrows << std::endl;
}

/**
 * Muesta las caracteristicas del tablero y del jugador.
 * param cabeza Puntero a la cabeza de la lista de celdas.
 * param jugador Referencia al objeto Jugador.
 */
void mostrarEstado(Celda* cabeza, const Jugador& jugador) {
    std::cout << "\n---------------------------------------------------------------------------------------------------" << std::endl; // AQUI PDORIAMOS LIMPIAR PANTALLA TAMBIEN
    std::cout << "Calabozo - Estado del Piso " << pisoCalabozo << ":" << std::endl;
    std::cout << "   A   B   C   D   E   F   G   H   I   J" << std::endl;
    for (int row = 1; row <= 10; ++row) {
        std::cout << row;
        for (char col = 'A'; col <= 'J'; ++col) {
            Celda* current = cabeza;
            while (current) {
                if (current->column == col && current->row == row) {
                    if (current == jugador.posicion) {
                        std::cout << " [x]";
                    }
                    else if (current->visited) {
                        std::cout << " [.]";
                    }
                    else {
                        if (current->hasEnemy) {
                            std::cout << " [E]";
                        }
                        else if (current->hasSavePoint) {
                            std::cout << " [S]";
                        }
                        else if (current->hasTavern) {
                            std::cout << " [T]";
                        }
                        else if (current->hasChest) {
                            std::cout << " [C]";
                        }
                        else {
                            std::cout << " [ ]";
                        }
                    }
                    break;
                }
                current = current->siguiente;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------------------------------------------------------------------------------------" << std::endl; // AQUI PDORIAMOS LIMPIAR PANTALLA TAMBIEN
    mostrarCaracteristicas(jugador); // Mostrar características del jugador
}

/**
 * Mueve al jugador a través de las celdas del calabozo basado en el lanzamiento de dados.
 * Después de lanzar los dados, el jugador puede moverse en una dirección específica (arriba, abajo, izquierda, derecha)
 * determinada por la entrada del usuario. El movimiento es limitado por la cantidad de pasos obtenidos en el lanzamiento.
 * Si el jugador llega a la celda 'J10' en el último piso (piso 10), se inicia una pelea con el Arcángel.
 * Si el jugador llega a la celda 'J10' en cualquier otro piso, avanza al siguiente piso.
 * Se verifica si la nueva posición del jugador está dentro de los límites del calabozo y se actualiza la posición del jugador.
 * La función termina si se excede el límite de tiradas de dados permitidas.
 *
 * param cabeza Puntero a la cabeza de la lista de celdas del calabozo.
 * param jugador Referencia al objeto Jugador que se está moviendo.
 * param arcangel Referencia al objeto Arcangel para iniciar la pelea si se llega a la celda 'J10' en el piso 10.
 */
void moverJugador(Celda*& cabeza, Jugador& jugador, Arcangel& arcangel) {

    std::cout << "\nPresiona Enter para lanzar los dados...";
    std::cin.ignore(); // Ignorar cualquier entrada anterior
    std::cin.get(); // Esperar a que se presione Enter

    int dice1 = rand() % 6 + 1;
    int dice2 = rand() % 6 + 1;
    int totalSteps = dice1 + dice2;
    numDiceThrows++;

    std::cout << "\nLanzaste los dados. Puedes avanzar " << totalSteps << " pasos." << std::endl;

    char direccion;
    std::cout << "Elige una direccion para moverte (W, A, S, D): ";
    std::cin >> direccion;

    Celda* current = jugador.posicion;
    int newRow = current->row;
    char newColumn = current->column;

    // Avanzar exactamente la cantidad de pasos determinada por los dados
    while (totalSteps > 0) {
        switch (direccion) {
        case 'W':  // Mover hacia arriba
            if (newRow > 1) {
                --newRow;
            }
            break;
        case 'S':  // Mover hacia abajo
            if (newRow < 10) {
                ++newRow;
            }
            break;
        case 'A':  // Mover hacia la izquierda
            if (newColumn > 'A') {
                --newColumn;
            }
            break;
        case 'D':  // Mover hacia la derecha
            if (newColumn < 'J') {
                ++newColumn;
            }
            break;
        default:
            std::cout << "Direccion de movimiento no valida." << std::endl;
            break;
        }

        // Verificar si el jugador llega a la celda J10 en el piso 10
        if (newColumn == 'J' && newRow == 10 && current->piso == 10) {
            pelearConArcangel(jugador, arcangel);
            return;
        }

        // Verificar límite de la celda J10
        if (newColumn == 'J' && newRow == 10) {
            std::cout << "\nHas llegado a la salida del piso (J10)! Iniciando nuevo piso." << std::endl;
            ++pisoCalabozo; // Incrementar el número de piso
            liberarLista(cabeza); // Liberar la memoria del calabozo actual
            crearCalabozo(cabeza, numEnemies); // Crear un nuevo calabozo con nuevas características
            numDiceThrows = 0;
            colocarJugador(cabeza, jugador); // Colocar al jugador en la nueva posición inicial
            mostrarEstado(cabeza, jugador); // Mostrar el estado del nuevo calabozo
            return;
        }

        --totalSteps;
    }

    // Verificar si la nueva posición está dentro de los límites del calabozo
    Celda* newCell = cabeza;
    while (newCell) {
        if (newCell->column == newColumn && newCell->row == newRow) {
            jugador.posicion->hasPlayer = false;
            newCell->hasPlayer = true;
            jugador.posicion = newCell;

            verificarCelda(cabeza, jugador, arcangel); // Verificar la nueva celda
            if (juego) {
                mostrarEstado(cabeza, jugador);
            }
            break;
        }
        newCell = newCell->siguiente;
    }

    // Restricción para perder el juego si se tiran los dados más de 15 veces
    if (numDiceThrows > 15) {
        std::cout << "Has excedido el límite de tiradas de dados permitidas. ¡Has perdido el juego!" << std::endl;
        juego = false;
        return;
    }
}

void mostrarTexto() {
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "                  Nueva Aventura" << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "En las tierras ancestrales de Erresire, un joven orco valiente y decidido," << std::endl;
    std::cout << "comienza su odisea hacia el misterioso Calabozo del Arcangel." << std::endl;
    std::cout << "Su objetivo: derrotar al legendario Arcangel y reclamar su poder," << std::endl;
    std::cout << "que lo convertira en el senor supremo del continente." << std::endl;
    std::cout << "Con su libro de hechizos para principiantes en una mano y la determinacion" << std::endl;
    std::cout << "ardiente en su corazón, Erresire se adentra en las profundidades del calabozo," << std::endl;
    std::cout << "con la esperanza de forjar su destino y alcanzar la gloria." << std::endl;
    std::cout << "Que comience la epica aventura de Erresire en su busqueda de poder y redencion!" << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;

    std::cout << "Presiona Enter para continuar...";
    std::cin.ignore(); // Ignorar cualquier entrada anterior
    std::cin.get(); // Esperar a que se presione Enter
}

void mostrarMenu() {
    std::cout << "Bienvenido al juego del Calabozo!" << std::endl;
    std::cout << "1. Iniciar nueva partida" << std::endl;
    std::cout << "2. Cargar partida guardada" << std::endl;
    std::cout << "3. Salir" << std::endl;
}

void limpiarPantalla() {
    std::cout << "\033[2J\033[1;1H"; // Código ANSI para limpiar la pantalla
}

int main() {
    srand(time(nullptr));

    Celda* cabeza = nullptr;
    Jugador jugador;
    Arcangel arcangel;


    mostrarMenu();

    int opcion;
    std::cout << "Seleccione una opcion: ";
    std::cin >> opcion;

    switch (opcion) {
    case 1: // Iniciar nueva partida

        limpiarPantalla();
        mostrarTexto();
        limpiarPantalla();

        crearCalabozo(cabeza, numEnemies);
        colocarJugador(cabeza, jugador);
        mostrarEstado(cabeza, jugador);

        break;
    case 2: // Cargar partida guardada

        limpiarPantalla();
        cargarCeldasDesdeArchivo(cabeza);
        cargarInformacionJugador(jugador, cabeza); //tambien se coloca de una vez el jugador
        mostrarEstado(cabeza, jugador);

        break;
    case 3: // Salir del juego

        limpiarPantalla();
        std::cout << "Gracias por jugar. Hasta luego!" << std::endl;
        return 0;
    default:
        std::cout << "Opción no valida. Saliendo del juego." << std::endl;
        return 0;
    }

    // JUEGO
    while (juego) {
        moverJugador(cabeza, jugador, arcangel);
    }

    // liberacion de memoria
    liberarLista(cabeza);

    return 0;
}
