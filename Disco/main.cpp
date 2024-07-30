#include "Disco.cpp"
#include "../megatron2/pesos.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "../Acceso/BPlusTree.cpp"
#include <queue>
#include "../Buffer/BufferManager.cpp"
#include "../megatron2/database.h"
#include "../megatron2/data_manager.h"
using namespace std;

struct Requerimiento {
    int numPagina;
    char accion;
};

queue<Requerimiento> colaDeRequerimientos;
//queue<Requerimiento> colaDeEspera;

/* Funcion para ver REQUERIMIENTOS incial*/

void visualizarCola() {
    queue<Requerimiento> tempCola = colaDeRequerimientos;
    cout << "Cola de Requerimientos:" << endl;
    if (tempCola.empty()) {
        cout << "La cola está vacía." << endl;
        return;
    }
    while (!tempCola.empty()) {
        Requerimiento req = tempCola.front();
        tempCola.pop();
        cout << "Pagina: " << req.numPagina << ", Accion: " << (req.accion == 'L' || req.accion == 'l' ? "Leer" : "Escribir") << endl;
    }
}

void procesarColaEscritura(int paginaLiberar) {
    queue<Requerimiento> tempCola;
    bool escrituraEncontrada = false;
    bool lecturaEncontrada = false;

    while (!colaDeRequerimientos.empty()) {
        Requerimiento req = colaDeRequerimientos.front();
        colaDeRequerimientos.pop();
        
        if (req.numPagina == paginaLiberar && (req.accion == 'W' || req.accion == 'w')) {
            cout << "Procesando Escritura en Pagina: " << req.numPagina << endl;
            escrituraEncontrada = true;
            break;
        } else {
            tempCola.push(req);
        }
    }

    if (escrituraEncontrada) {
        while (!tempCola.empty()) {
            colaDeRequerimientos.push(tempCola.front());
            tempCola.pop();
        }
        
        cout << "Cola actualizada después de liberar una escritura:" << endl;
        visualizarCola();
        return;
    }

    tempCola = queue<Requerimiento>();
    while (!colaDeRequerimientos.empty()) {
        Requerimiento req = colaDeRequerimientos.front();
        colaDeRequerimientos.pop();

        if (req.numPagina == paginaLiberar && (req.accion == 'L' || req.accion == 'l')) {
            cout << "Procesando Lectura en Pagina: " << req.numPagina << endl;
            lecturaEncontrada = true;
            break;
        } else {
            tempCola.push(req);
        }
    }

    if (!escrituraEncontrada && lecturaEncontrada) {
        cout << "Cola actualizada después de liberar una lectura:" << endl;
        while (!tempCola.empty()) {
            colaDeRequerimientos.push(tempCola.front());
            tempCola.pop();
        }
        visualizarCola();
    }

    if (!escrituraEncontrada && !lecturaEncontrada) {
        cout << "Cola original sin liberaciones:" << endl;
        visualizarCola();
    }
}

bool noEscrituraEnCola(int pagina) {
    queue<Requerimiento> tempCola = colaDeRequerimientos;
    while (!tempCola.empty()) {
        Requerimiento req = tempCola.front();
        tempCola.pop();
        if (req.numPagina == pagina && (req.accion == 'W' || req.accion == 'w')) {
            return false; 
        }
    }
    return true; 
}

void procesarColaLectura(int paginaLiberar) {
    queue<Requerimiento> tempCola;
    bool escrituraEncontrada = false;
    bool lecturaEncontrada = false;

    while (!colaDeRequerimientos.empty()) {
        Requerimiento req = colaDeRequerimientos.front();
        colaDeRequerimientos.pop();
        
        if (req.numPagina == paginaLiberar && (req.accion == 'L' || req.accion == 'l')) {
            cout << "Procesando Escritura en Pagina: " << req.numPagina << endl;
            escrituraEncontrada = true;
            break;
        } else {
            tempCola.push(req);
        }
    }

    if (escrituraEncontrada) {
        while (!tempCola.empty()) {
            colaDeRequerimientos.push(tempCola.front());
            tempCola.pop();
        }
        
        cout << "Cola actualizada después de liberar una escritura:" << endl;
        visualizarCola();
        return;
    }

    tempCola = queue<Requerimiento>();
    while (!colaDeRequerimientos.empty()) {
        Requerimiento req = colaDeRequerimientos.front();
        colaDeRequerimientos.pop();

        if (req.numPagina == paginaLiberar && (req.accion == 'L' || req.accion == 'l')) {
            cout << "Procesando Lectura en Pagina: " << req.numPagina << endl;
            lecturaEncontrada = true;
            break;
        } else {
            tempCola.push(req);
        }
    }

    if (!escrituraEncontrada && lecturaEncontrada) {
        cout << "Cola actualizada después de liberar una lectura:" << endl;
        while (!tempCola.empty()) {
            colaDeRequerimientos.push(tempCola.front());
            tempCola.pop();
        }
        visualizarCola();
    }

    if (!escrituraEncontrada && !lecturaEncontrada) {
        cout << "Cola original sin liberaciones:" << endl;
        visualizarCola();
    }
}

