//
// Created by jazze on 05-10-2025.
//

#ifndef UNTITLED1_USUARIO_H
#define UNTITLED1_USUARIO_H


class Usuario {
    private:
    int id;
    char nombre[50];
    double reputacion;
    int edad;
    char correo[50];
    char clave[50];

public:
    Usuario();
    Usuario(int _id, const char* _nombre, double _reputacion, int _edad,
            const char* _correo, const char* _clave);

    int getId() const;
    const char* getNombre() const { return nombre; }
    const char* getCorreo() const { return correo; }
    const char* getClave() const { return clave; }
    double getReputacion() const;
    int getEdad() const;
    void mostrar() const;
    static Usuario desdeLineaCSV(const char* linea);
};


#endif //UNTITLED1_USUARIO_H