#ifndef PLANCONVALIDACION_H
#define PLANCONVALIDACION_H

#include <string>
#include <vector>
using namespace std;

class PlanConvalidacion {
public:
    int idPlan;
    string carrera;
    string ubicacionCentro;

    PlanConvalidacion();
    void crearPlanConvalidacion(const vector<PlanConvalidacion>& planesExistentes);
    void guardarEnArchivo(const string& archivo) const;
};

vector<PlanConvalidacion> cargarPlanes(const string& archivo);

#endif // PLANCONVALIDACION_H
