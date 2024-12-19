#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Alumno.h"
#include "PlanConvalidacion.h"

using namespace std;

// Función para cargar los alumnos desde un archivo
vector<Alumno> cargarAlumnos(const string& archivo) {
    vector<Alumno> alumnos;
    ifstream file(archivo);
    if (!file) {
        cerr << "Error al abrir el archivo de alumnos: " << archivo << endl;
        return alumnos;
    }

    string linea;
    while (getline(file, linea)) {
        stringstream ss(linea);
        string idStr, nombre, correo, expediente, nivelIdiomas, autenticadoStr;
        getline(ss, idStr, ',');
        getline(ss, nombre, ',');
        getline(ss, correo, ',');
        getline(ss, expediente, ',');
        getline(ss, nivelIdiomas, ',');
        getline(ss, autenticadoStr, ',');

        try {
            int id = stoi(idStr);
            bool autenticado = (autenticadoStr == "1");
            alumnos.push_back(Alumno(id, nombre, correo, expediente, nivelIdiomas, autenticado));
        } catch (const exception& e) {}
    }

    file.close();
    return alumnos;
}



// Función para guardar las inscripciones en un archivo
void guardarInscripcionesEnArchivo(const unordered_map<int, set<int>>& registroInscripciones, const string& archivo) {
    ofstream file(archivo, ios::trunc);  // Abrir el archivo en modo truncado para sobrescribir
    if (!file) {
        cerr << "Error al abrir el archivo de inscripciones.\n";
        return;
    }

    // Itera sobre todas las inscripciones y las guarda en el archivo
    for (const auto& pair : registroInscripciones) {
        int alumnoId = pair.first;
        const set<int>& planes = pair.second;
        for (int planId : planes) {
            file << alumnoId << "," << planId << "\n";  // Guarda alumnoId y planId
        }
    }

    file.close();
}

// Función para cargar las inscripciones desde un archivo
unordered_map<int, set<int>> cargarInscripcionesDesdeArchivo(const string& archivo) {
    unordered_map<int, set<int>> registroInscripciones;
    ifstream file(archivo);
    if (!file) {
        cerr << "Error al abrir el archivo de inscripciones.\n";
        return registroInscripciones;  // Retorna un mapa vacío si no se puede abrir el archivo
    }

    string linea;
    while (getline(file, linea)) {
        stringstream ss(linea);
        string alumnoIdStr, planIdStr;
        getline(ss, alumnoIdStr, ',');
        getline(ss, planIdStr, ',');

        try {
            int alumnoId = stoi(alumnoIdStr);
            int planId = stoi(planIdStr);
            registroInscripciones[alumnoId].insert(planId);  // Inserta la inscripción en el registro
        } catch (const exception& e) {
            cerr << "Error al procesar la línea: " << linea << ". Excepción: " << e.what() << endl;
        }
    }

    file.close();
    return registroInscripciones;
}

// Función para inscribir a un alumno en un plan
void inscribirAlumno(const Alumno& alumno, const vector<PlanConvalidacion>& planes, unordered_map<int, set<int>>& registroInscripciones) {
    cout << "Introduce el ID del plan de convalidación para inscribir al alumno " << alumno.nombre << ":\n";
    for (const auto& plan : planes) {
        cout << plan.idPlan << ": " << plan.carrera << " - " << plan.ubicacionCentro << endl;
    }

    int planId;
    cin >> planId;

    // Verificar si el alumno ya está inscrito en el plan
    if (registroInscripciones[alumno.id].count(planId)) {
        cout << "El alumno ya está inscrito en este plan de convalidación.\n";
    } else {
        registroInscripciones[alumno.id].insert(planId);
        cout << "Inscripción realizada con éxito.\n";
    }
}

// Función para consultar las solicitudes de inscripción de un alumno
void consultarSolicitudesInscripcion(int idAlumno, const unordered_map<int, set<int>>& registroInscripciones, const vector<PlanConvalidacion>& planes) {
    auto it = registroInscripciones.find(idAlumno);

    if (it != registroInscripciones.end()) {
        const set<int>& planesInscritos = it->second;
        cout << "Planes en los que el alumno con ID " << idAlumno << " está inscrito:\n";

        for (int planId : planesInscritos) {
            // Buscar el plan por ID y mostrar la información del plan
            auto planIt = find_if(planes.begin(), planes.end(), [planId](const PlanConvalidacion& plan) {
                return plan.idPlan == planId;
            });

            if (planIt != planes.end()) {
                cout << "ID Plan: " << planIt->idPlan << ", Carrera: " << planIt->carrera << ", Ubicación: " << planIt->ubicacionCentro << endl;
            }
        }
    } else {
        cout << "El alumno con ID " << idAlumno << " no tiene solicitudes de inscripción.\n";
    }
}

int main() {
    const string archivoAlumnos = "alumnos.txt";
    const string archivoPlanes = "planes.txt";
    const string archivoInscripciones = "inscripciones.txt";  // Nuevo archivo de inscripciones

    // Cargar los datos
    vector<Alumno> alumnos = cargarAlumnos(archivoAlumnos);
    vector<PlanConvalidacion> planes = cargarPlanes(archivoPlanes);
    unordered_map<int, set<int>> registroInscripciones = cargarInscripcionesDesdeArchivo(archivoInscripciones);  // Cargar las inscripciones desde el archivo

    int opcion;
    do {
        cout << "\nMenú Principal\n";
        cout << "1. Inscribir alumno en SICUE\n";
        cout << "2. Mostrar alumnos\n";
        cout << "3. Crear nuevo plan de convalidación\n";
        cout << "4. Consultar solicitudes de inscripción de un alumno\n";
        cout << "0. Salir\n";
        cout << "Elige una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << "Introduce el ID del alumno: ";
                int idSeleccionado;
                cin >> idSeleccionado;

                auto it = find_if(alumnos.begin(), alumnos.end(), [idSeleccionado](const Alumno& a) {
                    return a.id == idSeleccionado;
                });

                if (it != alumnos.end()) {
                    inscribirAlumno(*it, planes, registroInscripciones);
                    guardarInscripcionesEnArchivo(registroInscripciones, archivoInscripciones);  // Guardar las inscripciones después de cada inscripción
                } else {
                    cout << "Alumno no encontrado.\n";
                }
                break;
            }
            case 2:
                for (const auto& alumno : alumnos) {
                    alumno.mostrarDatos();
                }
                break;
            case 3: {
                PlanConvalidacion nuevoPlan;
                nuevoPlan.crearPlanConvalidacion(planes);
                nuevoPlan.guardarEnArchivo(archivoPlanes);
                planes = cargarPlanes(archivoPlanes); // Recargar planes
                break;
            }
            case 4: {
                cout << "Introduce el ID del alumno: ";
                int idAlumno;
                cin >> idAlumno;
                consultarSolicitudesInscripcion(idAlumno, registroInscripciones, planes);
                break;
            }
            case 0:
                cout << "Saliendo del programa.\n";
                guardarInscripcionesEnArchivo(registroInscripciones, archivoInscripciones);  // Guardar las inscripciones al salir
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (opcion != 0);

    return 0;
}
