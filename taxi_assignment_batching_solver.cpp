#include "taxi_assignment_batching_solver.h"
#include <algorithm>
#include <chrono>


BatchingSolver::BatchingSolver() {}

BatchingSolver::BatchingSolver(TaxiAssignmentInstance &instance) {
    this->_instance = instance;
    this->_objective_value = 0;
    this->_solution_status = 0;
    this->_solution_time = 0;
}

void BatchingSolver::setInstance(TaxiAssignmentInstance &instance) {
    this->_instance = instance;
}


void BatchingSolver::solve() {
    int n = (this->_instance).n;
    std::vector<std::vector<double>> distancias = (this->_instance).dist; 

    TaxiAssignmentSolution solucion((this->_instance).n);

    // Instantiate a SimpleMinCostFlow solver.
    operations_research::SimpleMinCostFlow min_cost_flow;
    
    //start_nodes = taxis, size = n*n
    int64_t n2 = static_cast<int64_t>(n);
    std::vector<int64_t> start_nodes = std::vector<int64_t>(n*n, 0);
    int64_t x = 0;
    for(int i=0; i<start_nodes.size(); i++){
        start_nodes[i] = x;
        if(i!=0 && i%n2 == n2-1){
            x++;
        }
    }
    
    //end_nodes = pasajeros, size = n*n
    std::vector<int64_t> end_nodes = std::vector<int64_t>(n*n, 0);
    x = 0;
    for(int64_t i=0; i<end_nodes.size(); i++){
        end_nodes[i] = x+n2;
        x++;
        if(x>=n){
            x=0;
        }
    }
    

    //capacities = capacidades, size = n*n
    //en nuestro modelo, la capacidad de cada arista es 1. si interpretamos al flujo como la cantidad de taxis, queda claro que solo uno a la vez pude ir a buscar a un pasajero
    std::vector<int64_t> capacities = std::vector<int64_t>(n*n, 1);
    
    //unit_cost = distancias = n*n
    std::vector<int64_t> unit_costs = std::vector<int64_t>(n*n, 0);
    x=0;
    for(int64_t i=0; i<distancias.size(); i++){
        for(int64_t j=0; j<distancias[i].size(); j++){
            unit_costs[x] = distancias[i][j]*10;
            x++;
        }
    }
    

    //supplies = imbalances, size = n+n
    //en nuestro modelo, los taxis pueden generar 1 unidad de flujo, y los pasajeros pueden captar 1 unidad
    std::vector<int64_t> supplies = std::vector<int64_t>(n+n, 0);
    for(int64_t i=0; i<supplies.size(); i++){
        if(i<supplies.size()/2){
            supplies[i] = 1; //imbalances de los taxis
        }
        else{
            supplies[i] = -1; //imbalances de los pasajeros
        }
    }
    
    // Add each arc.
    for (int i = 0; i < start_nodes.size(); ++i) {
        int arc = min_cost_flow.AddArcWithCapacityAndUnitCost(
            start_nodes[i], end_nodes[i], capacities[i], unit_costs[i]);
        if (arc != i) LOG(FATAL) << "Internal error";
    }

    // Add node supplies.
    for (int i = 0; i < supplies.size(); ++i) {
        min_cost_flow.SetNodeSupply(i, supplies[i]);
    }

    auto start = std::chrono::high_resolution_clock::now();

    int status = min_cost_flow.Solve(); //Encontramos el flujo de costo minimo

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    this->_solution_time = static_cast<double>(duration.count());

    this ->_objective_value = static_cast<double>(min_cost_flow.OptimalCost())/n; //Lo guardamos en el valor objetivo
    
    if (status == operations_research::MinCostFlow::OPTIMAL) {
        // LOG(INFO) << std::endl << "Minimum cost flow: " << static_cast<double>(min_cost_flow.OptimalCost())/n;
        // LOG(INFO) << "";
        // LOG(INFO) << " Arc   Flow / Capacity  Cost";
        // for (std::size_t i = 0; i < min_cost_flow.NumArcs(); ++i) {
        // double cost = static_cast<double>(min_cost_flow.Flow(i) * min_cost_flow.UnitCost(i))/n;
        // LOG(INFO) << min_cost_flow.Tail(i) << " -> " << min_cost_flow.Head(i)
        //             << "  " << min_cost_flow.Flow(i) << "  / "
        //             << min_cost_flow.Capacity(i) << "       " << cost;
        //}
        for (std::size_t i = 0; i < min_cost_flow.NumArcs(); ++i) {
        double cost = static_cast<double>(min_cost_flow.Flow(i) * min_cost_flow.UnitCost(i))/n;
        if(cost!=0){
            solucion.assign(min_cost_flow.Tail(i),min_cost_flow.Head(i));
        }
        }
    } else {
        LOG(INFO) << "Solving the min cost flow problem failed. Solver status: "
                << status;
    }
}



double BatchingSolver::getObjectiveValue() const {
    return this->_objective_value;
}

TaxiAssignmentSolution BatchingSolver::getSolution() const {
    return this->_solution;
}

int BatchingSolver::getSolutionStatus() const {
    return this->_solution_status;
}

double BatchingSolver::getSolutionTime() const {
    return this->_solution_time;
}

