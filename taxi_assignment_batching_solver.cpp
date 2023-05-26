#include "taxi_assignment_batching_solver.h"
#include <algorithm>
#include <chrono>
#include <cmath>


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
        end_nodes[i] = x+n2; //le sumamos n a los numeros de los pasajeros para poder diferenciarlos de los taxistas
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
            unit_costs[x] = distancias[i][j]*10; //multiplicamos por 10 para convertir a entero sin redondear
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

    
    if (status == operations_research::MinCostFlow::OPTIMAL) {
        this ->_objective_value = static_cast<double>(min_cost_flow.OptimalCost())/n; //Lo guardamos en el valor objetivo
        // LOG(INFO) << std::endl << "Minimum cost flow: " << static_cast<double>(min_cost_flow.OptimalCost())/n;
        // LOG(INFO) << "";
        // LOG(INFO) << " Arc   Flow / Capacity  Cost";
        // for (std::size_t i = 0; i < min_cost_flow.NumArcs(); ++i) {
        // double cost = static_cast<double>(min_cost_flow.Flow(i) * min_cost_flow.UnitCost(i))/n;
        // LOG(INFO) << min_cost_flow.Tail(i) << " -> " << min_cost_flow.Head(i)
        //             << "  " << min_cost_flow.Flow(i) << "  / "
        //             << min_cost_flow.Capacity(i) << "       " << cost;
        // }
        for (int i = 0; i < min_cost_flow.NumArcs(); ++i) {
            double cost = static_cast<double>(min_cost_flow.Flow(i) * min_cost_flow.UnitCost(i))/10; //dividimos por el 10 que habiamos multiplicado antes, para volver a los valores decimales originales
            if(cost!=0){
                solucion.assign(min_cost_flow.Tail(i),min_cost_flow.Head(i)-n); //le quitamos el n a los pasajeros sumado anteriormente, para que queden los numeros originales correspondientes (y podamos realizar la asignacion)
            }
        }
        
    } else {
        LOG(INFO) << "Solving the min cost flow problem failed. Solver status: "
                << status;
    }

    this ->_solution = solucion; //guardamos la solucion en el atributo _solution de la clase
}

void BatchingSolver::solve_alternativa() {
    int n = (this->_instance).n;
    std::vector<std::vector<double>> distancias = (this->_instance).dist; 

    TaxiAssignmentSolution solucion((this->_instance).n);

    operations_research::SimpleMinCostFlow min_cost_flow;

    int64_t n2 = static_cast<int64_t>(n);
    std::vector<int64_t> start_nodes = std::vector<int64_t>(n*n, 0);
    int64_t x = 0;
    for(int i=0; i<start_nodes.size(); i++){
        start_nodes[i] = x;
        if(i!=0 && i%n2 == n2-1){
            x++;
        }
    }

    std::vector<int64_t> end_nodes = std::vector<int64_t>(n*n, 0);
    x = 0;
    for(int64_t i=0; i<end_nodes.size(); i++){
        end_nodes[i] = x+n2;
        x++;
        if(x>=n){
            x=0;
        }
    }

    std::vector<int64_t> capacities = std::vector<int64_t>(n*n, 1);
    
    //diferente a la implementacion anterior
    std::vector<int64_t> unit_costs = std::vector<int64_t>(n*n, 0);
    x=0;
    for(int64_t i=0; i<distancias.size(); i++){
        for(int64_t j=0; j<distancias[i].size(); j++){
            unit_costs[x] = (distancias[i][j] / (this->_instance).pax_trip_dist[j]) * 1000; 
            //consideramos al costo como un ratio entre la distancia recorrida hasta el pasajero y la distancia del viaje en sí. Si el ratio es menor a 1, la distancia recorrida hasta el pasajero es menor que la del viaje total, y por ende es más "util" para el taxista. Así, seguimos buscando minimizar costos, solo que ahora minimizamos los ratios.
            //lo multiplicamos por 1000 para pasarlo a entero sin que algun costo nos quede en 0 la redondear
            x++;
        }
    }
    

    std::vector<int64_t> supplies = std::vector<int64_t>(n+n, 0);
    for(int64_t i=0; i<supplies.size(); i++){
        if(i<supplies.size()/2){
            supplies[i] = 1; 
        }
        else{
            supplies[i] = -1; 
        }
    }
    

    for (int i = 0; i < start_nodes.size(); ++i) {
        int arc = min_cost_flow.AddArcWithCapacityAndUnitCost(
            start_nodes[i], end_nodes[i], capacities[i], unit_costs[i]);
        if (arc != i) LOG(FATAL) << "Internal error";
    }
    

    for (int i = 0; i < supplies.size(); ++i) {
        min_cost_flow.SetNodeSupply(i, supplies[i]);
    }

    auto start = std::chrono::high_resolution_clock::now();

    int status = min_cost_flow.Solve(); 

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    this->_solution_time = static_cast<double>(duration.count());

    
    if (status == operations_research::MinCostFlow::OPTIMAL) {
        this ->_objective_value = 0;
        for (int i = 0; i < min_cost_flow.NumArcs(); ++i) {
            double cost = static_cast<double>(min_cost_flow.Flow(i) * min_cost_flow.UnitCost(i))/1000;
            //dividimos por el 1000 multiplicado anteriormente para obtener los costos verdaderos (los ratios son doubles)
            if(cost!=0){
                solucion.assign(min_cost_flow.Tail(i),min_cost_flow.Head(i)-n); //quitamos el n
                this ->_objective_value += cost * (this->_instance).pax_trip_dist[min_cost_flow.Head(i)-n]; //habiendo calculado los costos como dij/dj, podemos recuperar los dij que nos interesan para el valor del a funcion objetivo si hacemos costo*dj (dj es la distancia del viaje del pasajero j, una vez sea recogido por el taxi)
            }
        }
        
        
    } else {
        LOG(INFO) << "Solving the min cost flow problem failed. Solver status: "
                << status;
    }

    this ->_solution = solucion;
    this->_objective_value = std::trunc(this->_objective_value*10)/10; //truncamos el valor de la funcion objetivo a un lugar decimal
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


