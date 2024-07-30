#include "data_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <cmath>

using namespace std;
#define esquemas "Esquemas"
#define tamanio_sector 1000

string DataManager::determinarTipoDato(const string& valor) {
    if (valor.empty()) {
        return "empty";
    }

    bool soloDigitos = true;
    bool puntoDecimal = false;

    for (char c : valor) {
        if (isdigit(c)) {
            continue;
        } else if (c == '.' && !puntoDecimal) {
            puntoDecimal = true;
        } else {
            soloDigitos = false;
            break;
        }
    }

    if (soloDigitos) {
        if (puntoDecimal) {
            return "float";
        } else {
            return "int";
        }
    } else {
        return "string";
    }
}

string DataManager::toLowercase(string str) {
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c) { return tolower(c); });
    return str;
}

string DataManager::capitalize(string& str) {
    if (!str.empty()) {
        str[0] = toupper(str[0]);
        transform(str.begin() + 1, str.end(), str.begin() + 1, [](unsigned char c) { return tolower(c); });
    }
    return str;
}

vector<string> DataManager::get_type_from_line(string line) {
    vector<string> types;
    string value;
    stringstream ss(line);
    while (getline(ss, value, ';')) {
        types.push_back(determinarTipoDato(value));
    }
    return types;
}

void DataManager::consultas_datos(string nombre_relacion){
    string linea_esquema;
    vector <string> elementos;
    ifstream archivo_entrada(esquemas);
    string linea;
    int pos;
    while(getline(archivo_entrada, linea_esquema)){
        string palabra_linea_esquema;
        istringstream linea_esquema_stream(linea_esquema);
        pos = linea_esquema.find(nombre_relacion);
        if (pos == 0) {
            while (getline(linea_esquema_stream, palabra_linea_esquema, '#')) {
                if (pos > 0 && pos % 2 == 1) {
                    elementos.push_back(palabra_linea_esquema);        
                }
                pos++;
            }
            break;
        }
    }
    int idx = 0;
    for (auto i : elementos) {
        cout << i;
        if (idx < elementos.size() - 1) {
            cout << "\t#\t";
            idx++;
        }
    }
    cout << endl;
    cout << "-----------------------------------------------------" << endl;
    
    ifstream data_relacion(nombre_relacion);
    int idj = 0;
    while(getline(data_relacion, linea_esquema)) {
        // cout << linea_esquema << endl;
        stringstream ss(linea_esquema);
        while (getline(ss, linea_esquema, '#')) {
            cout << linea_esquema;
            if (idj < idx) {
                cout << "\t#\t";
                idj++;
            }
        }
        idj = 0;
        cout << endl;
    } 
}

// vector<string> DataManager::consultar_columnas(string nombre_columna) {
//     vector<string> columnas;
//     columnas.push_back(nombre_columna);
//     return columnas;
// }

template <typename... Args>
vector<string> DataManager::consultar_columnas(string nombre_columna, Args... args) {
    vector<string> columnas;
    columnas.push_back(nombre_columna);
    vector<string> columnas_siguientes = consultar_columnas(args...);
    columnas.insert(columnas.end(), columnas_siguientes.begin(), columnas_siguientes.end());
    return columnas;
}

