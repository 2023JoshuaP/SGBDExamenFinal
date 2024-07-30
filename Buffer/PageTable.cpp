#include "./PageTable.h"

PageTable::PageTable() {}

PageTable::~PageTable() {}

int clockHand = 0;

void PageTable::mostrarPageTableLRU() {
    int numFilas = this->matrizPageTableLRU.size();
    int numColumas = this->numColumnasEnPageTable;
    cout << "Frame id \t PageId \t DirtyBit \t Pin Count \t Ref Bit \t Pinned" << endl;
    for (int i = 0; i < numFilas; i++) {
        cout << i << "\t\t";
        for (int j = 0; j < numColumas; j++) {
            cout << this->matrizPageTableLRU[i][j] << "\t\t";
        }
        cout << endl;
    }
}

bool PageTable::verificarDirtyPagina(int numPagina)
{
    int numFilaElegida;
    if (this->verificarExistenciaDePagina(numPagina) == true) {
        int j = 0;
        for (int i = 0; i < this->columnaFrameIdSize; i++) {
            if (this->matrizPageTableLRU[i][j] == numPagina) {
                numFilaElegida = i;
                break; 
            }
        }
        int numColumnaDirty = 1;

        if (this->matrizPageTableLRU[numFilaElegida][numColumnaDirty] == 1) {
            return true;
        }
        else {
            return false;
        }
    }
}

bool PageTable::verificarExistenciaDePagina(int numPagina) {
    int j = 0;
    for (int i = 0; i < this->columnaFrameIdSize; i++) {
        if (this->matrizPageTableLRU[i][j] == numPagina) {
            cout << "Pagina " + to_string(numPagina) + " si existe." << endl;
            return true;
        }
    }
    cout << "Pagina " + to_string(numPagina) + " no existe" << endl;
    return false;
}

int PageTable::getNumFrameDeUnaPagina(int numPagina) {
    if (this->verificarExistenciaDePagina(numPagina) == true) {
        int j = 0;
        for (int i = 0; i < this->columnaFrameIdSize; i++) {
            if (this->matrizPageTableLRU[i][j] == numPagina) {
                cout << "Pagina hallada, devolviendo el Frame Id: " << endl;
                return i;
            }
        }
    }
    else {
        cout << "No existe la Página para obtener el Frame." << endl;
        int j = 0;
        for (int i = 0; i < this->columnaFrameIdSize; i++) {
            if (this->matrizPageTableLRU[i][j]) {
                cout << this->matrizPageTableLRU[i][j] << "Pagina si tiene valor." << endl;
            }
            else {
                cout << "Espacio vacio, Page Id confirmado." << endl;
                return i;
            }
        }
    }
}

void PageTable::descontarPinCountApagina(int numPagina) {
    int numFilaElegida;
    if (this->verificarExistenciaDePagina(numPagina) == true) {
        int j = 0;
        for (int i = 0; i < this->columnaFrameIdSize; i++) {
            if (this->matrizPageTableLRU[i][j] == numPagina) {
                numFilaElegida = i;
            }
        }
        int numColumnaPinCount = 2;
        this->matrizPageTableLRU[numFilaElegida][numColumnaPinCount] = this->matrizPageTableLRU[numFilaElegida][numColumnaPinCount] - 1;
        cout<<"Descuento de Pin Count de la Pagina " << numPagina << endl;
    }
}

void PageTable::aumentarPinCountDePagina(int numPagina) {
    int numFilaElegida;
    if (this->verificarExistenciaDePagina(numPagina) == true) {
        int j = 0;
        for (int i = 0; i < this->columnaFrameIdSize; i++) {
            if (this->matrizPageTableLRU[i][j] == numPagina) {
                numFilaElegida = i;
            }
        }
        int numColumnaPinCount = 2;
        this->matrizPageTableLRU[numFilaElegida][numColumnaPinCount] = this->matrizPageTableLRU[numFilaElegida][numColumnaPinCount] + 1;
        cout << "Aumento de Pin Count de la Pagina " << numPagina << endl;
    }
}

