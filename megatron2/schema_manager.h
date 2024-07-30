#ifndef SCHEMA_MANAGER_H
#define SCHEMA_MANAGER_H

#include <string>
#include "pesos.h"

using namespace std;

#define esquemas "Esquemas"
#define tamanio_sector 1000


class SchemaManager {
public:
    static void create_esquema(string relacion_nombre = "");
    static bool existe_relacion(string relacion);
    static void create_relacion();
    static string encontrar_relacion(string nombre_relacion);
    static void new_register(string name_relacion, int cant_platos, int cant_superficies, int cant_pistas, int cant_sectores, bool llenar_desde_file = true, string n_registro = "", string name_archivo_relacion = "");
    static void dictionary_search(int register_data, int cant_platos, int cant_superficies, int cant_pistas, int cant_sectores);
};

#endif // SCHEMA_MANAGER_H