void menuBufferManager(BufferManager &bufferManagerPrincipal) {
    cout << "------------Bienvenido a implementación de BUFFER-------------" << endl;
    int pesoBytesBLoque;
    cout << "Ingrese el peso de su bloque de Disco: " << endl;
    cin >> pesoBytesBLoque;
    int numFrames;
    cout << "Ingrese la cantidad de Frames a tener en el Buffer Pool: " << endl;
    cin >> numFrames;
    bufferManagerPrincipal.crearBufferPoolSegunNumFrames(numFrames);
    bufferManagerPrincipal.establecerLimiteDeFrames(pesoBytesBLoque);
    
    cout << "Creación de estructura Básica de BufferManager implementado." << endl;
}
void menuBuffer(BufferManager &bufferManagerPrincipal) {   
    int opcion = 0;
    int numPagina;
    int numFrame;
    int opcionPaginaGuardado;

    while (opcion != 8) {
        cout << endl;
        cout << "------- Menú Principal Programa Buffer Manager -------" << endl;
        cout << "1. Crear Buffer Pool" << endl;
        cout << "2. Obtener Una Página" << endl;
        cout << "3. Mostrar Page Table" << endl;
        cout << "4. Liberar uso de Página" << endl;
        cout << "5. Unpinnear Una Página" << endl;
        cout << "6. Visualizar Cola de Requerimientos" << endl;
        cout << "7. Mostrar contenido de una pagina" << endl;
        cout << "8. Salir del programa principal" << endl;
        cout << "Ingrese el numero de opcion" << endl;
        cin >> opcion;

        switch (opcion) {
            case 1: {
                menuBufferManager(bufferManagerPrincipal);
                break;
            }
            case 2: {
                cout << "Ingrese el numero de pagina que desea OBTENER (Modo Prueba 1 - 6): " << endl;
                cin >> numPagina;
                bufferManagerPrincipal.obtenerUnaPagina(numPagina);
                
                char accionPagina;
                cout << "Leer (L/l) o Escribir (W/w) en Pagina?: ";
                cin >> accionPagina;

                //bool requerimientoAgregado = false;
                
                
                //cout << "Requerimiento agregado a la cola." << endl;

                numFrame = bufferManagerPrincipal.pageTable.getNumFrameDeUnaPagina(numPagina);

                if (accionPagina == 'L' || accionPagina == 'l') {
                    cout << "Proceso de Lectura." << endl;
                    if (bufferManagerPrincipal.pageTable.verificarDirtyPagina(numPagina) == false) {
                        bufferManagerPrincipal.bufferPool.mostrarFramePagina(numFrame);
                        bufferManagerPrincipal.pageTable.aumentarPinCountDePagina(numPagina);
                        if (noEscrituraEnCola(numPagina) == true)
                            {
                                Requerimiento req = {numPagina, accionPagina};
                                colaDeRequerimientos.push(req);
                                break;
                            }
                            else
                            {
                                cout << "No se puede añadir a la cola este proceso de lectura, hay uno de escritura en cola" << endl;
                                break;
                            }
                    }
                    else {
                        cout << "Dirty Bit de la Pagina " << numPagina << " esta en 1." << endl;
                        cout << "No se puede procesar el requerimiento de Lectura hasta que Dirty Bit sea 0." << endl;
                        cout << "Si se quiere procesar el requerimiento de Lectura, debe guardar el archivo." << endl;
                        cout << "Guardar contenido? 0/NO GUARDAR  1/GUARDAR: ";
                        cin >> opcionPaginaGuardado;

                        if (opcionPaginaGuardado == 0) {
                            cout << "No se guardo el archivo " << endl;
                            //Requerimiento colaEspera = {numPagina, accionPagina};
                            //colaDeEspera.push(colaEspera);
                            //requerimientoAgregado = true;
                        }
                        else if (opcionPaginaGuardado == 1) {
                            bufferManagerPrincipal.pageTable.cambiarDirtyBitDePagina(numPagina);
                            bufferManagerPrincipal.bufferPool.agregarContenidoPaginaAbloque(numFrame, numPagina);
                            bufferManagerPrincipal.pageTable.aumentarPinCountDePagina(numPagina);
                            Requerimiento req = {numPagina, accionPagina};
                            colaDeRequerimientos.push(req);
                                
                        }
                    }
                    break;
                }
                else if (accionPagina == 'W' || accionPagina == 'w') {
                    bufferManagerPrincipal.bufferPool.lecturaOescrituraPagina(numFrame);
                    bufferManagerPrincipal.pageTable.aumentarPinCountDePagina(numPagina);
                    
                    if (noEscrituraEnCola(numPagina) == true)
                    {
                        bufferManagerPrincipal.pageTable.cambiarDirtyBitDePagina(numPagina);
                        Requerimiento req = {numPagina, accionPagina};
                        colaDeRequerimientos.push(req);
                        break;
                    }
                    else
                    {
                        cout << "No se puede añadir a la cola este proceso de escritura, ya existe uno en espera" << endl;
                        break;
                    }
                    break;
                }

                /*
                if (!requerimientoAgregado) {
                    Requerimiento req = {numPagina, accionPagina};
                    colaDeRequerimientos.push(req);
                    cout << "Requerimiento agregado a la cola." << endl;
                }
                */

                //Requerimiento req = {numPagina, accionPagina};
                //colaDeRequerimientos.push(req);
                //cout << "Requerimiento agregado a la cola." << endl;
                break;
            }
            case 3: {
                bufferManagerPrincipal.mostrarPageTAble();
                cout << endl;
                break;
            }
            case 4: {
                cout << "Ingrese el numero de la pagina a liberar (descontar PinCount)" << endl;
                cin >> numPagina;

                if (bufferManagerPrincipal.pageTable.verificarDirtyPagina(numPagina) == true) {
                    cout << "Dirty = 1 " << endl;
                    cout << "Para continuar, desea guardar el contenido? 0/NO GUARDAR  1/GUARDAR" << endl;
                    int opcionPaginaGuardado;
                    cin >> opcionPaginaGuardado;

                    if (opcionPaginaGuardado == 0) {
                        cout << "Página no guardada, no se descuenta pin count." << endl;
                    } else if (opcionPaginaGuardado == 1) {
                        bufferManagerPrincipal.pageTable.cambiarDirtyBitDePagina(numPagina);
                        bufferManagerPrincipal.bufferPool.agregarContenidoPaginaAbloque(numFrame, numPagina);
                        bufferManagerPrincipal.pageTable.descontarPinCountApagina(numPagina);
                        procesarColaEscritura(numPagina);
                    } else {
                        cout << "Opción inválida" << endl;
                    }
                } else if (bufferManagerPrincipal.pageTable.verificarDirtyPagina(numPagina) == false) {
                    cout << "Dirty = 0 " << endl;
                    bufferManagerPrincipal.pageTable.descontarPinCountApagina(numPagina);
                    procesarColaLectura(numPagina);
                }
                
                break;
            }
            case 5: {
                cout << "Ingrese Pagina a Despinnear " << endl;
                cin >> numPagina;
                bufferManagerPrincipal.pageTable.unpinnearPagina(numPagina);
                break;
            }
            case 6: {
                visualizarCola();
                break;
            }
            case 7: {
                cout << "Ingrese pagina a visualizar " << endl;
                cin >> numPagina;
                bufferManagerPrincipal.mostrarUnaPagina(numPagina);
                break;
            }
            case 8: {
                cout << "Saliendo del programa..." << endl;
                break;
            }
            default:
                cout << "Opción inválida, por favor intente de nuevo." << endl;
                break;
        }
    }
}

