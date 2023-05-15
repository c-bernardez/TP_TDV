#include "greedy_solver.h"
#include <algorithm>
#include <chrono>

GreedySolver::GreedySolver() {}

GreedySolver::GreedySolver(TaxiAssignmentInstance &instance) {
    this->_instance = instance;
    this->_objective_value = 0;
    this->_solution_status = 0;
    this->_solution_time = 0;
}

void GreedySolver::setInstance(TaxiAssignmentInstance &instance) {
    this->_instance = instance;
}

void GreedySolver::solve() {
    auto start = std::chrono::high_resolution_clock::now(); //usamos la librería <chrono> para medir el tiempo de ejecución para el método solve
    TaxiAssignmentSolution solucion((this->_instance).n); //inicializamos una solucion a partir del n de la instancia de TaxiAssignmentSolution
    std::vector<std::vector<double>> distancias = (this->_instance).dist; //guardamos la matriz de distancias de la instancia en una variable por comodidad
    for(int j=0; j<distancias.size(); j++){ 
        //para cada pasajero j encontramos el taxi i más cercano al pasajero y que no se encuentre ya asignado
        double minimo = INT_MAX;
        int taxi = 0;
        for(int i=0; i<distancias.size(); i++){
            if(!solucion.isTaxiAssigned(i) && distancias[j][i]<minimo){ 
                minimo = distancias[i][j];
                taxi = i;
            }
        }
        solucion.assign(taxi, j);
    }
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    this->_solution_time = static_cast<double>(duration.count()); //guardamos el tiempo de ejecucion para la instancia

    for(int j=0; j<distancias.size();j++){
        //para cada pasajero j obtenemos el taxi que le fue asignado, y sumamos la distancia entre ellos al valor objetivo
        int taxi_asignado = solucion.getAssignedTaxi(j);
        this->_objective_value += distancias[taxi_asignado][j];
    }
    
}

double GreedySolver::getObjectiveValue() const {
    return this->_objective_value;
}

TaxiAssignmentSolution GreedySolver::getSolution() const {
    return this->_solution;
}

int GreedySolver::getSolutionStatus() const {
    return this->_solution_status;
}

double GreedySolver::getSolutionTime() const {
    return this->_solution_time;
}