void PageTable::cambiarDirtyBitDePagina(int numPagina) {
    int numFilaElegida;
    if (this->verificarExistenciaDePagina(numPagina) == true) {
        int j = 0;
        for (int i = 0; i < this->columnaFrameIdSize; i++) {
            if (this->matrizPageTableLRU[i][j] == numPagina) {
                numFilaElegida = i;
            }
        }
        int numColumnaDirtyBit = 1;
        if (this->matrizPageTableLRU[numFilaElegida][numColumnaDirtyBit] == 1) {
            cout << "DirtyBit 1 de pagina numero: " << numPagina << endl;
            this->matrizPageTableLRU[numFilaElegida][numColumnaDirtyBit] = 0;
            cout << "DirtyBit cambiado a : " << this->matrizPageTableLRU[numFilaElegida][numColumnaDirtyBit] << endl;
        }
        else {
            cout << "DirtyBit 0 de pagina numero: " << numPagina << endl;
            this->matrizPageTableLRU[numFilaElegida][numColumnaDirtyBit] = 1;
            cout << "DirtyBit cambiado a : " << this->matrizPageTableLRU[numFilaElegida][numColumnaDirtyBit] << endl;
        }
    }
}

/*
void PageTable::renovarLastUsedDePagina(int numPagina) {
    int numFilaElegida;
    if (this->verificarExistenciaDePagina(numPagina) == true) {
        int j = 0;
        for (int i = 0; i < this->columnaFrameIdSize; i++) {
            if (this->matrizPageTableLRU[i][j] == numPagina) {
                numFilaElegida = i;
            }
        }
        int numColumnaLastUsed = 3;
        this->matrizPageTableLRU[numFilaElegida][numColumnaLastUsed] = 0;
    }
}
*/

/*
void PageTable::aumentarLastUsedDePagina(int numPagina) {
    int numFilaElegida;
    if (this->verificarExistenciaDePagina(numPagina) == true) {
        int j = 0;
        for (int i = 0; i < this->columnaFrameIdSize; i++) {
            if (this->matrizPageTableLRU[i][j] == numPagina) {
                numFilaElegida = i;
            }
        }
        int numColumnaLastUsed = 3;
        this->matrizPageTableLRU[numFilaElegida][numColumnaLastUsed] = this->matrizPageTableLRU[numFilaElegida][numColumnaLastUsed] + 1;
    }
}


*/
void PageTable::aumentarRefBitDePagina(int numPagina) {
    int numFilaElegida;
    if (this->verificarExistenciaDePagina(numPagina)) 
    {
        int j = 0;
        for (int i = 0; i < this->columnaFrameIdSize; i++) {
            if (this->matrizPageTableLRU[i][j] == numPagina) {
                numFilaElegida = i;
                break; 
            }
        }
        //int numColumnaLastUsed = 3;
        if (this->matrizPageTableLRU[numFilaElegida][3] < 1) {
            this->matrizPageTableLRU[numFilaElegida][3]++;
        }
    }
}

/*
void PageTable::aumentarLastUsedDeTodasLasDemasPaginas(int numFrameAignorar) {
    cout << "Actualizando la Page Table." << endl;
    int numIdPaginaAux;
    for (int i = 0; i < this->columnaFrameIdSize; i++) {
        for (int j = 0; j < this->numColumnasEnPageTable; j++) {
            if (i == numFrameAignorar) {
                continue;
            }
            else {
                if (j == 3) {
                    numIdPaginaAux = this->matrizPageTableLRU[i][0];
                    //this->aumentarLastUsedDePagina(numIdPaginaAux);
                    cout << "Frame Id: " << i << endl;
                    cout << "Aumento dado en Last Used." << endl;
                    cout << "Agregando +1 a Last Used de todas las demas Paginas." << endl;
                }
            }
        }
    }
    cout << "Aumento de Last Used en las Paginas." << endl;
}
*/

