#include <iostream>
#include <string>
#include <clocale>
using namespace std;

// Definici�n de la estructura Nodo para representar a una persona en un �rbol geneal�gico
struct Nodo {
    string id;         // Identificador �nico de la persona
    string nombre;     // Nombre de la persona
    string apellido;   // Apellido de la persona
    Nodo* padre;       // Puntero al nodo del padre
    Nodo* izq;         // Hijo izquierdo (puede representar al primer hijo)
    Nodo* der;         // Hijo derecho (puede representar al segundo hijo)
};

// Funci�n para buscar una persona en el �rbol por su ID
Nodo* buscarPorID(Nodo* nodo, const string& id) {
    if (nodo == NULL) return NULL;               // Si el nodo es nulo, no se encontr�
    if (nodo->id == id) return nodo;             // Si el ID coincide, se encontr�
    Nodo* encontrado = buscarPorID(nodo->izq, id); // Buscar recursivamente en el sub�rbol izquierdo
    if (encontrado) return encontrado;             // Si se encontr� en el sub�rbol izquierdo, devolverlo
    return buscarPorID(nodo->der, id);             // Si no, buscar en el sub�rbol derecho
}

// Funci�n para verificar si un nodo es ancestro de otro (para evitar ciclos)
bool esAncestro(Nodo* posibleAncestro, Nodo* nodo) {
    while (nodo != NULL) {
        if (nodo == posibleAncestro)              // Si encontramos el nodo en la cadena ascendente, es ancestro
            return true;
        nodo = nodo->padre;                       // Subir al padre
    }
    return false;                                 // Si se llega al final sin coincidencias, no es ancestro
}

// Funci�n para insertar una nueva persona en el �rbol bajo un padre especificado
bool insertarPersona(Nodo* raiz, Nodo* nueva, const string& padreID) {
    Nodo* padre = buscarPorID(raiz, padreID);     // Buscar el nodo del padre
    if (padre == NULL) {
        cout << "Error: Padre no encontrado.\n";  // Validar existencia del padre
        return false;
    }
    if (nueva->padre != NULL) {
        // Validar que la nueva persona no tenga ya un padre asignado
        cout << "Error: Esta persona ya tiene un padre asignado (" << nueva->padre->id << ").\n";
        return false;
    }
    if (esAncestro(nueva, padre)) {
        // Prevenir ciclos: no se puede insertar un ancestro como hijo de su descendiente
        cout << "Error: No se puede insertar (crear a un ciclo).\n";
        return false;
    }
    nueva->padre = padre;                          // Asignar el padre
    if (padre->izq == NULL) {
        padre->izq = nueva;                        // Insertar como hijo izquierdo si est� disponible
        return true;
    } else if (padre->der == NULL) {
        padre->der = nueva;                        // Insertar como hijo derecho si est� disponible
        return true;
    } else {
        cout << "Error: El padre ya tiene dos hijos.\n"; // Si ya tiene dos hijos, no se puede insertar
        return false;
    }
}

// Recorrido en preorden: visita ra�z, luego hijo izquierdo y luego derecho
void preorden(Nodo* nodo) {
    if (nodo != NULL) {
        cout << nodo->nombre << " " << nodo->apellido << " (" << nodo->id << ")\n";
        preorden(nodo->izq);  // Visitar hijo izquierdo
        preorden(nodo->der);  // Visitar hijo derecho
    }
}

// Recorrido en inorden: hijo izquierdo, luego ra�z, luego derecho
void inorden(Nodo* nodo) {
    if (nodo != NULL) {
        inorden(nodo->izq);   // Visitar hijo izquierdo
        cout << nodo->nombre << " " << nodo->apellido << " (" << nodo->id << ")\n";
        inorden(nodo->der);   // Visitar hijo derecho
    }
}

// Recorrido en postorden: hijo izquierdo, hijo derecho, luego ra�z
void postorden(Nodo* nodo) {
    if (nodo != NULL) {
        postorden(nodo->izq);  // Visitar hijo izquierdo
        postorden(nodo->der);  // Visitar hijo derecho
        cout << nodo->nombre << " " << nodo->apellido << " (" << nodo->id << ")\n";
    }
}

