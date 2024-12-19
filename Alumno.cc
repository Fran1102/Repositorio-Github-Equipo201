#include "Alumno.h"
#include <iostream>

using namespace std;

void Alumno::mostrarDatos() const {
    cout << "ID: " << id << "\nNombre: " << nombre 
         << "\nCorreo: " << correo 
         << "\nExpediente: " << expedienteAcademico 
         << "\nNivel Idiomas: " << nivelIdiomas 
         << "\nAutenticado: " << (autenticado ? "Sí" : "No") << "\n";
}
