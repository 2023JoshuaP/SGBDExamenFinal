#include "./BufferPool.h"

BufferPool::BufferPool() {}

BufferPool::~BufferPool() {};

void BufferPool::agregarNuevaPaginaBufferPool(int numFrame, int numPagina) {
    if (this->vectorFramesBufferPool[numFrame].paginaEnFrame.vectorRegistrosEnPagina.empty()) {
        this->vectorFramesBufferPool[numFrame].leerDatosEnPaginaDeFrame(numPagina);
    }
    else {
        this->vectorFramesBufferPool[numFrame].paginaEnFrame.vectorRegistrosEnPagina.clear();
        this->vectorFramesBufferPool[numFrame].leerDatosEnPaginaDeFrame(numPagina);
    }
}

void BufferPool::mostrarFramePagina(int numFrame) {
    this->vectorFramesBufferPool[numFrame].paginaEnFrame.mostrarContenidoDePagina();
}

void BufferPool::lecturaOescrituraPagina(int numFrame) {
    this->vectorFramesBufferPool[numFrame].paginaEnFrame.leerOescribirEnPagina();
}

void BufferPool::agregarContenidoPaginaAbloque(int numFrame, int numPaginaEliminada) {
    this->vectorFramesBufferPool[numFrame].paginaEnFrame.agregarContenido(numPaginaEliminada);
}
void BufferPool::buscarRegistro(string& registroFrame, int frameElegido) {
    this->vectorFramesBufferPool[frameElegido].paginaEnFrame.añadirRegistro(registroFrame);
}

void BufferPool::verificarExistenciaRegistro(string& registroFrame) {
    bool encontrado = false;
    for (int i = 0; i < vectorFramesBufferPool.size(); ++i) {
        if (vectorFramesBufferPool[i].paginaEnFrame.buscarRegistro(registroFrame)) {
            cout << "Registro encontrado en el frame: " << i << endl;
            encontrado = true;
            return;
        }
    }
    if (!encontrado) {
        int frameElegido;
        cout << "Registro no encontrado. Elige un frame para añadir el registro: ";
        cin >> frameElegido;

        if (frameElegido >= 0 && frameElegido < vectorFramesBufferPool.size()) {
            buscarRegistro(registroFrame, frameElegido);
            cout << "Registro añadido en el frame: " << frameElegido << endl;
        } else {
            cout << "Frame inválido." << endl;
        }
        
    }
}