#include <fstream>

void insertionNode(BPlusTree** Btree) {
    int rolNo;
    string data;

    cout << "Ingrese el rolNo: ";
    cin >> rolNo;
    cin.ignore(); 

    cout << "Ingrese nueva data: ";
    getline(cin, data);

    string fileName = "../Archivos/" + to_string(rolNo) + ".txt";

    ifstream infile(fileName);
    if (infile.good()) {
        infile.close();
        FILE* filePtr = fopen(fileName.c_str(), "a");
        if (filePtr == NULL) {
            perror("Error al abrir el archivo");
            return;
        }
        (*Btree)->insert(rolNo, filePtr, data);
    } else {
        FILE* filePtr = fopen(fileName.c_str(), "w");
        if (filePtr == NULL) {
            perror("Error al crear el archivo");
            return;
        }
        string userTuple = to_string(rolNo) + " " + data + "\n";
        fprintf(filePtr, "%s", userTuple.c_str());
        fclose(filePtr);
        (*Btree)->insert(rolNo, filePtr, data);
        cout << "Inserción del rol: " << rolNo << " exitosa." << endl;
    }
}

void printing(BPlusTree* BTree) {
    int option;
    cout << "1. Display Secuencial." << endl;
    cout << "2. Display Iterativo." << endl;
    cin >> option;

    if (option == 1) {
        BTree->seqDisplay(BTree->getRoot());
    }
    else if (option == 2) {
        BTree->display(BTree->getRoot());
    }
}

