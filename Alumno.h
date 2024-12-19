#ifndef ALUMNO_H
#define ALUMNO_H

#include <string>
using namespace std;

class Alumno {
public:
    int id;
    string nombre;
    string correo;
    string expedienteAcademico;
    string nivelIdiomas;
    bool autenticado;

    Alumno(int id, const string& nombre, const string& correo, const string& expediente, const string& nivelIdiomas, bool autenticado)
        : id(id), nombre(nombre), correo(correo), expedienteAcademico(expediente), nivelIdiomas(nivelIdiomas), autenticado(autenticado) {}

    void mostrarDatos() const;
};

#endif // ALUMNO_H
