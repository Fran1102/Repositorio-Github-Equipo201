#include "PlanConvalidacion.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

PlanConvalidacion::PlanConvalidacion() : idPlan(0), carrera(""), ubicacionCentro("") {}

void PlanConvalidacion::crearPlanConvalidacion(const vector<PlanConvalidacion>& planesExistentes) {
    int idNuevo;

    // Solicitar ID único
    while (true) {
        cout << "Introduce un ID único para el nuevo plan de convalidación: ";
        cin >> idNuevo;

        // Verificar si el ID ya existe
        bool idDuplicado = any_of(planesExistentes.begin(), planesExistentes.end(), [idNuevo](const PlanConvalidacion& plan) {
            return plan.idPlan == idNuevo;
        });

        if (idDuplicado) {
            cout << "Error: El ID ingresado ya está en uso. Por favor, elige otro ID.\n";
        } else {
            break;
        }
    }

    idPlan = idNuevo; // Asignar el ID ingresado

    // Solicitar datos del plan
    cout << "Introduce el nombre de la carrera: ";
    cin.ignore();
    getline(cin, carrera);

    cout << "Introduce la ubicación del centro: ";
    getline(cin, ubicacionCentro);

    cout << "Plan creado exitosamente.\n";
}

void PlanConvalidacion::guardarEnArchivo(const string& archivo) const {
    ofstream file(archivo, ios::app);
    if (!file) {
        cerr << "Error al abrir el archivo para guardar el plan.\n";
        return;
    }
    file << idPlan << "," << carrera << "," << ubicacionCentro << "\n";
    file.close();
}

vector<PlanConvalidacion> cargarPlanes(const string& archivo) {
    vector<PlanConvalidacion> planes;
    ifstream file(archivo);
    if (!file) {
        cerr << "Error al abrir el archivo de planes: " << archivo << endl;
        return planes;
    }

    string linea;
    while (getline(file, linea)) {
        stringstream ss(linea);
        string idStr, carrera, ubicacion;
        getline(ss, idStr, ',');
        getline(ss, carrera, ',');
        getline(ss, ubicacion, ',');

        if (idStr.empty() || carrera.empty() || ubicacion.empty()) {
            cerr << "Error: Línea mal formateada en el archivo de planes: " << linea << endl;
            continue;
        }

        try {
            int id = stoi(idStr);
            PlanConvalidacion plan;
            plan.idPlan = id;
            plan.carrera = carrera;
            plan.ubicacionCentro = ubicacion;
            planes.push_back(plan);
        } catch (const exception& e) {
            cerr << "Error al procesar la línea: " << linea << ". Excepción: " << e.what() << endl;
        }
    }

    file.close();
    return planes;
}