// Muestra la ascendencia detallada de una persona, incluyendo niveles, padres y hermanos
void mostrarAscendenciaDetallada(Nodo* persona) {
    if (persona == NULL) {
        cout << "Persona no encontrada.\n"; // Verifica si la persona existe
        return;
    }

    cout << "\nPersona encontrada: " << persona->nombre << " " << persona->apellido << " (" << persona->id << ")\n";

    Nodo* camino[100];   // Arreglo para guardar la ruta hasta la ra�z
    int nivel = 0;
    Nodo* actual = persona;
    while (actual != NULL) {
        camino[nivel++] = actual; // Guarda cada nodo del camino hacia arriba
        actual = actual->padre;
    }

    // Recorre el camino desde la ra�z hasta la persona buscada
    for (int i = nivel - 1; i >= 0; i--) {
        actual = camino[i];
        cout << "\nNivel " << (nivel - i) << ":\n  ";

        if (actual->padre != NULL) {
            Nodo* hermano = NULL;

            // Determina si existe un hermano (el otro hijo del padre)
            if (actual->padre->izq == actual && actual->padre->der != NULL)
                hermano = actual->padre->der;
            else if (actual->padre->der == actual && actual->padre->izq != NULL)
                hermano = actual->padre->izq;

            // Imprime la informaci�n de la persona en ese nivel
            cout << actual->nombre << " " << actual->apellido << " (" << actual->id;
            if (i == nivel - 1) cout << ") (Raiz)";      // Es la ra�z
            else if (i == 0) cout << ") (buscado)";      // Es la persona buscada
            else cout << ") (padre)";                    // Es un ancestro intermedio

            // Si hay hermano, mostrar su informaci�n tambi�n
            if (hermano != NULL) {
                cout << "    " << hermano->nombre << " " << hermano->apellido << " (" << hermano->id;
                if (i == 0) cout << ") (hermano)";       // Hermano del nodo buscado
                else cout << ")";                        // Hermano de un ancestro
            } else {
                cout << "\n"; // Nueva l�nea si no hay hermano
            }
        } else {
            // Es la ra�z del �rbol (no tiene padre)
            cout << actual->nombre << " " << actual->apellido << " (" << actual->id << ") (Raiz)\n";
        }
    }
    cout << "\n";
}

// Permite a�adir una nueva persona al �rbol
void anadirPersona(Nodo*& raiz) {
    Nodo* nueva = new Nodo();  // Crear nuevo nodo

    cout << "ID unico: ";
    getline(cin, nueva->id);

    // Verificar si ya existe una persona con ese ID
    if (raiz != NULL && buscarPorID(raiz, nueva->id) != NULL) {
        cout << "Error: Ya existe una persona con ese ID.\n";
        delete nueva; // Liberar memoria si ya existe
        return;
    }

    // Ingresar nombre y apellido
    cout << "Nombre: ";
    getline(cin, nueva->nombre);
    cout << "Apellido: ";
    getline(cin, nueva->apellido);

    // Inicializar punteros
    nueva->izq = nueva->der = nueva->padre = NULL;

    if (raiz == NULL) {
        raiz = nueva; // Si el �rbol est� vac�o, esta persona es la ra�z
        cout << "Anadido como raiz del arbol.\n";
    } else {
        // Solicitar ID del padre y tratar de insertar
        string padreID;
        cout << "ID del padre: ";
        getline(cin, padreID);
        if (!insertarPersona(raiz, nueva, padreID)) {
            delete nueva; // Si falla, liberar memoria
        } else {
            cout << "Persona anadida al arbol.\n";
        }
    }
}

// Permite eliminar una persona del �rbol si no tiene hijos
void eliminarPersona(Nodo*& raiz, const string& id) {
    Nodo* objetivo = buscarPorID(raiz, id); // Buscar a la persona
    if (objetivo == NULL) {
        cout << "Error: Persona no encontrada.\n";
        return;
    }

    // No se permite eliminar personas con hijos
    if (objetivo->izq != NULL || objetivo->der != NULL) {
        cout << "Error: No se puede eliminar. Esta persona tiene hijos.\n";
        return;
    }

    // Si es la ra�z y no tiene hijos, eliminarla y vaciar el �rbol
    if (objetivo == raiz) {
        delete raiz;
        raiz = NULL;
        cout << "Raiz eliminada. Arbol vacio.\n";
        return;
    }

    // Si no es la ra�z, eliminarla del padre
    Nodo* padre = objetivo->padre;
    if (padre->izq == objetivo)
        padre->izq = NULL;
    else if (padre->der == objetivo)
        padre->der = NULL;

    delete objetivo; // Liberar memoria del nodo
    cout << "Persona eliminada correctamente.\n";
}