vector<string> DataManager::consultas_tablas(string nombre_relacion, string columna) {
    string linea_esquema;
    string linea_data;
    vector<string> elementos;
    ifstream archivo_entrada(esquemas);
    ifstream archivo_data(nombre_relacion);
    string linea;
    int pos;
    int pos_columna = -1;
    cout << "[DEBUG] Buscando esquema para la relación: " << nombre_relacion << endl;

    while (getline(archivo_entrada, linea_esquema)) {
        istringstream linea_esquema_stream(linea_esquema);
        pos = linea_esquema.find(nombre_relacion);
        string columna_esquema;
        int columna_posicion = 0;
        if (pos == 0) {
            cout << "[DEBUG] Esquema encontrado: " << linea_esquema << endl;
            while (getline(linea_esquema_stream, columna_esquema, '#')) {
                if (columna_esquema == columna) {
                    pos_columna = columna_posicion;
                    cout << "[DEBUG] Columna encontrada: " << columna << " en posición " << pos_columna << endl;
                    break;
                }
                if (pos % 2 != 0) {
                    columna_posicion++;
                }
                pos++;
            }
            if (pos_columna != -1) {
                break;
            }
        }
    }

    if (pos_columna == -1) {
        cout << "[ERROR] No se encontró la columna " << columna << " en la relación " << nombre_relacion << endl;
        return elementos;
    }
    cout << "[DEBUG] Leyendo datos de la relación: " << nombre_relacion << endl;

    int pos_data = 0;
    while (getline(archivo_data, linea_data)) {
        stringstream ss(linea_data);
        string valor;
        while (getline(ss, valor, '#')) {
            if (pos_data == pos_columna) {
                elementos.push_back(valor);
                cout << "[DEBUG] Valor encontrado: " << valor << endl;
            }
            pos_data++;
        }
        pos_data = 0;
    }
    archivo_entrada.close();
    archivo_data.close();
    cout << "[DEBUG] Resultados encontrados:" << endl;
    for (const auto& elem : elementos) {
        cout << elem << endl;
    }

    if (elementos.empty()) {
        cout << "[INFO] No se encontraron datos para la consulta." << endl;
    }

    return elementos;
}

void DataManager::mezclar_data(vector<vector<string>> data) {
    int data_size = data[0].size();
    int aux = 0;
    while (aux < data_size) {
        for (int i = 0; i < data.size(); i++) {
            cout << data[i][aux];
            if (i < data.size() - 1) cout << "\t#\t";
        }
        cout << endl;
        aux++;
    }
}

void DataManager::consultas_tablas_final(string nombre_relacion, vector<string> columnas) {
    vector<vector<string>> data;
    for (int i = 0; i < columnas.size(); i++) {
        cout << columnas[i];
        if (i < columnas.size() - 1) cout << "\t#\t";
        data.push_back(consultas_tablas(nombre_relacion, columnas[i]));
    }
    cout << endl;
    mezclar_data(data);
}

void DataManager::where(string relacion, string data_string, string simbolo, int compare) {
    string linea_esquema;
    ifstream nombre_relacion(esquemas);
    int pos;
    int pos_columna_data = -1;
    vector<string> elementos;

    cout << "[DEBUG] Buscando esquema para la relación: " << relacion << endl;

    while (getline(nombre_relacion, linea_esquema)) {
        string palabra_linea_esquema;
        istringstream linea_esquema_stream(linea_esquema);
        pos = linea_esquema.find(relacion);
        int pos_columna = -1;

        if (pos == 0) {
            cout << "[DEBUG] Esquema encontrado: " << linea_esquema << endl;
            while (getline(linea_esquema_stream, palabra_linea_esquema, '#')) {
                pos_columna++;
                if (palabra_linea_esquema == data_string) {
                    pos_columna_data = pos_columna;
                    
                    cout << "[DEBUG] Columna encontrada para " << data_string << ": " << pos_columna_data << endl;
                }
            }
            break;
        }
    }

    if (pos_columna_data == -1) {
        cout << "[ERROR] No se encontró la columna " << data_string << " en la relación " << relacion << endl;
        return;
    }

    nombre_relacion.close();
    cout << "[DEBUG] Buscando datos en la relación: " << relacion << endl;
    pos_columna_data--;
    string linea_relacion;
    ifstream data(relacion);
    while (getline(data, linea_relacion)) {
        string palabra_linea_relacion;
        istringstream linea_relacion_stream(linea_relacion);
        int pos_relacion = 0;

        while (getline(linea_relacion_stream, palabra_linea_relacion, '#')) {
            if (pos_relacion == pos_columna_data) {
                bool match = false;
                int valor = stoi(palabra_linea_relacion);

                if (simbolo == ">") {
                    match = valor > compare;
                } else if (simbolo == ">=") {
                    match = valor >= compare;
                } else if (simbolo == "<") {
                    match = valor < compare;
                } else if (simbolo == "<=") {
                    match = valor <= compare;
                } else if (simbolo == "==") {
                    match = valor == compare;
                } else if (simbolo == "!=") {
                    match = valor != compare;
                } else {
                    cout << "[ERROR] Operador no soportado: " << simbolo << endl;
                    return;
                }
                if (match) {
                    elementos.push_back(linea_relacion);
                    cout << "[DEBUG] Coincidencia encontrada: " << linea_relacion << endl;
                }
            }
            pos_relacion++;
        }
    }

    data.close();

    cout << "[DEBUG] Resultados encontrados:" << endl;
    for (const auto& elem : elementos) {
        cout << elem << endl;
    }

    if (elementos.empty()) {
        cout << "[INFO] No se encontraron coincidencias para la consulta." << endl;
    }
}

