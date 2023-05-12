#include "greedy_solver.h"


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
    TaxiAssignmentSolution solucion((this->_instance).n);
    std::vector<std::vector<double>> distancias = (this->_instance).dist;
    for(int j=0; j<distancias.size(); j++){
        int minimo = INT_MAX;
        for(int i=0; i<distancias.size(); i++){
            if(!solucion.isTaxiAssigned(i) && distancias[j][i]<minimo){
                minimo = distancias[j][i];
            }
        }
        solucion.assign(minimo, j);
    }

    for(int j=0; j<distancias.size();j++){
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


