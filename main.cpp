#include "taxi_assignment_instance.h"
#include "taxi_assignment_solution.h"
#include "checker.h"
#include "greedy_solver.h"


int main(int argc, char** argv) {
    std::string filename = "input/small_1.csv";

    TaxiAssignmentInstance instance(filename);
    std::cout << filename << std::endl;

    TaxiAssignmentSolution solution(instance.n);

    GreedySolver solver(instance);

    solver.solve();

    solution = solver.getSolution();

    auto time = solver.getSolutionTime();

    //chequeos
    
    std::cout << "tiempo de ejecución: " << time << std::endl;
    std::cout << "objective value: " << solver.getObjectiveValue() << std::endl;

    return 0;
}