#include "./Pagina.h"
//#include "../megatron2/data_manager.cpp"
//#include "../megatron2/pesos.cpp"
#include <algorithm>

Pagina::Pagina() {}

Pagina::~Pagina(){}

int tamanioString = 0;
string rutaCompleta = "";

bool isNumber(const std::string &s) {
    for (char const &ch : s) {
        if (std::isdigit(ch) == 0 && ch != '.')
            return false;
    }
    return true;
}

void Pagina::recogerRegistros(int numPaginaEliminada) {
    string rutaDirectorio = "../megatron2/directorio.txt";
    ifstream directorio(rutaDirectorio);
    string rutaBloqueEscribir;
    string linea;

    while (getline(directorio, linea)) {
        size_t pos = linea.rfind('/');
        if (pos != string::npos) {
            string valor = linea.substr(pos + 1);
            if (stoi(valor) == numPaginaEliminada) {
                rutaBloqueEscribir = linea;
                cout << "El valor a considerar es " << valor << endl;
                break;
            }
        }
    }

    cout << "RutaBloque es " << rutaBloqueEscribir;
    stringstream ss2(rutaBloqueEscribir);
    vector<string> componentes;
    string valor2;

    while (getline(ss2, valor2, '/')) {
        componentes.push_back(valor2);
    }

  
    if (componentes.size() >= 1) { 
        rutaBloqueEscribir ="../Disco/Disco/Plato " + componentes[0] + "/Superficie " + componentes[1] + "/Pista " + componentes[2] + "/Bloque " + componentes[3] + ".txt";
    } else {
        cout << "No se encontró la ruta del bloque correspondiente." << endl;
        return;
    }

    ifstream rutaBloque(rutaBloqueEscribir);
    if (!rutaBloque.is_open()) {
        cout << "No se pudo abrir el archivo: " << rutaBloqueEscribir << endl;
        return;
    } else {
        int lineNumber = 0;
        while (getline(rutaBloque, linea)) {
            lineNumber++;
            if (lineNumber > 3) {
                this->vectorRegistrosEnPagina.push_back(linea);
            }
        }
        rutaBloque.close();
        cout << "Contenido de página en memoria mandado a bloque correctamente." << endl;
    }   
}

void Pagina::mostrarContenidoDePagina() {
    for (size_t i =0 ; i < this->vectorRegistrosEnPagina.size(); i++) {
        cout << this->vectorRegistrosEnPagina[i] << endl;
    }
    cout << "------------------------------------------------------------------------" << endl;
    
}
void Pagina::leerOescribirEnPagina() {
    int opcion;

    do {
        int filaAencontrar;
        string registroNuevo;
        string cadenaFinal = "";
        
        cout << "------------------Opcion de edicion de pagina----------------" << endl;
        cout << "(1) Editar contenido de pagina" << endl;
        cout << "(2) Eliminar contenido de pagina" << endl;
        cout << "(3) Agregar registro nuevo a pagina" << endl;
        cout << "(4) Salir" << endl;
        cout << "Ingrese opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            cout << "-------------Edicion de registros-----------" << endl; 
            cout << "Fila a editar: "; 
            cin >> filaAencontrar;
            cin.ignore();

            string& registro = this->vectorRegistrosEnPagina[filaAencontrar];
            cout << "Registro actual: " << registro << endl;

            vector<string> valoresRegistroNuevo;
            stringstream ss(registro);
            string valor;

            cout << "Registro nuevo: ";
            getline(cin, registroNuevo);

            stringstream ssNuevo(registroNuevo);

            while (getline(ssNuevo, valor, ',')) {
                valoresRegistroNuevo.push_back(valor);
            }

            for (size_t i = 0; i < valoresRegistroNuevo.size(); ++i) {
                if (isNumber(valoresRegistroNuevo[i])) {
                    cadenaFinal += valoresRegistroNuevo[i] + ',';
                } else {
                    cadenaFinal += valoresRegistroNuevo[i];
                    int pesoValor = valoresRegistroNuevo[i].size();
                    do {
                        cadenaFinal += " ";
                        pesoValor += 1;
                    } while (pesoValor < tamanioString);

                    if (i < valoresRegistroNuevo.size() - 1) {
                        cadenaFinal += ',';
                    }
                }
            }
            this->vectorRegistrosEnPagina[filaAencontrar] = cadenaFinal;
            cout << "Guardado" << endl;
            cout << "------------------------------------------------------------------------" << endl;
        } else if (opcion == 2) {
            cout << "-------------Eliminacion de registros-----------" << endl;
            cout << "Fila a eliminar: "; 
            cin >> filaAencontrar;
            cin.ignore();
            this->vectorRegistrosEnPagina[filaAencontrar] = " ";
        } else if (opcion == 3) {
            int OpcionRegistro;
            cout << "-------------Agregar nuevo registro-----------" << endl;
            cout << "(1) Longitud Fija" << endl;
            cout << "(2) Longitud Variable" << endl;
            cout << "Opcion? ";
            cin >> OpcionRegistro;
            
            string fileName = "../Archivos/esquema.txt";
            ifstream file(fileName);

            if (!file.is_open()) {
                cerr << "No se pudo abrir el archivo: " << fileName << endl;
                continue;
            }

            string line;
            vector<int> ultimosValores;

            while (getline(file, line)) {
                stringstream ss(line);
                string word;
                vector<string> words;

                while (ss >> word) {
                    words.push_back(word);
                }

                if (!words.empty()) {
                    try {
                        int ultimoValor = stoi(words.back());
                        ultimosValores.push_back(ultimoValor);
                    } catch (const invalid_argument &e) {
                        // Manejo del error
                    }
                }
            }

            file.close();

            if (OpcionRegistro == 1) {
                string name_archivo_relacion;
                string nuevo_registro;
                cout << "Archivo relacion ? " << endl;
                cin >> name_archivo_relacion;

                ifstream archivo_relacion(name_archivo_relacion);
                ifstream archivo_esquema("../Archivos/esquema.txt");
                string linea_relacion;
                string data_insert_relacion;
                string linea_esquema;
                bool primer_valor;
                int peso_cadena = 0;
                int peso_string;
                size_t i = 0;

                while (getline(archivo_relacion, linea_relacion)) {
                    primer_valor = false;
                    stringstream ss(linea_relacion);
                    string valor_relacion;
                    bool tipo_dato_aceptado = true;
                    do {
                        cout << linea_relacion << endl;
                        cout << "Insert data: ";
                        cin >> data_insert_relacion;

                        while (getline(ss, valor_relacion, ',')) {
                            if (primer_valor) {
                                for (i; i < ultimosValores.size(); i) {
                                    cout << "El valor considerado fue " << ultimosValores[i] << endl;
                                    if (valor_relacion == "string" || valor_relacion == "int" || valor_relacion == "float") {
                                        cout << "Tipo de dato aceptado" << endl;
                                        int peso_cadena = data_insert_relacion.length();
                                        peso_string = ultimosValores[i];
                                        while (peso_cadena < peso_string) {
                                            data_insert_relacion += " ";
                                            peso_cadena++;
                                        }
                                        nuevo_registro += data_insert_relacion + ' ';
                                    } else {
                                        cout << "Tipo de dato no aceptado" << endl;
                                        tipo_dato_aceptado = false;
                                    }
                                    i++;
                                    break;
                                }
                            } else {
                                primer_valor = true;
                            }
                        }
                        if (tipo_dato_aceptado) {
                            break;
                        }
                    } while (true);
                }

                this->vectorRegistrosEnPagina.push_back(nuevo_registro);
                cout << "Registro agregado" << endl;
                cout << "------------------------------------------------------------------------" << endl;
            }
        }
    } while (opcion != 4);
    cout << "Regresando al menú principal..." << endl;
}

