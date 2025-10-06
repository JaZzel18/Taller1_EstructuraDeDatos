//
// Created by jazze on 05-10-2025.
//

#include "Vehiculo.h"
#include <cstdlib>
#include <cstring>
#include <iostream>


Vehiculo::Vehiculo() : idUsuario(0), monto(0), papelesAlDia(false), tieneFalla(false), siguiente(nullptr) {
    patente[0] = modelo[0] = mensaje[0] = '\0';
}


Vehiculo::Vehiculo(const char* p, int idU, const char* m, double mon, bool pap, bool falla, const char* msg)
    : idUsuario(idU), monto(mon), papelesAlDia(pap), tieneFalla(falla), siguiente(nullptr) {
    strncpy(patente, p, 9); patente[9] = '\0';
    strncpy(modelo, m, 49); modelo[49] = '\0';
    strncpy(mensaje, msg, 449); mensaje[449] = '\0';
}


Vehiculo Vehiculo::desdeLineaCSV(const char* linea) {
    char temp[512];
    strncpy(temp, linea, 511); temp[511] = '\0';

    char* token;
    char patente[10];
    int idUsuario;
    char modelo[50];
    double monto;
    char papeles[6];
    char falla[6];
    char mensaje[450];

    token = strtok(temp, ","); strncpy(patente, token, 9); patente[9] = '\0';
    token = strtok(NULL, ","); idUsuario = atoi(token);
    token = strtok(NULL, ","); strncpy(modelo, token, 49); modelo[49] = '\0';
    token = strtok(NULL, ","); monto = atof(token);
    token = strtok(NULL, ","); strncpy(papeles, token, 5); papeles[5] = '\0';
    token = strtok(NULL, ","); strncpy(falla, token, 5); falla[5] = '\0';
    token = strtok(NULL, ","); strncpy(mensaje, token, 449); mensaje[449] = '\0';

    Vehiculo v;
    strncpy(v.patente, patente, 9);
    v.idUsuario = idUsuario;
    strncpy(v.modelo, modelo, 49);
    v.monto = monto;
    v.papelesAlDia = strcmp(papeles, "true") == 0;
    v.tieneFalla = strcmp(falla, "true") == 0;
    strncpy(v.mensaje, mensaje, 449);
    return v;
}


void Vehiculo::mostrar() const {
    std::cout << patente << " | " << idUsuario << " | " << modelo
              << " | " << monto << " | " << (papelesAlDia ? "true" : "false")
              << " | " << (tieneFalla ? "true" : "false")
              << " | " << mensaje << "\n";
}


Vehiculo* Vehiculo::getSiguiente() const {
    return siguiente;
}


void Vehiculo::setSiguiente(Vehiculo* nodo) {
    siguiente = nodo;
}


const char* Vehiculo::getPatente() const {
    return patente;
}

int Vehiculo::getIdUsuario() const {
    return idUsuario;
}

const char* Vehiculo::getModelo() const {
    return modelo;
}

double Vehiculo::getMonto() const {
    return monto;
}

bool Vehiculo::getPapelesAlDia() const {
    return papelesAlDia;
}

bool Vehiculo::getTieneFalla() const {
    return tieneFalla;
}

const char* Vehiculo::getMensaje() const {
    return mensaje;
}