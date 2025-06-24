#include <iostream>
#include <string>
using namespace std;

// Estructura que representa a una persona en el arbol genealogico
struct Nodo {
    string id;           // Identificador unico de la persona
    string nombre;       // Nombre de la persona
    string apellido;     // Apellido de la persona
    Nodo* padre;         // Puntero al nodo del padre
    Nodo* izq;           // Hijo izquierdo (primer hijo)
    Nodo* der;           // Hijo derecho (segundo hijo)
};

// Funcion para buscar una persona en el arbol por su ID
Nodo* buscarPorID(Nodo* nodo, const string& id) {
    if (nodo == NULL) return NULL;                   // Caso base: arbol vacio
    if (nodo->id == id) return nodo;                 // Si encontramos el ID, retornamos el nodo

    // Busqueda recursiva primero por el hijo izquierdo, luego derecho
    Nodo* encontrado = buscarPorID(nodo->izq, id);
    if (encontrado) return encontrado;
    return buscarPorID(nodo->der, id);
}

// Funcion para verificar si un nodo es ancestro de otro (para evitar ciclos)
bool esAncestro(Nodo* posibleAncestro, Nodo* nodo) {
    while (nodo != NULL) {
        if (nodo == posibleAncestro)
            return true;                             // Si hay coincidencia, es ancestro
        nodo = nodo->padre;                          // Subimos por la rama del padre
    }
    return false;                                    // No se encontro el ancestro
}

// Funcion para insertar una nueva persona en el arbol bajo un padre existente
bool insertarPersona(Nodo* raiz, Nodo* nueva, const string& padreID) {
    Nodo* padre = buscarPorID(raiz, padreID);        // Buscamos al padre por su ID

    if (padre == NULL) {
        cout << "Error: Padre no encontrado.\n";
        return false;
    }

    if (nueva->padre != NULL) {
        cout << "Error: Esta persona ya tiene un padre asignado (" 
             << nueva->padre->id << ").\n";
        return false;
    }

    if (esAncestro(nueva, padre)) {
        cout << "Error: No se puede insertar (crearia un ciclo).\n";
        return false;
    }

    // Insertamos como hijo izquierdo si esta vacio
    if (padre->izq == NULL) {
        padre->izq = nueva;
        nueva->padre = padre;
        return true;
    }
    // Si no, insertamos como hijo derecho si esta disponible
    else if (padre->der == NULL) {
        padre->der = nueva;
        nueva->padre = padre;
        return true;
    } else {
        cout << "Error: El padre ya tiene dos hijos.\n";
        return false;
    }
}

// Recorrido en Preorden: Nodo -> Izquierda -> Derecha
void preorden(Nodo* nodo) {
    if (nodo != NULL) {
        cout << nodo->nombre << " " << nodo->apellido << " (" << nodo->id << ")\n";
        preorden(nodo->izq);
        preorden(nodo->der);
    }
}

// Recorrido en Inorden: Izquierda -> Nodo -> Derecha
void inorden(Nodo* nodo) {
    if (nodo != NULL) {
        inorden(nodo->izq);
        cout << nodo->nombre << " " << nodo->apellido << " (" << nodo->id << ")\n";
        inorden(nodo->der);
    }
}

// Funcion para mostrar la ascendencia de una persona hasta la raiz
void mostrarAscendencia(Nodo* persona) {
    Nodo* actual = persona;
    int nivel = 0;

    // Subimos por cada nodo padre hasta llegar a la raiz
    while (actual != NULL && actual->padre != NULL) {
        Nodo* padre = actual->padre;
        cout << "\nNivel " << ++nivel << ": Padre: " << padre->nombre << " " << padre->apellido << " (" << padre->id << ")";

        // Mostramos al hermano si existe
        if (padre->izq != NULL && padre->izq != actual)
            cout << "\n  Hermano: " << padre->izq->nombre << " " << padre->izq->apellido << " (" << padre->izq->id << ")";
        else if (padre->der != NULL && padre->der != actual)
            cout << "\n  Hermano: " << padre->der->nombre << " " << padre->der->apellido << " (" << padre->der->id << ")";

        actual = padre;
    }
}

// Funcion que contiene el menu principal del programa
void mostrarMenu() {
    Nodo* raiz = NULL;       // Arbol vacio al inicio
    int opcion;

    do {
        // Mostramos opciones disponibles al usuario
        cout << "\n=== MENU ARBOL GENEALOGICO ===\n";
        cout << "1. Anadir persona\n";
        cout << "2. Mostrar Preorden\n";
        cout << "3. Mostrar Inorden\n";
        cout << "4. Buscar persona por ID (ascendencia)\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();        // Limpieza del buffer

        switch (opcion) {
            case 1: {
                // Crear nuevo nodo persona
                Nodo* nueva = new Nodo();
                cout << "ID unico: ";
                getline(cin, nueva->id);

                // Verificamos si ya existe alguien con ese ID
                if (raiz != NULL && buscarPorID(raiz, nueva->id) != NULL) {
                    cout << "Error: Ya existe una persona con ese ID.\n";
                    delete nueva;
                    break;
                }

                // Recolectamos datos
                cout << "Nombre: ";
                getline(cin, nueva->nombre);
                cout << "Apellido: ";
                getline(cin, nueva->apellido);

                // Inicializamos punteros del nuevo nodo
                nueva->izq = nueva->der = nueva->padre = NULL;

                // Si el arbol esta vacio, esta persona se convierte en la raiz
                if (raiz == NULL) {
                    raiz = nueva;
                    cout << "Anadido como raiz del arbol.\n";
                } else {
                    string padreID;
                    cout << "ID del padre: ";
                    getline(cin, padreID);
                    // Intentamos insertar bajo ese padre
                    if (!insertarPersona(raiz, nueva, padreID)) {
                        delete nueva;  // Liberamos memoria si falla
                    } else {
                        cout << "Persona anadida al arbol.\n";
                    }
                }
                break;
            }

            case 2:
                cout << "\n-- Recorrido Preorden --\n";
                preorden(raiz);
                break;

            case 3:
                cout << "\n-- Recorrido Inorden --\n";
                inorden(raiz);
                break;

            case 4: {
                string id;
                cout << "Ingrese el ID de la persona: ";
                getline(cin, id);
                Nodo* persona = buscarPorID(raiz, id);
                if (persona != NULL) {
                    cout << "\nPersona encontrada: " << persona->nombre << " " << persona->apellido << " (" << persona->id << ")\n";
                    mostrarAscendencia(persona);
                } else {
                    cout << "Persona no encontrada.\n";
                }
                break;
            }

            case 5:
                cout << "Saliendo del programa...\n";
                break;

            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
        }

    } while (opcion != 5);   // El menu se repite hasta que el usuario elija salir
}

// Funcion principal
int main() {
    mostrarMenu();   // Llamamos al menu principal
    return 0;
}