void deletion(BPlusTree* BTree) {
    cout << "Showing you the Tree, Choose a key from here: " << endl;
    BTree->display(BTree->getRoot());
 
    int tmp;
    cout << "Enter a key to delete: " << endl;
    cin >> tmp;
    BTree->removeKey(tmp);

    //Displaying
    BTree->display(BTree->getRoot());
}

void searching(BPlusTree* Btree, BufferManager &bufferManagerPrincipal) {
    int rolNo;
    cout << "Rol a buscar? ";
    cin >> rolNo;
    string data = Btree->search(rolNo); 

    if (!data.empty()) {
        size_t pos = data.find(' ');
        if (pos != string::npos) {   
            data = data.substr(pos + 1);
        }
        cout << "Data obtenida: " << data << endl;
        bufferManagerPrincipal.bufferPool.verificarExistenciaRegistro(data);
    }
}

string ajustarCampo(const string& campo, int longitud) {
    if (campo.length() < longitud) {
        return campo + string(longitud - campo.length(), ' ');
    }
    else if (campo.length() > longitud) {
        return campo.substr(0, longitud);
    }
    return campo;
}

void ajustarRegistros(const string& fileRegistrosNormal, const string& esquema) {
    string fileLonFija;
    cout << "Archivo para Registros de Longitud Fija: ";
    cin >> fileLonFija;
    string pathLonfija = "../Archivos/" + fileLonFija;
    ifstream fileRegistros(fileRegistrosNormal);
    ifstream fileEsquema(esquema);
    ofstream fileLongitudFija(pathLonfija);

    vector<string> columnasEsquema;
    vector<int> longitudes;

    if (fileEsquema.is_open()) {
        string line;
        getline(fileEsquema, line);
        while (getline(fileEsquema, line)) {
            istringstream ss(line);
            string nombreColumna, tipoDato, longitud;
            ss >> nombreColumna >> tipoDato >> longitud;
            columnasEsquema.push_back(nombreColumna);
            longitudes.push_back(stoi(longitud));
        }
        fileEsquema.close();
    }
    else {
        cerr << "No se pudo abrir el archivo del Esquema." << endl;
        return;
    }

    if (fileRegistros.is_open()) {
        string line;
        while (getline(fileRegistros, line)) {
            istringstream ss(line);
            string campo;
            bool firstField = true;
            int colIndex = 0;

            while (getline(ss, campo, ',')) {
                if (campo.front() == '"' && campo.back() != '"') {
                    string campoCadena = campo;
                    while (getline(ss, campo, ',') && campo.back() != '"') {
                        campoCadena += "," + campo;
                    }
                    campoCadena += "," + campo;
                    campo = campoCadena;
                }
                
                campo = ajustarCampo(campo, longitudes[colIndex]);
                if (!firstField) {
                    fileLongitudFija << ",";
                }
                fileLongitudFija << campo;
                firstField = false;
                colIndex++;
            }
            fileLongitudFija << endl;
        }
        fileRegistros.close();
        fileLongitudFija.close();
    }
    else {
        cerr << "Error en el archivo de Registros." << endl;
    }
}

