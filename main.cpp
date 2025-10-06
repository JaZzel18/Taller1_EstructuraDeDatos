#include <fstream>
#include <windows.h>
#include <cstring>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "Usuario.h"
#include "Vehiculo.h"


// Prototipos de funciones auxiliares :(
bool esCorreoValido(const char* correo);
int login(Usuario* usuarios, int nUsuarios, const char* correo, const char* clave);
Usuario registrarUsuario(Usuario*& usuarios, int& nUsuarios, int& capacidad);
void MenuPrincipal(Usuario& usuarioActual, Usuario* usuario, Vehiculo* cabezaVehiculos, int totalUsuarios, int totalRedimension);

int main() {
    int totalRedimension = 0;
    SetConsoleOutputCP(CP_UTF8);
    // CARGA DE USUARIOS
    int capacidadUsuarios = 5;
    int totalUsuarios = 0;
    Usuario* usuarios = (Usuario*) malloc(capacidadUsuarios * sizeof(Usuario));

    std::ifstream archivoUsuarios("usuarios.csv");
    if (!archivoUsuarios.is_open()) {
        std::cerr << "No se pudo abrir usuarios.csv\n";
        return 1;
    }

    char linea[512];
    if (archivoUsuarios.getline(linea, 512)) {
        if (strstr(linea, "Id") != nullptr) {
            // saltar encabezado
        } else {
            archivoUsuarios.seekg(0);
        }
    }

    while (archivoUsuarios.getline(linea, 512)) {
        if (strlen(linea) == 0) continue;

        Usuario usuario = Usuario::desdeLineaCSV(linea);


        if (totalUsuarios >= capacidadUsuarios) {
            capacidadUsuarios *= 2;
            usuarios = (Usuario*) realloc(usuarios, capacidadUsuarios * sizeof(Usuario));
            totalRedimension++;
        }

        // Insertar en orden por ID
        int i = totalUsuarios - 1;
        while (i >= 0 && usuarios[i].getId() > usuario.getId()) {
            usuarios[i + 1] = usuarios[i];
            i--;
        }
        usuarios[i + 1] = usuario;
        totalUsuarios++;
    }
    archivoUsuarios.close();

    std::cout << "Usuarios cargados:\n";
    for (int i = 0; i < totalUsuarios; i++) {
        usuarios[i].mostrar();
    }

    //CARGA DE VEHÍCULOS
    Vehiculo* cabezaVehiculos = nullptr;
    Vehiculo* ultimoVehiculo = nullptr;

    std::ifstream archivoVehiculos("vehiculos.csv");
    if (!archivoVehiculos.is_open()) {
        std::cerr << "No se pudo abrir vehiculos.csv\n";
        free(usuarios);
        return 1;
    }

    while (archivoVehiculos.getline(linea, 512)) {
        if (strlen(linea) == 0) continue;

        Vehiculo vehiculo = Vehiculo::desdeLineaCSV(linea);

        Vehiculo* nodoVehiculo = (Vehiculo*) malloc(sizeof(Vehiculo));
        *nodoVehiculo = vehiculo;
        nodoVehiculo->setSiguiente(nullptr);

        if (!cabezaVehiculos) {
            cabezaVehiculos = nodoVehiculo;
            ultimoVehiculo = nodoVehiculo;
            nodoVehiculo->setSiguiente(cabezaVehiculos);
        } else {
            ultimoVehiculo->setSiguiente(nodoVehiculo);
            nodoVehiculo->setSiguiente(cabezaVehiculos);
            ultimoVehiculo = nodoVehiculo;
        }
    }
    archivoVehiculos.close();

    std::cout << "\nVehículos cargados:\n";
    if (cabezaVehiculos) {
        Vehiculo* ptr = cabezaVehiculos;
        do {
            ptr->mostrar();
            ptr = ptr->getSiguiente();
        } while (ptr != cabezaVehiculos);
    }

    //MENÚ INICIAL
    int opcion;
    do {
        std::cout << "\n=== Menú Inicial ===\n";
        std::cout << "1. Iniciar sesión\n";
        std::cout << "2. Registrarse\n";
        std::cout << "3. Salir\n";
        std::cout << "Opción: ";
        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1: {
                char correo[50], clave[50];
                std::cout << "Correo: ";
                std::cin.getline(correo, 50);
                std::cout << "Clave: ";
                std::cin.getline(clave, 50);

                int indiceUsuario = login(usuarios, totalUsuarios, correo, clave);
                if (indiceUsuario != -1) {
                    Usuario& usuarioActual = usuarios[indiceUsuario];
                    std::cout << "¡Bienvenido " << usuarios[indiceUsuario].getNombre() << "!\n";
                    MenuPrincipal(usuarioActual, usuarios, cabezaVehiculos, totalUsuarios, totalRedimension);

                } else {
                    std::cout << "Credenciales incorrectas.\n";
                    std::cout << "volviendo al menu\n";
                }

            }
            case 2: {
                Usuario nuevoUsuario = registrarUsuario(usuarios, totalUsuarios, capacidadUsuarios);
                std::cout << "Usuario registrado con éxito. ¡Bienvenido " << nuevoUsuario.getNombre() << "!\n";
            }
            case 3: {
                std::cout << "Saliendo del programa.\n";
                break;
                default:
                std::cout << "Opcion invalida, seleccione una opcion disponible.\n";


            }
        }
    } while (opcion != 3);

    //LIBERAR MEMORIA
    free(usuarios);

    if (cabezaVehiculos) {
        Vehiculo* ptr = cabezaVehiculos->getSiguiente();
        while (ptr != cabezaVehiculos) {
            Vehiculo* aux = ptr;
            ptr = ptr->getSiguiente();
            free(aux);
        }
        free(cabezaVehiculos);
    }

    return 0;


}
//FUNCIONES AUXILIARES
bool esCorreoValido(const char* correo) {
    return strchr(correo, '@') != nullptr && strchr(correo, '.') != nullptr;
}