void DataManager::where_condition(string relacion, string data_string, string simbolo, int compare, string sign, string name_new_relation) {
    string linea_esquema;
    ifstream nombre_relacion(esquemas);
    int pos;
    int pos_string;
    int pos_columna_data = -1;
    vector<string> elementos;
    cout << "[DEBUG] Buscando esquema para la relación: " << relacion << endl;
    while (getline(nombre_relacion, linea_esquema)) {
        string palabra_linea_esquema;
        istringstream linea_esquema_stream(linea_esquema);
        pos = linea_esquema.find(relacion);
        int pos_columna = -1;

        if (pos == 0) {
            cout << "[DEBUG] Esquema encontrado: " << linea_esquema << endl;
            while (getline(linea_esquema_stream, palabra_linea_esquema, '#')) {
                pos_columna++;
                if (palabra_linea_esquema == data_string) {
                    pos_columna_data = pos_columna;
                    cout << "[DEBUG] Columna encontrada para " << data_string << ": " << pos_columna_data << endl;
                }
            }
            break;
        }
    }
    if (pos_columna_data == -1) {
        cout << "[ERROR] No se encontró la columna " << data_string << " en la relación " << relacion << endl;
        return;
    }
    nombre_relacion.close();
    cout << "[DEBUG] Buscando datos en la relación: " << relacion << endl;

    string linea_relacion;
    ifstream data(relacion);
    while (getline(data, linea_relacion)) {
        string palabra_linea_relacion;
        istringstream linea_relacion_stream(linea_relacion);
        int pos_relacion = 0;
        while (getline(linea_relacion_stream, palabra_linea_relacion, '#')) {
            if (pos_relacion == pos_columna_data) {
                bool match = false;

                // Verificación de valor numérico
                try {
                    int valor = stoi(palabra_linea_relacion);

                    if (simbolo == ">" && valor > compare) {
                        match = true;
                    } else if (simbolo == ">=" && valor >= compare) {
                        match = true;
                    } else if (simbolo == "<" && valor < compare) {
                        match = true;
                    } else if (simbolo == "<=" && valor <= compare) {
                        match = true;
                    } else if (simbolo == "==" && valor == compare) {
                        match = true;
                    } else if (simbolo == "!=" && valor != compare) {
                        match = true;
                    } else {
                        cout << "[ERROR] Operador no soportado: " << simbolo << endl;
                        return;
                    }
                } catch (const std::invalid_argument&) {
                    cout << "[ERROR] Valor no numérico encontrado en la columna: " << palabra_linea_relacion << endl;
                    return;
                }

                if (match) {
                    elementos.push_back(linea_relacion);
                    cout << "[DEBUG] Coincidencia encontrada: " << linea_relacion << endl;
                }
            }
            pos_relacion++;
        }
    }

    data.close();

    if (sign == "|") {
        string linea_esquema2;
        name_new_relation = capitalize(name_new_relation);
        std::fstream archivo_esquemas(esquemas, std::ios::in | std::ios::out | std::ios::app);
        archivo_esquemas.seekp(0, std::ios::end);
        archivo_esquemas << name_new_relation << "#"; 
        int idx = 0;
        vector<string> elementos2;
        archivo_esquemas.seekg(0, std::ios::beg);

        cout << "[DEBUG] Añadiendo esquema a la nueva relación: " << name_new_relation << endl;

        while (getline(archivo_esquemas, linea_esquema2)) {
            string palabra_linea_esquema;
            istringstream linea_esquema_stream2(linea_esquema2);
            pos = linea_esquema2.find(relacion);
            if (pos == 0) {
                while (getline(linea_esquema_stream2, palabra_linea_esquema, '#')) {
                    if (pos > 0) {
                        elementos2.push_back(palabra_linea_esquema);
                    }
                    pos++;
                }
                break;
            }
        }

        for (int i = 0; i < elementos2.size(); i++) {
            archivo_esquemas << elementos2[i];
            if (i < elementos2.size() - 1) {
                archivo_esquemas << "#";
            }
        }
        archivo_esquemas << endl;
        archivo_esquemas.close();

        cout << "[DEBUG] Esquema añadido a la nueva relación" << endl;
    }

    cout << "[DEBUG] Resultados encontrados:" << endl;
    for (const auto& elem : elementos) {
        cout << elem << endl;
    }

    if (elementos.empty()) {
        cout << "[INFO] No se encontraron coincidencias para la consulta." << endl;
    } else {
        ofstream archivo_nuevo(name_new_relation);
        for (const auto& i : elementos) {
            archivo_nuevo << i << endl;
        }
        archivo_nuevo.close();
        cout << "[DEBUG] Nueva relación creada: " << name_new_relation << endl;
    }
}

