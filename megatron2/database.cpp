#include "database.h"
#include "data_manager.h"
#include "pesos.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <cmath>
#include <filesystem>
using namespace std;
#define esquemas "Esquemas"
#define tamanio_sector 1000 // 10000 bits

void Database::init() {
    string nombre_relacion;
    string commands;
    string string_id;
    string operador;
    string separator;
    string nombre_nueva_relacion;
    int dato_operador;
    int idRegister;
    int cant_platos, cant_superficies, cant_pistas, cant_sectores, tamanioBloque;
    while (true) {
        cout << "-------------------WELCOME TO MEGATRON 3000!------------------" << endl;
        cout << "The commands:" << endl;
        cout << "1. create_esquema" << endl;
        cout << "2. create_relacion" << endl;
        cout << "3. SELECT*FROM" << endl;
        cout << "4. SELECT*FROM_" << endl;
        cout << "5. SELECT" << endl;
        cout << "5. SEARCH" << endl;
        cout << "6. exit" << endl;
        cout << "Write the command to realize:" << endl;
        cin >> commands;
        
        if (commands == "create_esquema")
            SchemaManager::create_esquema();
        else if (commands == "create_relacion")
            SchemaManager::create_relacion();
        else if (commands == "SELECT*FROM") { // SELECT*FROM Titanic
            cout << "RELATION NAME: " << endl;
            cin >> nombre_relacion;
            DataManager::consultas_datos(nombre_relacion);
        }
        else if (commands == "SELECT*FROM_") { // SELECT*FROM_ Estudiantes WHERE id < 64
            cout << "RELATION NAME: " << endl;
            cin >> nombre_relacion;
            cout << "WHERE (name column)" << " ";
            cin >> string_id;
            cout << "OPERATOR:" << endl;
            cin >> operador;
            cout << "NUMBER OPERATOR" << endl;
            cin >> dato_operador;
            cout << "SEPARATOR (NULL && |)" << endl;
            cin >> separator;
            if (separator == "NULL") {
                cout << endl;
                DataManager::where(nombre_relacion, string_id, operador, dato_operador); 
                cout << endl; 
            } else if (separator == "|") {
                cout << endl;
                cout << "NEW RELATION:" << endl;
                cin >> nombre_nueva_relacion; 
                cout << endl;
                DataManager::where_condition(nombre_relacion, string_id, operador, dato_operador, separator, nombre_nueva_relacion);
                cout << endl;
            }
        }
        else if (commands == "SELECT") {
            vector<string> columnas;
            string columna;
            while (cin >> columna) {
                cout << "COLUMNS? (string):" << endl;
                if (columna == "FROM") break;
                columnas.push_back(columna);
            }
            cin >> nombre_relacion;
            DataManager::consultas_tablas_final(nombre_relacion, columnas);
        }
        else if (commands == "SEARCH") {
            ifstream archivo("data_disco");
            archivo >> cant_platos >> cant_superficies >> cant_pistas >> cant_sectores;
            archivo.close();
            cout << "ID register? " << endl;
            cin >> idRegister;
            SchemaManager::dictionary_search(idRegister, cant_platos, cant_superficies, cant_pistas, cant_sectores);
        }
        else if (commands == "exit")
            break;
        else
            cout << "Not a valid command" << endl;
    }
}