// Submen� para mostrar los distintos recorridos del �rbol
void submenuMetodosBusqueda(Nodo* raiz) {
    int subopcion;
    do {
        cout << "\n--- METODOS DE BUSQUEDA ---\n";
        cout << "[1]: Mostrar Preorden\n";
        cout << "[2]: Mostrar Inorden\n";
        cout << "[3]: Mostrar Postorden\n";
        cout << "[4]: Volver al menu principal\n";

        // Validar que se elija una opci�n v�lida
        do {
            cout << "Elegir opcion: ";
            cin >> subopcion;
            if (subopcion < 1 || subopcion > 4) {
                cout << "ERROR, opcion incorrecta.\n";
            }
        } while (subopcion < 1 || subopcion > 4);

        cin.ignore(); // Limpiar buffer de entrada

        switch (subopcion) {
            case 1:
                // Mostrar recorrido preorden
                cout << "\n-- Recorrido Preorden --\n";
                preorden(raiz);
                break;
            case 2:
                // Mostrar recorrido inorden
                cout << "\n-- Recorrido Inorden --\n";
                inorden(raiz);
                break;
            case 3:
                // Mostrar recorrido postorden
                cout << "\n-- Recorrido Postorden --\n";
                postorden(raiz);
                break;
            case 4:
                // Volver al men� principal
                cout << "Regresando al menu principal...\n";
                break;
        }

        // Pausar y limpiar pantalla si no se sale
        if (subopcion != 4) {
            system("pause");
            system("cls");
        }

    } while (subopcion != 4);
}

// Permite buscar una persona por su ID y mostrar su ascendencia detallada
void buscarPersonaYAscendencia(Nodo* raiz) {
    string id;
    cout << "Ingrese el ID de la persona: ";
    getline(cin, id); // Leer el ID de la persona
    Nodo* persona = buscarPorID(raiz, id); // Buscar persona
    mostrarAscendenciaDetallada(persona);  // Mostrar su ascendencia
}

// Funci�n principal que gestiona el men� del �rbol geneal�gico
void menu() {
    Nodo* raiz = NULL; // �rbol inicialmente vac�o
    int op;

    do {
        // Mostrar opciones del men� principal
        cout << "*** MENU ARBOL GENEALOGICO ***\n";
        cout << "[1]: Anadir persona\n";
        cout << "[2]: Eliminar persona\n";
        cout << "[3]: Metodos de busqueda (Preorden, Inorden, Postorden)\n";
        cout << "[4]: Buscar persona por ID (ascendencia)\n";
        cout << "[5]: Salir\n";

        // Validar opci�n ingresada
        do {
            cout << "Elegir opcion: ";
            cin >> op;
            if (op < 1 || op > 5) {
                cout << "ERROR, opcion incorrecta\n";
            }
        } while (op < 1 || op > 5);

        cin.ignore(); // Limpiar buffer

        // Ejecutar opci�n elegida
        switch (op) {
            case 1:
                // A�adir nueva persona al �rbol
                anadirPersona(raiz);
                break;
            case 2: {
                // Eliminar persona del �rbol
                string id;
                cout << "ID de la persona a eliminar: ";
                getline(cin, id);
                eliminarPersona(raiz, id);
                break;
            }
            case 3:
                // Acceder a submen� de m�todos de recorrido
                submenuMetodosBusqueda(raiz);
                break;
            case 4:
                // Buscar persona y mostrar su ascendencia
                buscarPersonaYAscendencia(raiz);
                break;
            case 5:
                // Salir del programa
                break;
        }

        // Pausar y limpiar pantalla si no se ha elegido salir
        if (op != 5) {
            system("pause");
            system("cls");
        }

    } while (op != 5);
}

// Funci�n principal del programa
int main() {
    setlocale(LC_ALL, "Spanish"); // Configura la codificaci�n para admitir caracteres en espa�ol
    menu();                        // Llama al men� principal
    return 0;                      // Fin del programa
}


