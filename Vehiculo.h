//
// Created by jazze on 05-10-2025.
//

#ifndef UNTITLED1_VEHICULO_H
#define UNTITLED1_VEHICULO_H
#include <string>

class Vehiculo {
private:
    char patente[10];
    int idUsuario;
    char modelo[50];
    double monto;
    bool papelesAlDia;
    bool tieneFalla;
    char mensaje[450]; Vehiculo* siguiente;
public:
    Vehiculo();
    Vehiculo(const char* patente, int idUsuario, const char* modelo, double monto, bool papelesAlDia, bool tieneFalla, const char* mensaje);
    Vehiculo(const std::string & string, const std::string & modelo, double monto, bool monto1, bool papeles_al_dia, const std::string & mensaje);
    static Vehiculo desdeLineaCSV(const char* linea);

    const char* getPatente() const;
    int getIdUsuario() const;
    const char* getModelo() const;
    double getMonto() const;
    bool getPapelesAlDia() const;
    bool getTieneFalla() const;
    const char* getMensaje() const;
    Vehiculo* getSiguiente() const;
    void setSiguiente(Vehiculo* nodo);
    void mostrar() const;

};


#endif //UNTITLED1_VEHICULO_H