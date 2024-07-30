#ifndef PESO_H
#define PESO_H
#include "data_manager.h"
#include "pesos.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

using namespace std; // hola#213123#hola2

int Peso::calcular_peso(vector<string> elementos) {
    int peso = 0;
    for (int i = 0; i < elementos.size(); i++) {
        peso += elementos[i].size();
    }
    return peso * 8; // 8 bits
}

int Peso::peso_sector(string sector_archivo) {
    ifstream archivo(sector_archivo);
    string linea;
    vector<string> elementos;
    while(getline(archivo, linea)) {
        vector<string> palabras = DataManager::split(linea, '#');
        for (int i = 0; i < palabras.size(); i++) {
            elementos.push_back(palabras[i]);
        }
    }
    return calcular_peso(elementos);
}

// El peso máximo de un sector será de 1000 bits
bool Peso::verificar_peso(string sector_archivo, string nuevo_registro) {
    vector<string> elementos = DataManager::split(nuevo_registro, '#');
    int tamanio_registro = calcular_peso(elementos);
    return peso_sector(sector_archivo) + tamanio_registro <= tamanio_sector;
}
#endif

void Peso::peso_disco(int cant_platos, int cant_superficies, int cant_pistas, int cant_sectores) {
    int plato;
    int superficie;
    int pista;
    int sector;
    int result = 0;
    int result2 = 0;
        for (plato = 1; plato <= cant_platos; plato++) {
            for (superficie = 1; superficie <= cant_superficies; superficie++) {
                for (pista = 1; pista <= cant_pistas; pista++) {
                    for (sector = 1; sector <= cant_sectores; sector++) {
                        string primer_sector = "../Disco/Disco/" + to_string(plato) + "/" + to_string(superficie) + "/" + to_string(pista) + "/" + to_string(sector);
                        result += Peso::peso_sector(primer_sector);
                        result2 += tamanio_sector;
                    }
                }
            }
        }
    
    cout << "CAPACIDAD TOTAL DEL DISCO ES : " << result2 << endl;
    cout << "CAPACIDAD USADA DEL DISCO ES : " << result << endl;
    cout << "CAPACIDAD RESTANTE DEL DISCO ES : " << (result2 - result) << endl;
}
