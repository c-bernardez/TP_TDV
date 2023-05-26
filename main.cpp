#include "taxi_assignment_instance.h"
#include "taxi_assignment_solution.h"
#include "checker.h"
#include "greedy_solver.h"
#include "taxi_assignment_batching_solver.h"
#include <fstream>
#include <filesystem>
#include <map>

//ordenamos los archivos en orden small, medium, large, xl
std::map<char, int> orden = {{'s',1} , {'m',2}, {'l',3}, {'x', 4}};
bool compareFilenames(const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b){
    return a.path().filename().string() < b.path().filename().string();
    
}

bool compareFilenames2(const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b){
    return orden[a.path().filename().string()[0]] <  orden[b.path().filename().string()[0]];
}

int main(int argc, char** argv) {
    //ejercicio 1: implementamos la estrategia FCFS con el archivo small_1.csv
    std::string filename = "input/small_1.csv";
    TaxiAssignmentInstance instance(filename); //crea una instancia de taxis y pasajeros (a usar en el ejercicio 1 y 3)
    

    //GREEDY FCFS
    GreedySolver solver(instance); //creamos la instancia del GreedySolver
    solver.solve(); //minimiza la suma de las distancias entre taxis y pasajeros de forma 'greedy': FCFS
    TaxiAssignmentSolution solution(instance.n); //crea una instancia de solucion al problema, con los taxis y pasajeros definidos anteriormente
    solution = solver.getSolution(); //guardamos la solucion obtenida con el GreedySolver en la instancia de TaxiAssignmentSolution creada
    double time_greedy = solver.getSolutionTime(); //guardamos el tiempo de ejecución del GreedySolver


    //ejercicio 3: implementamos la estrategia de batching propuesta en el ejercicio 2 
    //MIN COST FLOW
    BatchingSolver mincostflow(instance); //creamos la instancia del BatchingSolver
    mincostflow.solve(); //minimiza la suma de las distancias con un grafo, modelado como flujo de costo minimo
    TaxiAssignmentSolution solution_mincostflow(instance.n);
    solution_mincostflow = mincostflow.getSolution(); //solucion obtenida (asignaciones de taxis-pasajeros)
    double time_mcf = mincostflow.getSolutionTime(); //tiempo de ejecucion del batching



    //ejercicio 4: experimentamos sobre todos los archivos
    std::string folderPath = "input/";
    std::vector<std::filesystem::directory_entry> entries;

    // BORRAR - creo que no hace falta
    // std::size_t numFiles = std::count_if(std::filesystem::directory_iterator(folderPath),   std::filesystem::directory_iterator(),[](const auto& entry) {                                      return entry.is_regular_file() && entry.path().extension() == ".csv"; });

    // entries.reserve(numFiles);

    int i=0;
    for(const auto& entry : std::filesystem::directory_iterator(folderPath)){
        if (entry.is_regular_file() && entry.path().extension() == ".csv"){
            entries.push_back(entry); //creamos un vector 'entry' con todos los archivos a usar
        }
    }

    //ordenamos los archivos
    std::sort(entries.begin(), entries.end(), compareFilenames);
    std::sort(entries.begin(), entries.end(), compareFilenames2);

    //creamos el archivo csv, con todas las columnas que vamos a usar
    std::ofstream myfile;
    myfile.open ("gap.csv");
    myfile << "archivo" << "," << "f_obj greedy" << "," << "tiempo greedy" << "," << "dist_max greedy" << "," << "f_obj mcf" << "," << "tiempo mcf" << "," << "dist_max mcf" << "," << "z_f_obj" << "," << "z_tiempo" << "," << "z_d_max" << "\n";

    for(const auto& entry : entries){
        //realizamos los experimentos sobre todos los archivos
        std::string filePath = entry.path().string();
        std::ifstream file(filePath);
        if(!file.is_open()){
            std::cout << "No se puede acceder al archivo: " << filePath << std::endl;
        }

        else{
            TaxiAssignmentInstance instance(filePath); //creamos la instancia a partir de cada archivo que tomamos

            //calculamos la funcion objetivo, tiempo de ejecucion, y distancia maxima recorrida por un taxi para buscar a un pasajero bajo esta asignacion, con el método FCFS (greedy)
            GreedySolver solver_greedy(instance); 
            solver_greedy.solve(); 
            TaxiAssignmentSolution solution_greedy(instance.n); 
            solution_greedy = solver_greedy.getSolution(); 

            double funcion_objetivo_greedy = solver_greedy.getObjectiveValue();
            double tiempo_greedy = solver_greedy.getSolutionTime(); 
            double distancia_max_greedy = 0;
            for(int i=0; i<instance.n ; i++){
                int j = solution_greedy.getAssignedPax(i);
                if(instance.dist[i][j] > distancia_max_greedy){
                    distancia_max_greedy = instance.dist[i][j];
                }
            }

            //calculamos la funcion objetivo, tiempo de ejecucion, y distancia maxima recorrida por un taxi para buscar a un pasajero bajo esta asignacion, con el método Min Cost Flow (batching)
            BatchingSolver solver_mcf(instance); 
            solver_mcf.solve(); 
            TaxiAssignmentSolution solution_mcf(instance.n); 
            solution_mcf = solver_mcf.getSolution(); 

            double funcion_objetivo_mcf = solver_mcf.getObjectiveValue();
            double tiempo_mcf= solver_mcf.getSolutionTime(); 
            double distancia_max_mcf = 0;
            for(int i=0; i<instance.n ; i++){
                int j = solution_mcf.getAssignedPax(i);
                if(instance.dist[i][j] > distancia_max_mcf){
                    distancia_max_mcf = instance.dist[i][j];
                }
            }
            
            //calculamos la mejora relativa en funcion del valor de la funcion objetivo, el tiempo para calcularla, y la distancia máxima que debe recorrer un taxi de la asignacion para recoger a su pasajero correspondiente
            double z_funcion_obj = (funcion_objetivo_greedy - funcion_objetivo_mcf)/funcion_objetivo_greedy;
            double z_tiempo = (tiempo_greedy - tiempo_mcf)/tiempo_greedy;
            double z_dist_max = (distancia_max_greedy - distancia_max_mcf)/distancia_max_greedy;


            //cargamos los datos calculados a nuestro csv
            myfile << filePath << "," << funcion_objetivo_greedy << "," << tiempo_greedy << "," << distancia_max_greedy << "," << funcion_objetivo_mcf << "," << tiempo_mcf << "," << distancia_max_mcf << "," << z_funcion_obj << "," << z_tiempo << "," << z_dist_max <<  "\n";
        }
    } 
   

    //ejercicio 7: implementamos la estrategia propuesta en el ejercicio 6, basada en el ejercicio 3, adaptada para considerar la relación entre la distancia recorrida hasta al pasajero j, y luego el viaje a realizar
    BatchingSolver mcf(instance); 
    mcf.solve_alternativa(); 
    TaxiAssignmentSolution solution_mcf(instance.n);
    solution_mcf = mcf.getSolution();
    std::cout << "asignacion: " << std::endl << solution_mcf << std::endl;
    std::cout << "funcion objetivo: " << mcf.getObjectiveValue() << std::endl;

    
    //chequeos
    // std::cout << std::endl;

    // std::cout << "objective value_greedy: " << solver.getObjectiveValue() << std::endl; //imprime el valor objetivo obtenido con el greedysolver
    // std::cout << "tiempo de ejecución_greedy: " << time_greedy << " microsegundos" << std::endl;

    // std::cout << std::endl;

    // std::cout << "objective value_mincostflow: " << mincostflow.getObjectiveValue() <<  std::endl; //mprime el valor objetivo obtenido con el mincostflow
    // std::cout << "tiempo de ejecución_mincostflow " << time_mcf << " microsegundos" << std::endl;

    // std::cout << std::endl;

    return 0;
}