int login(Usuario* usuarios, int nUsuarios, const char* correo, const char* clave) {
    for (int i = 0; i < nUsuarios; i++) {
        if (strcmp(usuarios[i].getCorreo(), correo) == 0 &&
            strcmp(usuarios[i].getClave(), clave) == 0) {
            return i;
            }
    }
    return -1;
}

Usuario registrarUsuario(Usuario*& usuarios, int& nUsuarios, int& capacidad) {
    char nombre[50], correo[50], clave[50], repetir[50];
    int edad;

    std::cout << "Nombre y apellido: ";
    std::cin.ignore();
    std::cin.getline(nombre, 50);

    do {
        std::cout << "Correo: ";
        std::cin.getline(correo, 50);
        if (!esCorreoValido(correo)) std::cout << "Correo inválido.\n";
    } while (!esCorreoValido(correo));

    do {
        std::cout << "Edad: ";
        std::cin >> edad;
        std::cin.ignore();
    } while (edad <= 0 || edad >= 75);

    do {
        std::cout << "Clave: ";
        std::cin.getline(clave, 50);
        std::cout << "Repetir clave: ";
        std::cin.getline(repetir, 50);
        if (strcmp(clave, repetir) != 0) std::cout << "Claves no coinciden.\n";
    } while (strcmp(clave, repetir) != 0);

    // Generar ID aleatorio único
    int id;
    bool unico;
    do {
        id = rand() % 900 + 100;
        unico = true;
        for (int i = 0; i < nUsuarios; i++) {
            if (usuarios[i].getId() == id) {
                unico = false;
                break;
            }
        }
    } while (!unico);

    Usuario nuevoUsuario(id, nombre, 0.5, edad, correo, clave);

    if (nUsuarios >= capacidad) {
        capacidad *= 2;
        usuarios = (Usuario*) realloc(usuarios, capacidad * sizeof(Usuario));
    }

    usuarios[nUsuarios++] = nuevoUsuario;
    return nuevoUsuario;
}

void mostrarTodo(Usuario* usuarios, int totalUsuarios, Vehiculo* cabezaVehiculo) {

    std::cout <<"\n=== Patentes de Vehículos ===\n";
    if (cabezaVehiculo) {
        Vehiculo* ptr = cabezaVehiculo;
        do {
            std::cout << ptr->getPatente() << "\n";
            ptr = ptr->getSiguiente();
        } while (ptr != cabezaVehiculo);
    } else {
        std::cout << "No hay vehiculos registrados.\n";
    }

    //Mostrar Ids de todos los usuarios
    std::cout << "\n=== IDs de usuarios ===\n";
    for  (int i = 0; i < totalUsuarios; i++) {
        std::cout << usuarios[i].getId() << "\n";
    }
    std::cout << "------------------------------\n";

}
int buscarUsuarioPorId(Usuario* usuario, int totalUsuarios, int idBuscado) {
    int izquierda = 0;
    int derecha = totalUsuarios - 1;

    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;

        if (usuario[medio].getId() == idBuscado) {
            return medio;
        } else if (usuario[medio].getId() < idBuscado) {
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }

    return -1;
}

