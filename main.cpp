#include "taxi_assignment_instance.h"
#include "taxi_assignment_solution.h"
#include "checker.h"
#include "greedy_solver.h"
#include "taxi_assignment_batching_solver.h"


int main(int argc, char** argv) {
    std::string filename = "input/small_1.csv";

    TaxiAssignmentInstance instance(filename); //crea una instancia de taxis y pasajeros
    std::cout << filename << std::endl;

    TaxiAssignmentSolution solution(instance.n); //crea una instancia de solucion al problema, con los taxis y pasajeros definidos anteriormente


    //GREEDY FCFS

    GreedySolver solver(instance); //creamos la instancia del GreedySolver
    solver.solve(); //minimiza la suma de las distancias entre taxis y pasajeros de forma 'greedy': FCFS
    solution = solver.getSolution(); //devuelve la asginacion obtenido
    auto time_greedy = solver.getSolutionTime(); //devuelve el tiempo de ejecución del greedy solver


    //MIN COST FLOW
    BatchingSolver mincostflow(instance); //creamos la instancia del BatchingSolver
    mincostflow.solve(); //minimiza la suma de las distancias con un grafo, modelado como flujo de costo minimo
    //TaxiAssignmentSolution solution_mincostflow(instance.n);
    //solution_mincostflow = mincostflow.getSolution(); //devuelve la asginacion obtenido
    // for(int i=0;i<10;i++){
    //     std::cout << "taxi: " << i<<  ", pasajero: " << solution_mincostflow.getAssignedPax(i);
    // }
    
    auto time_mcf = mincostflow.getSolutionTime(); //devuelve el tiempo de ejecución del min cost flow



    //chequeos
    std::cout << std::endl;

    std::cout << "objective value_greedy: " << solver.getObjectiveValue() << std::endl; //imprime el valor objetivo obtenido con el greedysolver
    std::cout << "tiempo de ejecución_greedy: " << time_greedy << " microsegundos" << std::endl;

    std::cout << std::endl;

    std::cout << "objective value_mincostflow: " << mincostflow.getObjectiveValue() <<  std::endl; //mprime el valor objetivo obtenido con el mincostflow
    std::cout << "tiempo de ejecución_mincostflow " << time_mcf << " microsegundos" << std::endl;

    std::cout << std::endl;

    return 0;
}