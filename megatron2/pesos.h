#ifndef PESOS_H
#define PESOS_H

#include <string>
#include <vector>
#include "data_manager.h"

using namespace std;

#define tamanio_sector 1000 // 10000 bits

class Peso {
public:
    static int calcular_peso(vector<string> elementos);
    static int peso_sector(string sector_archivo);
    static bool verificar_peso(string sector_archivo, string nuevo_registro);
    static void peso_disco(int cant_platos, int cant_superficies, int cant_pistas, int cant_sectores);
};

#endif 