void esquema() {
    string fileCSV;
    string pathCSV = "../Archivos/";
    cout << "File csv para convertir: ";
    cin >> fileCSV;
    string path = pathCSV + fileCSV;
    ifstream file(path);
    
    if (file.is_open()) {
        string csvTXT;
        string esquemaTXT;
        cout << "Nombre del Archivo para registros convertidos a TXT: ";
        cin >> csvTXT;
        cout << "Nombre del Archivo para creacion de esquema en TXT: ";
        cin >> esquemaTXT;

        string pathRegistros = pathCSV + csvTXT;
        string pathEsquema = pathCSV + esquemaTXT;
        ofstream fileRegistros(pathRegistros, ios::app);
        ofstream fileEsquema(pathEsquema, ios::app);
        string line;
        vector<string> columnas;

        if (getline(file, line)) {
            istringstream ss(line);
            string campo;

            while (getline(ss, campo, ',')) {
                columnas.push_back(campo);
            }

            string nameEsquema;
            cout << "Nombre del Esquema: ";
            cin >> nameEsquema;
            fileEsquema << nameEsquema << endl;

            for (const string& nombreColumna : columnas) {
                string tipoDato;
                string tamanio = "4";
                bool tipoValido = false;

                while (!tipoValido) {
                    cout << "Ingrese el tipo de dato para la Columna: " << nombreColumna << " (int, float, str): ";
                    cin >> tipoDato;

                    if (tipoDato == "int" || tipoDato == "float" || tipoDato == "str") {
                        tipoValido = true;
                        if (tipoDato == "str") {
                            cout << "Ingrese el espacio para la columna " << nombreColumna << ": ";
                            cin >> tamanio;
                        }
                    }
                    else {
                        cout << "Tipo de Dato incorrecto." << endl;
                    }
                }
                fileEsquema << left << setw(20) << nombreColumna << setw(10) << tipoDato << setw(5) << tamanio << endl;
            }

            fileRegistros << line << endl;
        }
        
        while (getline(file, line)) {
            istringstream ss(line);
            string campo;
            bool firstLine = true;

            while (getline(ss, campo, ',')) {
                if (campo.front() == '"' && campo.back() != '"') {
                    string campoCadena = campo;
                    while (getline(ss, campo, ',') && campo.back() != '"') {
                        campoCadena += ',' + campo;
                    }
                    campoCadena += ',' + campo;
                    campo = campoCadena;
                }

                if (!firstLine) {
                    fileRegistros << ",";
                }
                fileRegistros << campo;
                firstLine = false;
            }
            fileRegistros << endl;
        }
        fileRegistros.close();
        fileEsquema.close();
        file.close();
        cout << "Esquema y Registros creados." << endl;
    }
    else {
        cout << "Error en el archivo CSV." << endl;
    }
}

void menuArbol(BPlusTree* BTree,BufferManager &bufferManagerPrincipal) {
    bool flag = true;
    int option;

    do {
        cout << "------------------MENU DEL ARBOL---------------------" << endl;
        cout << "Arbol B+ como SGBD basico." << endl;
        cout << "1. Insercion." << endl;
        cout << "2. Busqueda." << endl;
        cout << "3. Imprimir." << endl;
        cout << "4. Eliminar." << endl;
        cout << "5. Generar .dot." << endl;
        cout << "6. Salir." << endl;
        cout << "Opcion? ";
        cin >> option;

        switch (option) {
            case 1:
                insertionNode(&BTree);
                break;
            case 2:
                searching(BTree,bufferManagerPrincipal);
                break;
            case 3:
                printing(BTree);
                break;
            case 4:
                deletion(BTree);
                break;
            case 5:
                BTree->generateDOTFile("BPlusTree.dot");
                break;
            case 6:
                cout << "Saliendo del menú del árbol..." << endl;
                flag = false;
                break;
            default:
                cout << "Opcion incorrecta, elija de nuevo." << endl;
                break;
        }
    } while (flag);
}

