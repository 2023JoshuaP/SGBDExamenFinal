#include "schema_manager.h"
#include "data_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <cmath>
#include <vector>
#include "global_variable.h"

using namespace std;

void SchemaManager::create_esquema(string relacion_nombre) {
    DataManager data_manager;
    string archivo_csv;
    if (relacion_nombre == "") {
        cout << "Enter relationship name: ";
        cin >> relacion_nombre;
    } else {
        cout << "The relationship will be created: " << relacion_nombre << endl;
    }
    relacion_nombre = data_manager.capitalize(relacion_nombre);
    cout << "Enter the name of the .csv file to create the schema" << endl;
    cin >> archivo_csv;
    ofstream archivo_salida_esquemas(esquemas, ios::app);
    ifstream archivo_entrada(archivo_csv);
    // Verificar que el archivo CSV se abrió correctamente
    if (!archivo_entrada.is_open()) {
        cerr << "Error opening CSV file: " << archivo_csv << endl;
        return;
    }
    archivo_salida_esquemas << relacion_nombre;
    string linea;
    // Solo leer la primera línea del archivo CSV
    if (getline(archivo_entrada, linea)) {
        stringstream ss(linea);
        string valor;
        while (getline(ss, valor, ',')) {
            archivo_salida_esquemas << "#";
            archivo_salida_esquemas << valor;
        }
    }
    archivo_salida_esquemas << endl;
    archivo_entrada.close();
    archivo_salida_esquemas.close();
    cout << "Schema created successfully" << endl;
}


bool SchemaManager::existe_relacion(string relacion) {
    ifstream archivo_entrada(esquemas);
    string linea;
    while(getline(archivo_entrada,linea,'#')){
        if(linea == relacion)
            return true;
    }
    return false;
}

void SchemaManager::create_relacion() {
    DataManager data_manager;
    string nombre_archivo_relacion;
    string nombre_relacion;
    cout << "Enter relationship name: " << endl;
    cin >> nombre_relacion;   
    
    if(!existe_relacion(data_manager.capitalize(nombre_relacion))) {
        cout << " The relationship does not exist " << nombre_relacion << endl;
        create_esquema(nombre_relacion);
    }
    cout << "Enter file name with .csv to extract data" << endl;
    cin >> nombre_archivo_relacion; //pedir al usuario el nombre del archivo a extraer los datos

    ifstream archivo_entrada(nombre_archivo_relacion);
    if (!archivo_entrada.is_open()) {
        cerr << "Error opening CSV file." << endl;
        return;
    }

    string nombre_archivo_salida = data_manager.capitalize(nombre_relacion); // Estudiantes
    // bool validacion = validate(nombre_archivo_salida, nombre_archivo_relacion);
    bool validacion = true;
    if (validacion == true) {
        ofstream archivo_salida(nombre_archivo_salida, ios::app); //Estudiantes
        string linea;
        bool primera_linea = true;

        while (getline(archivo_entrada, linea)) { //estudiantes_data.csv -> Piero;123;Lima
            if (primera_linea) {
                primera_linea = false;
                continue; 
            }
            int count_commas = count(linea.begin(), linea.end(), ',');
            int count_hashtag = 0;
            stringstream ss(linea);
            string valor;
            string linea_relacion = "";
            while (getline(ss, valor, ',')) {
                archivo_salida << valor;
                linea_relacion += valor;
                if (count_hashtag < count_commas) {
                    archivo_salida << "#";
                    linea_relacion += "#";
                    count_hashtag++;
                }
            }
            archivo_salida << endl;
            int cant_platos, cant_superficies, cant_pistas, cant_sectores;
            ifstream archivo_data_disco("data_disco");
            archivo_data_disco >> cant_platos >> cant_superficies >> cant_pistas >> cant_sectores;
            archivo_data_disco.close();
            new_register(nombre_relacion, cant_platos, cant_superficies, cant_pistas, cant_sectores, false, linea_relacion);
        }

        archivo_entrada.close();
        archivo_salida.close();
        cout << "Relation created successfully" << endl;
    }
    else {
        cout << "The data does not match the schema" << endl;
    }
}

string SchemaManager::encontrar_relacion(string nombre_relacion){
    string linea_esquema;
    ifstream archivo_entrada(esquemas);
    while(getline(archivo_entrada, linea_esquema)){
        if (linea_esquema.find(nombre_relacion) != -1) {
            stringstream ss(linea_esquema);
            int pos = 0;
            while (getline(ss, linea_esquema, '#')) {
                if (pos > 0 && pos % 2 != 0) {
                    cout << linea_esquema << '#';
                }
                pos += 1;
            }
            archivo_entrada.close();
            return linea_esquema;
        }
    }
    archivo_entrada.close();
    return "Not found";
}