void PageTable::actualizarInfoDePageTableSolictandoNuevaPagina(int numPaginaActualizar, int numFilaFrameId) {
    cout << "Actualizando la Page Table" << endl;
    for (int j = 0; j < this->numColumnasEnPageTable; j++) {
        if (j == 0) {
            this->matrizPageTableLRU[numFilaFrameId][j] = numPaginaActualizar;
            cout << "Frame Id: " << numFilaFrameId << endl;
            cout << "Page Id establecida: " << numPaginaActualizar << endl;
        }
        else if (j == 1) {
            if (this->matrizPageTableLRU[numFilaFrameId][j] == 1) {
                cout << "Frame Id: " << numFilaFrameId <<endl;
                cout << "No se cambió el Dirty Bit = 1." << endl;
            }
            else {
                this->matrizPageTableLRU[numFilaFrameId][j] = 0;
                cout << "Frame Id: " << numFilaFrameId << endl;
                cout << "Dirty Bit = 0." << endl;
            }
        }
        else if (j == 2) {
            //this->aumentarPinCountDePagina(numPaginaActualizar);
            cout << "Frame Id: " << numFilaFrameId << endl;
            cout << "Pin Count establecido." << endl;
        }
        else if (j == 3) {
            //this->renovarLastUsedDePagina(numPaginaActualizar);
            cout << "Frame Id: " << numFilaFrameId <<endl;
            //cout << "Aumento dado en LastUsed." << endl;
            //cout << "Agregando +1 a Last Used de todos las demás Paginas" << endl;
            //int numFrameAignorar = this->getNumFrameDeUnaPagina(numPaginaActualizar);
            //this->aumentarLastUsedDeTodasLasDemasPaginas(numFrameAignorar);
            //this->aumentarLastUsedDeTodasLasDemasPaginas(numFrameAignorar);

            this->aumentarRefBitDePagina(numPaginaActualizar);
        }
    }
    cout << "Datos de Page Table actualizado" << endl;
}

void PageTable::aplicarCLOCK(int numPagina, int numFrameAignorar, bool &eliminarPageSinEscrituraEnDisco, bool &eliminarPageConEscrituraEnDisco, int &numPaginaEliminada) {
    cout << "------------------Aplicando Algoritmo CLOCK-----------------" << endl;
    cout << " La manesilla apunta actualmente al frame " << clockHand << endl;

    int numColumnaRefBit = 3; 
    int numColumnaDirtyBit = 1;
    int numColPinned = 4;

    while (true) {
        while (this->matrizPageTableLRU[clockHand][numColPinned] == 1) {
            cout << "Pagina pinned del frame " << clockHand << " saltada" << endl;
            clockHand = (clockHand + 1) % this->columnaFrameIdSize;
        }

        if (this->matrizPageTableLRU[clockHand][numColumnaRefBit] == 0) {
            numPaginaEliminada = this->matrizPageTableLRU[clockHand][0];
            cout << "Página encontrada para reemplazo: " << numPaginaEliminada << endl;

            if (this->matrizPageTableLRU[clockHand][numColumnaDirtyBit] == 0) {
                eliminarPageSinEscrituraEnDisco = true;
                eliminarPageConEscrituraEnDisco = false;
            } else {
                eliminarPageSinEscrituraEnDisco = false;
                eliminarPageConEscrituraEnDisco = true;
            }

            this->matrizPageTableLRU[clockHand][0] = numPagina;
            this->matrizPageTableLRU[clockHand][numColumnaDirtyBit] = 0;
            this->matrizPageTableLRU[clockHand][numColumnaRefBit] = 1;
            this->matrizPageTableLRU[clockHand][2] = 1;

            clockHand = (clockHand + 1) % this->columnaFrameIdSize;
            break;
        } else {
            this->matrizPageTableLRU[clockHand][numColumnaRefBit] = 0;
            clockHand = (clockHand + 1) % this->columnaFrameIdSize;
        }
    }

    cout << " (ACTUALIZACION) La manesilla apunta ahora al frame " << clockHand << endl;
}