bool Pagina::verificarPaginaVacia() {
    if (this->vectorRegistrosEnPagina.empty()) {
        return true;
    }
    else {
        return false;
    }
}

string Pagina::limpiarCadena(const string& cadena) {
    string resultado = cadena;
    resultado.erase(remove_if(resultado.begin(), resultado.end(), ::isspace), resultado.end());
    return resultado;
}

bool Pagina::buscarRegistro(string& registro) {
        string registroLimpio = limpiarCadena(registro);
        for (const auto& reg : this->vectorRegistrosEnPagina) {
            string regLimpio = limpiarCadena(reg);
            cout << "Comparando: [" << regLimpio << "] con [" << registroLimpio << "]" << endl;
            if (regLimpio == registroLimpio) {
                return true;
            }
        }
        return false;
    }

void Pagina::añadirRegistro(string& registro) {
    this->vectorRegistrosEnPagina.push_back(registro);
}

void Pagina::agregarContenido(int numPaginaEliminada) {
    string rutaDirectorio = "../megatron2/directorio";
    ifstream directorio(rutaDirectorio);
    string rutaBloqueEscribir;
    string linea;

    while (getline(directorio, linea)) {
        size_t pos = linea.rfind('/');
        if (pos != string::npos) {
            string valor = linea.substr(pos + 1);
            if (stoi(valor) == numPaginaEliminada) {
                rutaBloqueEscribir = linea;
                cout << "El valor a considerar es " << valor << endl;
                break;
            }
        }
    }

    cout << "RutaBloque es " << rutaBloqueEscribir;
    stringstream ss2(rutaBloqueEscribir);
    vector<string> componentes;
    string valor2;

    while (getline(ss2, valor2, '/')) {
        componentes.push_back(valor2);
    }

    if (componentes.size() >= 1) { 
        rutaBloqueEscribir = "../Disco/Disco/Plato " + componentes[0] + "/Superficie " + componentes[1] + "/Pista " + componentes[2] + "/Bloque " + componentes[3] + ".txt";
    } else {
        cout << "No se encontró la ruta del bloque correspondiente." << endl;
        return;
    }
    
    ofstream rutaBloque(rutaBloqueEscribir, ios::trunc);
    if (!rutaBloque.is_open()) {
        cout << "No se pudo abrir el archivo: " << rutaBloqueEscribir << endl;
    } else {
        for (int_least32_t i = 0; i < this->vectorRegistrosEnPagina.size(); ++i) {
            rutaBloque << this->vectorRegistrosEnPagina[i] << endl;
        }
        rutaBloque.close();
        cout << "Contenido de página en memoria mandado a bloque correctamente" << endl;
    }
    // Preguntar al usuario si también desea guardar en disco
    char respuesta;
    cout << "¿Desea también guardar en disco? (s/n): ";
    cin >> respuesta;

    if (respuesta == 's' || respuesta == 'S') {
        string rutaBloqueEscribirDisco = "../Disco/Disco/Plato " + componentes[0] + "/Superficie " + componentes[1] + "/Pista " + componentes[2] + "/" + componentes[3] + ".txt";
        
        ofstream rutaBloqueDisco(rutaBloqueEscribirDisco, ios::trunc);
        if (!rutaBloqueDisco.is_open()) {
            cout << "No se pudo abrir el archivo: " << rutaBloqueEscribirDisco << endl;
        } else {
            for (int_least32_t i = 0; i < this->vectorRegistrosEnPagina.size(); ++i) {
                rutaBloqueDisco << this->vectorRegistrosEnPagina[i] << endl;
            }
            rutaBloqueDisco.close();
            cout << "Contenido de página en memoria mandado a disco correctamente" << endl;
        }
    }
}