void SchemaManager::new_register(string name_relacion, int cant_platos, int cant_superficies, int cant_pistas, int cant_sectores, bool llenar_desde_file, string n_registro, string name_archivo_relacion) {
    ofstream archivo_salida_relacion(name_relacion, ios::app);
    string linea_esquema;
    ifstream archivo_entrada_esquemas(esquemas);
    string nuevo_registro;    
 

    int length;
    cout << "You must place "<< linea_esquema <<endl;
    while(getline(archivo_entrada_esquemas, linea_esquema)) {
        if(linea_esquema.find(name_relacion) != -1){ 
            break;
        }
    }
    if (llenar_desde_file) {
        cout << "New register" <<endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, nuevo_registro);
    } else {
        nuevo_registro = n_registro;
    }
    
    bool espacio_disponible = false;
    int plato;
    int superficie;
    int pista;
    int sector;

    for (plato = 1; plato <= cant_platos; plato++) {
        for (superficie = 1; superficie <= cant_superficies; superficie++) {
            for (pista = 1; pista <= cant_pistas; pista++) {
                for (sector = 1; sector <= cant_sectores; sector++) {
                    string primer_sector = "../Disco/Disco/" + to_string(plato) + "/" + to_string(superficie) + "/" + to_string(pista) + "/" + to_string(sector);
                    if (Peso::verificar_peso(primer_sector, nuevo_registro)) {
                        espacio_disponible = true;
                        ofstream directorio_ubicacion("directorio", ios::app);
                        id++;
                        directorio_ubicacion << id << " " << plato  << " " << superficie  << " " << pista  << " " << sector << endl;
                        break;
                    }
                }
                if (espacio_disponible) break;
            }
            if (espacio_disponible) break;
        }
        if (espacio_disponible) break;
    }

    if (!espacio_disponible) {
        cout << endl;
    }
    else {
        string ruta_completa = "../Disco/Disco/" + to_string(plato) + "/" + to_string(superficie) + "/" + to_string(pista) + "/" + to_string(sector);

        ofstream archivo_salida_disco(ruta_completa, ios::app);

        if (archivo_salida_disco.is_open()) 
        {
            cout << "Inserts 0 for fixed length or 1 for variable length" << endl; cin >> length;
            if(length == 0 ) 
            {
                cout << "Archivo relacion ? " << endl;
                cin >> name_archivo_relacion;

                ifstream archivo_relacion (name_archivo_relacion);
                string linea_relacion;
                string data_insert_relacion;
                bool primer_valor;
                int peso_cadena = 0;
                int peso_string;

                cout << "Ingrese peso del string: "; cin >> peso_string;

                while (getline (archivo_relacion,linea_relacion))
                {
                    cout << linea_relacion << endl;
                    primer_valor = false;
                    do 
                    {
                        cout << "Insert data: ";
                        cin >> data_insert_relacion;
                        stringstream ss(linea_relacion);
                        string valor_relacion;
                        bool tipo_dato_aceptado = true;
                        cout << DataManager::determinarTipoDato(data_insert_relacion) << endl;
                        while (getline(ss, valor_relacion, ',')) 
                        {
                            if (primer_valor) 
                            {
                                if (valor_relacion == DataManager::determinarTipoDato(data_insert_relacion)) 
                                {
                                    cout << "Tipo de dato aceptado" << endl;
                                    int peso_cadena = data_insert_relacion.length();

                                    if (valor_relacion == "string")
                                    {
                                        while (peso_cadena < peso_string) 
                                        {
                                            data_insert_relacion += " ";
                                            peso_cadena++;
                                        }
                                    }
                                    nuevo_registro += data_insert_relacion + '#';

                                } else {
                                    cout << "Tipo de dato no aceptado" << endl;
                                    tipo_dato_aceptado = false;
                                }
                                break;
                            } 
                            else {
                                primer_valor = true;
                            }
                        }
                        if (tipo_dato_aceptado) 
                        {
                            break; 
                        }
                    } while (true);
                }
                archivo_salida_disco << id << " " << nuevo_registro << endl;
                cout << "El archivo se ha guardado en la nueva carpeta correctamente." << endl;
            }
            else if (length == 1) 
            {
                cout << endl;
            }   
        } 
        archivo_salida_relacion.close();
    }
}

void SchemaManager::dictionary_search(int id_data, int cant_platos, int cant_superficies, int cant_pistas, int cant_sectores) {
    bool encontrado = false;

    for (int plato = 1; plato <= cant_platos; plato++) {
        for (int superficie = 1; superficie <= cant_superficies; superficie++) {
            for (int pista = 1; pista <= cant_pistas; pista++) {
                for (int sector = 1; sector <= cant_sectores; sector++) {
                    string archivo_relacion = "../Disco/Disco/Plato " + to_string(plato) + "/Superficie " + to_string(superficie) + "/Pista " + to_string(pista) + "/" + to_string(sector) + ".txt";
                    ifstream data(archivo_relacion);
                    string linea_data;

                    cout << "[DEBUG] Buscando en archivo: " << archivo_relacion << endl;

                    if (!data.is_open()) {
                        cerr << "Error opening file: " << archivo_relacion << endl;
                        continue;
                    }

                    while (getline(data, linea_data)) {
                        istringstream data_ss(linea_data);
                        string id_str;
                        getline(data_ss, id_str, ',');

                        try {
                            int id = stoi(id_str);
                            if (id == id_data) {
                                cout << "Record found: " << linea_data << endl;
                                encontrado = true;
                                break;
                            }
                        } catch (const std::invalid_argument& e) {
                            cerr << "Invalid ID encountered: " << id_str << " in file " << archivo_relacion << endl;
                            continue;
                        } catch (const std::out_of_range& e) {
                            cerr << "ID out of range: " << id_str << " in file " << archivo_relacion << endl;
                            continue;
                        }
                    }

                    data.close();

                    if (encontrado) {
                        break;
                    }
                }
                if (encontrado) {
                    break;
                }
            }
            if (encontrado) {
                break;
            }
        }
        if (encontrado) {
            break;
        }
    }

    if (!encontrado) {
        cout << "Record with ID " << id_data << " not found in any file." << endl;
    }
}