bool esConsonante(char c) {
    c = toupper(c);
    return (c >= 'A' && c <= 'Z') && !(c=='A'||c=='E'||c=='I'||c=='O'||c=='U');
}


bool esPatenteValida(const std::string& patente) {
    if (patente.length() != 6) return false;
    for (int i = 0; i < 4; i++) {
        if (!esConsonante(patente[i])) return false;
    }
    if (!isdigit(patente[4]) || !isdigit(patente[5])) return false;
    if (patente[4] == '0') return false;
    return true;
}


void publicarVehiculo(int idUsuarioActual, Vehiculo*& cabezaVehiculos) {
    std::string patente, modelo, papeles, falla, mensaje;
    double monto;
    bool errores = false;
    std::string erroresMsg;

    //Ingreso de datos
    std::cout << "Ingrese patente (4 consonantes + 2 dígitos, primer dígito !=0): ";
    std::cin >> patente;
    std::cin.ignore();
    if (!esPatenteValida(patente)) {
        errores = true;
        erroresMsg += "Patente inválida.\n";
    }

    std::cout << "Ingrese modelo (marca y año entre paréntesis): ";
    std::getline(std::cin, modelo);
    if (modelo.empty()) {
        errores = true;
        erroresMsg += "Modelo no puede estar vacío.\n";
    }

    std::cout << "Papeles al día (si/no): ";
    std::getline(std::cin, papeles);
    if (papeles != "si" && papeles != "no") {
        errores = true;
        erroresMsg += "Papeles deben ser 'si' o 'no'.\n";
    }

    std::cout << "Monto (máx. 1.000.000): ";
    std::cin >> monto;
    std::cin.ignore();
    if (monto <= 0 || monto > 1000000) {
        errores = true;
        erroresMsg += "Monto inválido.\n";
    }

    std::cout << "Tiene falla técnica (si/no): ";
    std::getline(std::cin, falla);
    if (falla != "si" && falla != "no") {
        errores = true;
        erroresMsg += "Falla técnica debe ser 'si' o 'no'.\n";
    }

    std::cout << "Ingrese mensaje adicional: ";
    std::getline(std::cin, mensaje);

    //Mostrar errores si existen
    if (errores) {
        std::cout << "\nErrores encontrados:\n" << erroresMsg;
        return;
    }

    // Crear vehículo nuevo
    Vehiculo* nuevoVehiculo = new Vehiculo(
        patente.c_str(),
        idUsuarioActual,
        modelo.c_str(),
        monto,
        papeles == "si",
        falla == "si",
        mensaje.c_str()
    );
    nuevoVehiculo->setSiguiente(nullptr);


    if (!cabezaVehiculos) {
        cabezaVehiculos = nuevoVehiculo;
        nuevoVehiculo->setSiguiente(cabezaVehiculos);
    } else {
        Vehiculo* ptr = cabezaVehiculos;
        while (ptr->getSiguiente() != cabezaVehiculos) ptr = ptr->getSiguiente();
        ptr->setSiguiente(nuevoVehiculo);
        nuevoVehiculo->setSiguiente(cabezaVehiculos);
    }

    std::cout << "Vehículo publicado exitosamente.\n";
}

