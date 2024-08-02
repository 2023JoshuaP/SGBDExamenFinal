#include "./Pagina.h"
//#include "../megatron2/data_manager.cpp"
//#include "../megatron2/pesos.cpp"
#include <algorithm>
//#include "../Acceso/BPlusTree.cpp"

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
void insertionRegister(BPlusTree* Btree, string data, int rolNo) {
    cout << "rolNo= " << rolNo << endl;
    cout << "Data a añadir " << data << endl;
    //cout << "Ingrese nueva data: ";
    //getline(cin, data);

    string fileName = "../Archivos/" + to_string(rolNo) + ".txt";

    ifstream infile(fileName);
    if (infile.good()) {
        infile.close();
        FILE* filePtr = fopen(fileName.c_str(), "a");  // Open the file in append mode
        if (filePtr == NULL) {
            perror("Error al abrir el archivo");
            return;
        }
        (Btree)->insert(rolNo, filePtr, data);
    } else {
        FILE* filePtr = fopen(fileName.c_str(), "w");
        if (filePtr == NULL) {
            perror("Error al crear el archivo");
            return;
        }
        string userTuple = to_string(rolNo) + " " + data + "\n";
        fprintf(filePtr, "%s", userTuple.c_str());
        fclose(filePtr);
        (Btree)->insert(rolNo, filePtr, data);
        cout << "Inserción del rol: " << rolNo << " exitosa." << endl;
    }
}

void deletionRegister(BPlusTree* BTree, int tmp) {
    cout << "Showing you the Tree, Choose a key from here: " << endl;
    //BTree->display(BTree->getRoot());
    BTree->removeKey(tmp);
    //Displaying
    BTree->display(BTree->getRoot());
}

void Pagina::leerOescribirEnPagina(BPlusTree* Btree) {
    int opcion;
    int id;
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
            cout << "ID registro a eliminar: "; 
            cin >> id;
            cin.ignore();
            deletionRegister(Btree, id);

            for (size_t i = 0; i < this->vectorRegistrosEnPagina.size(); ++i) {
                stringstream ss(this->vectorRegistrosEnPagina[i]);
                string idStr;
                getline(ss, idStr, ',');
                
                try {
                    if (stoi(idStr) == id) {
                        this->vectorRegistrosEnPagina.erase(this->vectorRegistrosEnPagina.begin() + i);
                        break;
                    }
                } catch (const std::invalid_argument& e) {
                    cout << "Error: El registro " << this->vectorRegistrosEnPagina[i] << " tiene un ID inválido." << endl;
                    continue;
                }
            }

            cout << "Registros actualizados:" << endl;
            for (const auto& registro : vectorRegistrosEnPagina) {
                cout << registro << endl;
            }
        }
        else if (opcion == 3) {
            int rolNo;
            string data;
            cout << "-------------Agregar nuevo registro-----------" << endl;
            cout << "(1) Key a ingresar" << endl;
            cin >> rolNo;
            cout << "(2) Data nueva a ingresar" << endl;
            cin >> data;
            getline(cin, data);

            insertionRegister(Btree, data, rolNo);
            this->vectorRegistrosEnPagina.push_back(data);
            
            cout << "Registro agregado" << endl;
            cout << "------------------------------------------------------------------------" << endl;
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
        cout << "La ruta donde se guardo el registro es: " << rutaBloqueEscribir << endl;
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
            cout << "La ruta donde se guardo el registro es: " << rutaBloqueEscribirDisco << endl;
        }
    }
}