vector<string> DataManager::split(string str, char delimiter) {
    vector<string> res;
    istringstream ss(str);
    string tok;
  
    while(getline(ss, tok, delimiter)) {
        if (tok != "") res.push_back(tok);
    } 
    return res;
}
void DataManager::Bloque(int tamanioBloque, int cant_platos, int cant_superficies, int cant_pistas, int cant_sectores) {
    int cantidadBloques;
    cout << "Ingrese la cantidad de bloques que desea crear: ";
    cin >> cantidadBloques;

    vector<vector<string>> vectores(cantidadBloques);  

    int plato, superficie, pista, sector;
    int currentBlockIndex = 0;  // Índice del bloque actual
    int currentBlockSize = 0;   // Tamaño actual del bloque

    for (plato = 1; plato <= cant_platos; plato++) {
        for (superficie = 1; superficie <= cant_superficies; superficie++) {
            for (pista = 1; pista <= cant_pistas; pista++) {
                for (sector = 1; sector <= cant_sectores; sector++) {
                    string primer_sector = "../Disco/Disco/" + to_string(plato) + "/" + to_string(superficie) + "/" + to_string(pista) + "/" + to_string(sector);
                    int peso_sector = Peso::peso_sector(primer_sector);
                    
                    if (currentBlockSize + peso_sector > tamanioBloque) {
                        currentBlockIndex++;
                        currentBlockSize = 0;
                        if (currentBlockIndex >= cantidadBloques) {
                            cout << "Se ha alcanzado la cantidad máxima de bloques." << endl;
                            // Imprimir el contenido de todos los bloques al alcanzar el límite
                            for (int i = 0; i < cantidadBloques; ++i) {
                                cout << "Bloque " << i + 1 << ":" << endl;
                                for (const string& registro : vectores[i]) {
                                    cout << registro << endl;
                                }
                                cout << endl;
                            }
                            return;
                        }
                    }

                    ifstream directorio(primer_sector);
                    if (!directorio) {
                        cerr << "Error al abrir el archivo: " << primer_sector << endl;
                        continue; // Saltar a la siguiente iteración si no se puede abrir el archivo
                    }

                    string linea_registros;
                    while (getline(directorio, linea_registros)) {
                        vectores[currentBlockIndex].push_back(linea_registros);
                        currentBlockSize += linea_registros.size();
                        if (currentBlockSize >= tamanioBloque) {
                            currentBlockIndex++;
                            currentBlockSize = 0;
                            if (currentBlockIndex >= cantidadBloques) {
                                cout << "Se ha alcanzado la cantidad máxima de bloques." << endl;
                                // Imprimir el contenido de todos los bloques al alcanzar el límite
                            
                            }
                        }
                    }
                }
            }
        }
    }

    // Imprimir el contenido de todos los bloques después de procesar todos los archivos
    for (int i = 0; i < cantidadBloques; ++i) {
        cout << "Bloque " << i + 1 << ":" << endl;
        for (const string& registro : vectores[i]) {
            cout << registro << endl;
        }
        cout << endl;
    }
}