/*
void PageTable::aplicarCLOCK(int numPagina, int numFrameAignorar, bool &eliminarPageSinEscrituraEnDisco, bool &eliminarPageConEscrituraEnDisco, int &numPaginaEliminada) {
    cout << "------------------Aplicando Algoritmo CLOCK-----------------" << endl;
    cout << " La manecilla apunta actualmente al frame " << clockHand << endl;
    
    int numColumnaRefBit = 3; 
    int numColumnaDirtyBit = 1;

    while (true) {
        // Si la manecilla apunta al frame a ignorar, simplemente avanza la manecilla
        //if (clockHand == numFrameAignorar) {
          //  clockHand = (clockHand + 1) % this->columnaFrameIdSize;
            //continue;
        //}

        // Verificar el bit de referencia
        if (this->matrizPageTableLRU[clockHand][numColumnaRefBit] == 0) {
            // Encontramos un frame con ref bit 0
            numPaginaEliminada = this->matrizPageTableLRU[clockHand][0];
            cout << "Página encontrada para reemplazo: " << numPaginaEliminada << endl;

            // Verificar el dirty bit
            if (this->matrizPageTableLRU[clockHand][numColumnaDirtyBit] == 0) {
                eliminarPageSinEscrituraEnDisco = true;
                eliminarPageConEscrituraEnDisco = false;
            } else {
                eliminarPageSinEscrituraEnDisco = false;
                eliminarPageConEscrituraEnDisco = true;
            }

            // Reemplazar la página
            this->matrizPageTableLRU[clockHand][0] = numPagina;
            this->matrizPageTableLRU[clockHand][numColumnaDirtyBit] = 0; // Resetear el dirty bit
            this->matrizPageTableLRU[clockHand][numColumnaRefBit] = 1; // Establecer el ref bit a 1
            this->matrizPageTableLRU[clockHand][2] = 1; // Pin count a 1

            // Mover la manecilla
            clockHand = (clockHand + 1) % this->columnaFrameIdSize;
            break;
        } else if (this->matrizPageTableLRU[clockHand][numColumnaRefBit] == 1){
            // Ref bit es 1, resetearlo y avanzar la manecilla
            this->matrizPageTableLRU[clockHand][numColumnaRefBit] = 0;
            clockHand = (clockHand + 1) % this->columnaFrameIdSize;
            aplicarCLOCK(numPagina,numFrameAignorar, eliminarPageSinEscrituraEnDisco, eliminarPageConEscrituraEnDisco, numPaginaEliminada);
        }
    }
    cout << " (ACTUALIZACION) La manecilla apunta ahora al frame " << clockHand << endl;
}
*/

void PageTable::aplicarClockConCondicionPinned(int numPagina, int numFrameAignorar, bool &eliminarPageSinEscrituraEnDisco, bool &eliminarPageConEscrituraEnDisco, int &numPaginaEliminada)
{
    int numColRefBit = 3;
    int numColPinned = 4;
    int numColDirtyBit = 1;
    int numColPageId = 0;

    while (this->matrizPageTableLRU[clockHand][numColPinned] == 1) {
        cout << "Pagina pinned del frame " << clockHand << " saltada" << endl;
        clockHand = (clockHand + 1) % this->columnaFrameIdSize;
    }

    if (this->matrizPageTableLRU[clockHand][numColPinned] == 0) {
        cout << "Pagina unpinned" << endl;
        cout << "Antes de eliminar, revisaremos el Dirty Bit" << endl;
        if (this->matrizPageTableLRU[clockHand][numColDirtyBit] == 0) {
            cout << "Dirty Bit = 0" << endl;
            cout << "Solo se eliminará la Pagina, sin escribir en Disco." << endl;
            aplicarCLOCK(numPagina, INT16_MAX, eliminarPageSinEscrituraEnDisco, eliminarPageConEscrituraEnDisco, numPaginaEliminada);
            eliminarPageSinEscrituraEnDisco = true;
            eliminarPageConEscrituraEnDisco = false;
        } else {
            cout << "Dirty Bit = 1" << endl;
            cout << "Se eliminará la Pagina, con escritura en Disco." << endl;
            actualizarInformacionDePaginaEliminada(clockHand, numPagina);
            aplicarCLOCK(numPagina, INT16_MAX, eliminarPageSinEscrituraEnDisco, eliminarPageConEscrituraEnDisco, numPaginaEliminada);
            numPaginaEliminada = clockHand;
            eliminarPageConEscrituraEnDisco = true;
            eliminarPageSinEscrituraEnDisco = false;
        }
    }
}

void PageTable::pinnearPagina(int numPagina) {
    cout << "-------------pinnearPagina--------------" << endl;
    int numFilaElegida;
    if (this->verificarExistenciaDePagina(numPagina)) 
    {
        int j = 0;
        for (int i = 0; i < this->columnaPinnedSize; i++) {
            if (this->matrizPageTableLRU[i][j] == numPagina) {
                numFilaElegida = i;
                break; 
            }
        }
        //int numColumnaLastUsed = 3;
        if (this->matrizPageTableLRU[numFilaElegida][4] == 0) {
            this->matrizPageTableLRU[numFilaElegida][4] = 1;
        }
        else {
            cout << "Pagina ya esta pinneada" << endl;
        }
    }
    else
    {
        cout << "Pagina no existe" << endl;
    }
}