void menuDisco(Disco &disco, BPlusTree* Btree) {
    bool creacionDisco = false;
    string fileTXT, fileEsquema;
    string recordsFixed;
    string schema;
    int capacidadBloques;
    bool flag = true;
    int option;

    do {
        cout << "------------------MENU DEL DISCO---------------------" << endl;
        cout << "1. Generar esquema y conversion de CSV a archivo de texto." << endl;
        cout << "2. Convertir los registros a longitud fija." << endl;
        cout << "3. Creacion del disco con los bloques y llenado de Sectores." << endl;
        cout << "4. Mostrar capacidad del disco." << endl;
        cout << "5. Eliminacion del disco." << endl;
        cout << "6. Salir." << endl;
        cout << "Opcion? ";
        cin >> option;

        switch (option) {
            case 1: {
                esquema();
                break;
            }
            case 2: {
                cout << "Archivo de Registros en formato de TXT: ";
                cin >> fileTXT;
                cout << "Archivo de Esquema en formato de TXT: ";
                cin >> fileEsquema;
                ajustarRegistros("../Archivos/" + fileTXT, "../Archivos/" + fileEsquema);
                break;
            }
            case 3: {
                if (!creacionDisco) {
                    int platos;
                    int pistas;
                    int sectores;
                    int capacidadSectores;

                    cout << "\t\t   Creacion de Disco\t\t" << endl;
                    cout << "-----------------------------------------------------" << endl;
                    cout << "Platos: ";
                    cin >> platos;
                    cout << "Pistas: ";
                    cin >> pistas;
                    cout << "Sectores: ";
                    cin >> sectores;
                    cout << "Capacidad de Sectores: ";
                    cin >> capacidadSectores;

                    disco.setNumPlatos(platos);
                    disco.setPistasPorSuperficie(pistas);
                    disco.setSectoresPorPista(sectores);
                    disco.setCapacidadPorSector(capacidadSectores);

                    disco.crearDisco();

                    cout << "Capacidad de los Bloques: ";
                    cin >> capacidadBloques;

                    cout << "Archivo con Registros de Longitud Fija en TXT: ";
                    cin >> recordsFixed;
                    cout << "Archivo con el Esquema en TXT: ";
                    cin >> schema;

                    disco.llenarRegistrosSector("../Archivos/" + recordsFixed, "../Archivos/" + schema, Btree);
                    disco.crearBloques(capacidadBloques, capacidadSectores, sectores);
                    disco.calcularEspacioLibreEnTodosLosBloques(capacidadBloques);
                }
                else {
                    cout << "Ya existe un Disco existente." << endl;
                }
                break;
            }
            case 4: {
                cout << "-----------------------------------------------------" << endl;
                cout << "\t\t   Capacidad de Disco\t\t" << endl;
                disco.capacidadDisco();
                cout << "-----------------------------------------------------" << endl;
                break;
            }
            case 5: {
                disco.eliminarDisco();
                cout << "Disco eliminado correctamente." << endl;
                break;
            }
            case 6:
                cout << "Saliendo del menú del árbol..." << endl;
                flag = false;
                break;
            default:
                cout << "Opcion incorrecta, elija de nuevo." << endl;
                break;
        }
    } while (flag);
}

int main() {
    Disco disco;
    Database database;
    int opcion;
    BufferManager bufferManagerPrincipal;
    int maxChildInt;
    int maxNodeLeaf;

    cout << "--------------------Generacion del Arbol--------------------" << endl;
    cout << "Limite maximo de Nodos Internos Hijos que pueden haber: ";
    cin >> maxChildInt;
    cout << endl;
    cout << "Limite maximo de claves que pueden haber en un Nodo Hijo: ";
    cin >> maxNodeLeaf;
    cout << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;

    BPlusTree* Btree = new BPlusTree(maxChildInt, maxNodeLeaf);
    menuBufferManager(bufferManagerPrincipal);
    do {
        cout << "-----------------------------------------------------" << endl;
        cout << "\t\t   Menu del Gestor\t\t" << endl;
        cout << "-----------------------------------------------------" << endl;
        cout << "1. Ejecutar programa del Megatron." << endl;
        cout << "2. Ejecutar programa del Disco." << endl;
        cout << "3. Ejecutar programa del Arbol." << endl;
        cout << "4. Ejecutar programa del Buffer." << endl;
        cout << "5. Salir del programa del Gestor." << endl;
        cout << "Ingrese su opcion a ejecutar: " << endl;
        cin >> opcion;
        cout << "-----------------------------------------------------" << endl;
        switch (opcion) {
            case 1: {
                system("../megatron2/MEGATRON"); 
                break;
            }
            case 2: {
                menuDisco(disco, Btree);
                break;
            }
            case 3: {
                menuArbol(Btree, bufferManagerPrincipal);
                break;
            }
            case 4: {
                menuBuffer(bufferManagerPrincipal);
                break;
            }
            case 5: {
                cout << "Saliendo del programa" << endl;
                break;
            }
            default: {
                cout << "Opcion incorrecta, elija de nuevo." << endl;
            }
        }
    } while (opcion != 5);
    
    return 0;
}
