#include "./BufferManager.h"

BufferManager::BufferManager() {}

BufferManager::~BufferManager() {}

void BufferManager::crearBufferPoolSegunNumFrames(int numFrames) {
    this->bufferPool.vectorFramesBufferPool.resize(numFrames);

    this->pageTable.columnaFrameIdSize = numFrames;
    this->pageTable.columnPageIdSize = 0;
    this->pageTable.columnaDirtyIdSize = 0;
    this->pageTable.columnaPinCountSize = 0;
    //this->pageTable.columnaLastUsedSize = 0;
    this->pageTable.columnaRefBitSize = 0;
    this->pageTable.columnaPinnedSize = 0;

    cout << "Inicializando la Page Table." << endl;
    this->pageTable.matrizPageTableLRU.resize(numFrames);

    for (int i = 0; i < numFrames; i++) {
        for (int j = 0; j < this->pageTable.numColumnasEnPageTable; j++) {
            this->pageTable.matrizPageTableLRU[i].resize(this->pageTable.numColumnasEnPageTable);
        }
    }
}
void BufferManager::verificarDirtyPagina(int numPagina) {
    this->pageTable.verificarDirtyPagina(numPagina);
}

void BufferManager::establecerLimiteDeFrames(int pesoBytesBLoque) {
    this->bufferPool.capacidadDeCadaFrame = pesoBytesBLoque;
    for (size_t i = 0; i < this->bufferPool.vectorFramesBufferPool.size(); i++) {
        this->bufferPool.vectorFramesBufferPool[i].capacidadBytesDeFrame = pesoBytesBLoque;
    }
}

void BufferManager::obtenerUnaPagina(int numPagina) {
    string resultadosParaEliminacionYCambios = this->pageTable.analizarPageTableParaAgregarPagina(numPagina);
    cout << "Resultado" << resultadosParaEliminacionYCambios << endl;
    istringstream iss(resultadosParaEliminacionYCambios);
    string resultado;
    string paginaEliminada;

    getline(iss,resultado,',');
    getline(iss,paginaEliminada,',');

    if (resultado == "eliminarPageSinEscrituraEnDisco") {
        cout << ">>>>>>>> eliminarPageSinEscrituraEnDisco" << endl;
        //cout << "Datos cambiados segun LRU en Page Table." << endl;
        this->pageTable.mostrarPageTableLRU();

        cout << "Aplicando cambios en Buffer Pool segun Page Table." << endl;
        cout << "Mandando a agregar la nueva Pagina." << endl;
        int numFrameDePagina = this->pageTable.getNumFrameDeUnaPagina(numPagina);

        cout << "Ingresando Página al BufferPoll dentro vector de Frames(Sin escritura)...." << endl;
        this->bufferPool.agregarNuevaPaginaBufferPool(numFrameDePagina,numPagina);
        this->bufferPool.mostrarFramePagina(numFrameDePagina);
    }
    else if(resultado == "eliminarPageConEscrituraEnDisco") {
        cout << ">>>>>>>> eliminarPageConEscrituraEnDisco" << endl;
        cout << "Datos cambiados segun LRU en Page Table" << endl;
        this->pageTable.mostrarPageTableLRU();

        cout << "Aplicando cambios en Buffer Pool segun Page Table" << endl;
        cout << "Mandando a agregar la nueva Pagina" << endl;
        int numFrameDePagina = this->pageTable.getNumFrameDeUnaPagina(numPagina);

        int numFrameDePaginaEliminada = this->pageTable.getNumFrameDeUnaPagina(numPagina);
        int numPaginaEliminada=stoi(paginaEliminada);
        

        cout << "Ingresando Página al BufferPoll dentro vector de Frames(Con escritura)...." << endl;
        this->bufferPool.agregarContenidoPaginaAbloque(numFrameDePaginaEliminada, numPaginaEliminada);
        this->bufferPool.agregarNuevaPaginaBufferPool(numFrameDePagina,numPagina);
        this->bufferPool.mostrarFramePagina(numFrameDePagina);
    }
    else {
        cout << "No hay eliminacion." << endl;
        this->pageTable.mostrarPageTableLRU();

        cout << "Aplicando cambios en Buffer Pool segun Page Table." << endl;
        cout << "Mandando a agregar la nueva Pagina." << endl;

        int numFrameDePagina = this->pageTable.getNumFrameDeUnaPagina(numPagina);

        cout << "Ingresando Página al BufferPoll dentro vector de Frames...." << endl;
        this->bufferPool.agregarNuevaPaginaBufferPool(numFrameDePagina,numPagina);
        this->bufferPool.mostrarFramePagina(numFrameDePagina);
    }
    char opcionPinned;
    cout << "Desea pinnear pagina? (S/N) " << endl;
    cin >> opcionPinned;

    if (opcionPinned == 'S' || opcionPinned == 's')
    {
        this->pageTable.pinnearPagina(numPagina);
    }
    else if (opcionPinned == 'N' || opcionPinned == 'n')
    {
        cout << "No se pinneó ninguna pagina" << endl;
    }
}

void BufferManager::mostrarUnaPagina(int numPagina) {
    if (this->pageTable.verificarExistenciaDePagina(numPagina) == true) {
        cout << "Existencia de Pàgina verificada." << endl;
        cout << "Mostrando la Pagina." << endl;
        int numFrameDePagina = this->pageTable.getNumFrameDeUnaPagina(numPagina);
        this->bufferPool.vectorFramesBufferPool[numFrameDePagina].paginaEnFrame.mostrarContenidoDePagina();
    }
    else {
        cout << "No existe la Pagina." << endl;
        cout << "Procediendo a pedido de nueva pàgina..." << endl;
    }
}

void BufferManager::mostrarPageTAble() {
    cout << endl;
    this->pageTable.mostrarPageTableLRU();
}

void BufferManager::dejarDeUsarUnaPagina(int numPagina) {
    this->pageTable.descontarPinCountApagina(numPagina);
}

void BufferManager::liberarPinnedPagina(int numPagina) {
    this->pageTable.unpinnearPagina(numPagina);
}