void PageTable::unpinnearPagina(int numPagina) {
    if (this->verificarExistenciaDePagina(numPagina) == true) {
        int numFilaElegida = -1;
        int j = 0;
        for (int i = 0; i < this->columnaFrameIdSize; i++) {
            cout << i << endl;
            if (this->matrizPageTableLRU[i][j] == numPagina) {
                numFilaElegida = i;
                break; 
            }
        }
        if (numFilaElegida != -1) {  
            int numColumnaPinned = 4;
            this->matrizPageTableLRU[numFilaElegida][numColumnaPinned] -= 1;
            cout << "Unpinned de la Pagina " << numPagina << endl;
        } else {
            cout << "Pagina " << numPagina << " no encontrada." << endl;
        }
    } else {
        cout << "Pagina " << numPagina << " no existe." << endl;
    }
}

/*
void PageTable::aplicarLRU(int numPagina, int numFrameAignorar, bool &eliminarPageSinEscrituraEnDisco, bool &eliminarPageConEscrituraEnDisco, int &numPaginaEliminada)
{
    cout << "--------------------- Estrategia LRU ------------------" << endl;
    int numColLastUsed = 3;
    int numColPinCount = 2;
    int numColDirtyBit = 1;
    int numColPageId = 0;

    int acumuladorMayorLastUsed = 0;
    int numFrameDelMayorLastUsed = 0;
    int numPagDelMayorLastUsed = 0;
    for (int i = 0; i < this->columnaFrameIdSize; i++) {
        if (i == numFrameAignorar) {
            continue;
        }
        else {
            if(acumuladorMayorLastUsed < this->matrizPageTableLRU[i][numColLastUsed]) {
                acumuladorMayorLastUsed = this->matrizPageTableLRU[i][numColLastUsed];
                numFrameDelMayorLastUsed = i;
                numPagDelMayorLastUsed = this->matrizPageTableLRU[i][numColPageId];
            }
        }         
    }

    cout << "Revisando Pin Count" << endl;
    if (this->matrizPageTableLRU[numFrameDelMayorLastUsed][numColPinCount] == 0) {
        cout << "Pin Count esta libre" << endl;
        cout << "Antes de eliminar, revisaremos el Dirty Bit" << endl;
        if (this->matrizPageTableLRU[numFrameDelMayorLastUsed][numColDirtyBit] == 0) {
            cout << "Dirty Bit = 0" << endl;
            cout << "Solo se eliminará la Pagina, sin escribir en Disco." << endl;
            actualizarInformacionDePaginaEliminada(numPagDelMayorLastUsed, numPagina);
            eliminarPageSinEscrituraEnDisco = true;
            eliminarPageConEscrituraEnDisco = false;
        }
        else {
            cout << "Dirty Bit = 1" << endl;
            cout << "Se eliminará la Pagina, con escritura en Disco." << endl;
            actualizarInformacionDePaginaEliminada(numPagDelMayorLastUsed, numPagina);
            numPaginaEliminada=numPagDelMayorLastUsed;
            eliminarPageConEscrituraEnDisco = true;
            eliminarPageSinEscrituraEnDisco = false;
        }  
    }
    else {
        cout << "Pin Count no esta libre." << endl;
        aplicarLRU(numPagina, numFrameDelMayorLastUsed, eliminarPageSinEscrituraEnDisco, eliminarPageConEscrituraEnDisco, numPaginaEliminada);
    }
}
*/