void mostrarUsuarioPorId(Usuario* usuario, int totalUsuarios) {
    int id;
    std::cout << "Ingrese ID del usuario a buscar: ";
    std::cin >> id;
    std::cin.ignore();

    int id_encontrado = buscarUsuarioPorId(usuario, totalUsuarios, id);
    if (id_encontrado != -1) {
        Usuario& u = usuario[id_encontrado];
        std::cout << "\n=== Información del Usuario ===\n";
        std::cout << "Nombre: " << u.getNombre() << "\n";
        std::cout << "ID: " << u.getId() << "\n";
        std::cout << "Reputación: " << (u.getReputacion() * 100) << "%\n";
        std::cout << "Correo: " << u.getCorreo() << "\n";
        std::cout << "-------------------------------\n";
    } else {
        std::cout << "Usuario con ID " << id << " no encontrado.\n";
    }
}
void eliminarUsuario(int idUsuario, Usuario*& usuarios, int& totalUsuarios, Vehiculo*& cabezaVehiculos) {

    int idx = buscarUsuarioPorId(usuarios, totalUsuarios, idUsuario);
    if (idx == -1) {
        std::cout << " No se encontró usuario con ID " << idUsuario << "\n";
        return;
    }

    std::string nombreUsuario = usuarios[idx].getNombre();


    for (int i = idx; i < totalUsuarios - 1; i++) {
        usuarios[i] = usuarios[i + 1];
    }
    totalUsuarios--;


    if (cabezaVehiculos) {
        Vehiculo* actual = cabezaVehiculos;
        Vehiculo* prev = nullptr;
        bool primero = true;

        do {
            Vehiculo* siguiente = actual->getSiguiente();
            if (actual->getIdUsuario() == idUsuario) {
                if (prev) {
                    prev->setSiguiente(siguiente);
                    if (actual == cabezaVehiculos) cabezaVehiculos = siguiente;
                } else {

                    if (actual->getSiguiente() == actual) {

                        cabezaVehiculos = nullptr;
                    } else {

                        Vehiculo* ultimo = actual;
                        while (ultimo->getSiguiente() != actual) ultimo = ultimo->getSiguiente();
                        ultimo->setSiguiente(siguiente);
                        cabezaVehiculos = siguiente;
                    }
                }
                delete actual;
                actual = siguiente;
                if (cabezaVehiculos == nullptr) break;
                continue;
            }
            prev = actual;
            actual = siguiente;
            primero = false;
        } while (actual != cabezaVehiculos);
    }


    std::cout << "Usuario" << nombreUsuario << "' eliminado junto con sus vehículos.\n";
}

void generarReporte(Usuario* usuarios, int totalUsuarios, Vehiculo* cabezaVehiculos, int totalRedimenciones) {
    //Obtener fecha actual
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    std::ostringstream nombreArchivo;
    nombreArchivo << "estadisticas_"
                  << std::setfill('0') << std::setw(2) << now->tm_mday << "-"
                  << std::setfill('0') << std::setw(2) << now->tm_mon + 1 << "-"
                  << now->tm_year + 1900 << "_"
                  << std::setfill('0') << std::setw(2) << now->tm_hour
                  << std::setfill('0') << std::setw(2) << now->tm_min
                  << std::setfill('0') << std::setw(2) << now->tm_sec
                  << ".txt";

    std::ofstream archivo(nombreArchivo.str());
    if (!archivo.is_open()) {
        std::cout << "Error al crear archivo de reporte.\n";
        return;
    }

    archivo << "=== Reporte Estadísticas ===\n\n";
    archivo << "Cantidad de redimensionamientos del arreglo de usuarios: " << totalRedimenciones << "\n";

    //Vehículo más barato
    if (cabezaVehiculos) {
        Vehiculo* ptr = cabezaVehiculos;
        Vehiculo* masBarato = cabezaVehiculos;
        do {
            if (ptr->getMonto() < masBarato->getMonto()) masBarato = ptr;
            ptr = ptr->getSiguiente();
        } while (ptr != cabezaVehiculos);

        // Buscar nombre del usuario dueño
        int idDueño = masBarato->getIdUsuario();
        int indiceUsuario = -1;
        for (int i = 0; i < totalUsuarios; i++)
            if (usuarios[i].getId() == idDueño) { indiceUsuario = i; break; }

        archivo << "Vehículo más barato:\n";
        archivo << "Patente: " << masBarato->getPatente() << "\n";
        if (indiceUsuario != -1)
            archivo << "Nombre dueño: " << usuarios[indiceUsuario].getNombre() << "\n";
        archivo << "Monto: " << masBarato->getMonto() << "\n\n";
    }

    //Media aritmética de precios
    double suma = 0; int contador = 0;
    if (cabezaVehiculos) {
        Vehiculo* ptr = cabezaVehiculos;
        do {
            suma += ptr->getMonto();
            contador++;
            ptr = ptr->getSiguiente();
        } while (ptr != cabezaVehiculos);
    }
    archivo << "Media aritmética de los precios: ";
    archivo << (contador > 0 ? suma / contador : 0) << "\n\n";

    //Usuario confiable con más vehículos
    int maxVehiculos = 0; int idxMax = -1;
    for (int i = 0; i < totalUsuarios; i++) {
        if (usuarios[i].getReputacion() <= 0.65) continue;
        int cuenta = 0;
        if (cabezaVehiculos) {
            Vehiculo* ptr = cabezaVehiculos;
            do {
                if (ptr->getIdUsuario() == usuarios[i].getId()) cuenta++;
                ptr = ptr->getSiguiente();
            } while (ptr != cabezaVehiculos);
        }
        if (cuenta > maxVehiculos) {
            maxVehiculos = cuenta;
            idxMax = i;
        }
    }
    if (idxMax != -1) {
        archivo << "Usuario confiable con más vehículos:\n";
        archivo << "Nombre: " << usuarios[idxMax].getNombre() << "\n";
        archivo << "Correo: " << usuarios[idxMax].getCorreo() << "\n";
        archivo << "Cantidad de vehículos: " << maxVehiculos << "\n\n";
    } else {
        archivo << "No hay usuarios confiables con vehículos.\n\n";
    }

    archivo.close();
    std::cout << "Reporte generado: " << nombreArchivo.str() << "\n";
}
void liberarMemoria(Vehiculo*& cabeza) {
    Vehiculo* actual = cabeza;
    while (actual != nullptr) {
        Vehiculo* temp = actual;
        actual = actual->getSiguiente();
        delete temp;
    }
    cabeza = nullptr;
}
void guardarVehiculosCSV(Vehiculo* cabeza) {
    std::ofstream archivo("vehiculos.csv");
    if (!archivo.is_open()) return;

    Vehiculo* actual = cabeza;
    while (actual != nullptr) {
        archivo << actual->getPatente() << ","
                << actual->getIdUsuario() << ","
                << actual->getModelo() << ","
                << actual->getMonto() << ","
                << actual->getPapelesAlDia() << ","
                << actual->getTieneFalla() << ","
                << actual->getMensaje() << "\n";
        actual = actual->getSiguiente();
    }

    archivo.close();
}
void guardarUsuariosCSV(Usuario* usuarios, int totalUsuarios) {
    std::ofstream archivo("usuarios.csv");
    if (!archivo.is_open()) return;

    for (int i = 0; i < totalUsuarios; i++) {
        archivo << usuarios[i].getId() << ","
                << usuarios[i].getNombre() << ","
                << usuarios[i].getReputacion() << ","
                << usuarios[i].getEdad() << ","
                << usuarios[i].getCorreo() << ","
                << usuarios[i].getClave() << "\n";
    }

    archivo.close();
}
void vicualizarOferta() {
    std::cout << "no pude :o";
}

