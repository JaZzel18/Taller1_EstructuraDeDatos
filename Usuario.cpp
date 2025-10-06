//
// Created by jazze on 05-10-2025.
//

#include "Usuario.h"
#include <cstring>
#include <iostream>

Usuario::Usuario() {
    id = 0;
    nombre[0] = '\0';
    reputacion = 0;
    edad = 0;
    correo[0] = '\0';
    clave[0] = '\0';
}

Usuario::Usuario(int _id, const char* _nombre, double _reputacion, int _edad,
                 const char* _correo, const char* _clave) {
    id = _id;
    strncpy(nombre, _nombre, sizeof(nombre));
    reputacion = _reputacion;
    edad = _edad;
    strncpy(correo, _correo, sizeof(correo));
    strncpy(clave, _clave, sizeof(clave));
}

int Usuario::getId() const {
    return id;
}


void Usuario::mostrar() const {
    std::cout << "ID: " << id
              << ", Nombre: " << nombre
              << ", Reputación: " << reputacion
              << ", Edad: " << edad
              << ", Correo: " << correo << std::endl;
}

Usuario Usuario::desdeLineaCSV(const char* linea) {
    int id, edad;
    double reputacion;
    char nombre[50], correo[50], clave[50];

    sscanf(linea, "%d,%49[^,],%lf,%d,%49[^,],%49s",
           &id, nombre, &reputacion, &edad, correo, clave);

    return Usuario(id, nombre, reputacion, edad, correo, clave);
}

double Usuario::getReputacion() const {
    return reputacion;
}
int Usuario::getEdad() const {
    return edad;
}