string PageTable::analizarPageTableParaAgregarPagina(int numPagina) {
    if (this->verificarExistenciaDePagina(numPagina) == true) {
        int numColumnaRefBit = 3;
        int numPaginaEliminada = INT16_MAX;
        bool eliminarPageSinEscrituraEnDisco = true;
        bool eliminarPageConEscrituraEnDisco = true;
        cout << "Pagina ya encontrada " << endl;

        int numFrame = this->getNumFrameDeUnaPagina(numPagina);
        this->matrizPageTableLRU[numFrame][numColumnaRefBit] = 1;
        cout << "  La manesilla no se mueve, apunta al frame " << clockHand << endl;
        this->actualizarInfoDePageTableSolictandoNuevaPagina(numPagina, numFrame);
        string normal="normal";
        normal=normal + ", " + to_string(INT16_MAX);
        return normal;
    }
    else {
        bool eliminarPageSinEscrituraEnDisco = false;
        bool eliminarPageConEscrituraEnDisco = false;
        cout << "Pagina no se encuentra en la Page Table." << endl;
        
        if (this->verificarFrameLlenos() == true) {
            cout<<"Frames llenos"<<endl;
            int numPaginaEliminada = INT16_MAX;
            
            aplicarClockConCondicionPinned(numPagina, INT16_MAX, eliminarPageSinEscrituraEnDisco, eliminarPageConEscrituraEnDisco, numPaginaEliminada);
            
            if (eliminarPageSinEscrituraEnDisco == true && eliminarPageConEscrituraEnDisco == false) {
                string primerBool = "eliminarPageSinEscrituraEnDisco";
                primerBool = primerBool + ", "+ to_string(numPaginaEliminada);
                return primerBool;
            }
            else if(eliminarPageConEscrituraEnDisco==true && eliminarPageSinEscrituraEnDisco==false) {
                string segundoBool="eliminarPageConEscrituraEnDisco";
                segundoBool = segundoBool + ", " + to_string(numPaginaEliminada);
                return segundoBool;
            }
        }
        else {
            cout<<"Frames no estan llenos, hay espacio."<<endl;
            int numFrame = this->getNumFrameDeUnaPagina(numPagina);
            this->actualizarInfoDePageTableSolictandoNuevaPagina(numPagina, numFrame);
            string normal = "normal";
            normal = normal+ ", " + to_string(INT16_MAX);
            return normal;
        }
    }
}

bool PageTable::verificarFrameLlenos() {
    int acumulador = 0;
    int j = 0;
    for (int i = 0; i < this->columnaFrameIdSize; i++) {
        if (this->matrizPageTableLRU[i][j]) {
            continue;
        }
        else {
            acumulador++;
        }
    }
    if (acumulador != 0) {
        return false;
    }
    else {
        return true;
    }
}

void PageTable::actualizarInformacionDePaginaEliminada(int numPaginaEliminar, int nuevaPaginaIngresar) {
    cout << "-------------------actualizarInformacionDePaginaEliminada------------------" << endl;
    int numFilaElegida;
    if (this->verificarExistenciaDePagina(numPaginaEliminar) == true) {
        int j = 0;

        for (int i = 0; i < this->columnaFrameIdSize; i++) {
            if (this->matrizPageTableLRU[i][j] == numPaginaEliminar) {
                cout << "Pagina a eliminar: " << numPaginaEliminar << endl;
                numFilaElegida = i;
            }
        }

        for (int j = 0; j < this->numColumnasEnPageTable; j++) {
            this->matrizPageTableLRU[numFilaElegida][j] = 0;
        }
    }

    cout << "Agregando Datos de nuevo registro." << endl;
    cout << "Parte 2 de Actulizar Informacion." << endl;
    cout << "Actualizando la Page Table." << endl;
    for (int j = 0; j < this->numColumnasEnPageTable; j++) {
        if (j == 0) {
            this->matrizPageTableLRU[numFilaElegida][j] = nuevaPaginaIngresar;
            cout << "Frame Id: " << numFilaElegida << endl;
            cout << "Page Id establecida: " << nuevaPaginaIngresar << endl;
        }
        else if (j == 1) {
            this->matrizPageTableLRU[numFilaElegida][j] = 0;
            cout << "Frame Id: " << numFilaElegida <<endl;
            cout << "Dirty Bit = 0." << endl;
        }
        else if (j == 2) {
            //this->aumentarPinCountDePagina(nuevaPaginaIngresar);
            cout << "Frame Id: " << numFilaElegida << endl;
            cout << "Pin Count establecido: " << nuevaPaginaIngresar << endl;
        }
        else if (j == 3) {
            //this->renovarLastUsedDePagina(nuevaPaginaIngresar);
            cout << "Frame Id: " << numFilaElegida << endl;
            //cout << "Aumento dado en Last Used." << endl;
            //cout << "Agregando +1 a Last Used de todas las demás Paginas." << endl;
            //int numFilaElegida = this->getNumFrameDeUnaPagina(nuevaPaginaIngresar);
            //this->aumentarLastUsedDeTodasLasDemasPaginas(numFilaElegida);
            
            this -> aumentarRefBitDePagina(nuevaPaginaIngresar);
        }
    }
    cout << "Datos de Page Table actualizado" << endl;
}