void MenuPrincipal(Usuario& usuarioActual, Usuario* usuario, Vehiculo* cabezaVehiculos, int totalUsuarios, int totalRedimension) {
    int opcion;

    do {
        std::cout << "Mi primer auto un millon ===\n";
        std::cout << "Usuario: " << usuarioActual.getNombre() << "\n";
        std::cout << "---------------------------------\n";
        std::cout << "1. Mostrar todo\n";
        std::cout << "2. Buscar un usuario\n";
        std::cout << "3. Publicar un Vehiculo\n";
        std::cout << "4. Visualizar ofertas\n";
        std::cout << "5. Eliminar un usuario\n";
        std::cout << "6. Generar reporte\n";
        std::cout << "7. salir\n";
        std::cout << "Seleccione una Opción: ";
        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1: {
                mostrarTodo(usuario, totalUsuarios, cabezaVehiculos);
                break;

            }
            case 2: {
                mostrarUsuarioPorId(usuario, totalUsuarios);
                break;

            }
            case 3: {
                publicarVehiculo(usuarioActual.getId(), cabezaVehiculos);
                break;


            }
            case 4: {
                vicualizarOferta();
                break;

            }
            case 5: {
                int id;
                std::cout << "Ingrese ID del usuario a eliminar: ";
                std::cin >> id;
                std::cin.ignore();
                eliminarUsuario(id, usuario, totalUsuarios, cabezaVehiculos);
                break;


            }
            case 6: {

                generarReporte(usuario, totalUsuarios, cabezaVehiculos, totalRedimension);
                break;



            }
            case 7:
                std::cout << "\nGuardando datos y cerrando el sistema...\n";
                guardarUsuariosCSV(usuario, totalUsuarios);
                guardarVehiculosCSV(cabezaVehiculos);
                liberarMemoria(cabezaVehiculos);
                delete[] usuario;
                std::cout << "Datos guardados correctamente. ¡Hasta luego!\n";
                break;
            default:
                std::cout << "Opción inválida.\n";
        }

    } while (opcion != 7);


}