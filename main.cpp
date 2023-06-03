#include "taxi_assignment_instance.h"
#include "taxi_assignment_solution.h"
#include "checker.h"
#include "greedy_solver.h"
#include "auxiliares.h"
#include "taxi_assignment_batching_solver.h"
#include <fstream>
#include <limits>
#include <filesystem>


int main(int argc, char** argv) {
    //--------------------------------------------------------------------------------------------------
    //ejercicio 1: implementamos la estrategia FCFS con el archivo small_1.csv
    std::string filename = "input/small_1.csv";
    TaxiAssignmentInstance instance(filename); //crea una instancia de taxis y pasajeros (a usar en el ejercicio 1 y 3)
    

    //GREEDY FCFS
    GreedySolver solver(instance); //creamos la instancia del GreedySolver
    solver.solve(); //minimiza la suma de las distancias entre taxis y pasajeros de forma 'greedy': FCFS
    TaxiAssignmentSolution solution(instance.n); //crea una instancia de solucion al problema, con los taxis y pasajeros definidos anteriormente
    solution = solver.getSolution(); //guardamos la solucion obtenida con el GreedySolver en la instancia de TaxiAssignmentSolution creada
    double time_greedy = solver.getSolutionTime(); //guardamos el tiempo de ejecución del GreedySolver

    //resultados para small_1.csv con método greedy
    std::cout << "objective value_greedy: " << solver.getObjectiveValue() << std::endl;
    std::cout << "tiempo de ejecución_greedy: " << time_greedy << " microsegundos" << std::endl;

    std::cout << std::endl;

    //--------------------------------------------------------------------------------------------------
    //ejercicio 3: implementamos la estrategia de batching propuesta en el ejercicio 2 
    //MIN COST FLOW
    BatchingSolver mincostflow(instance); //creamos la instancia del BatchingSolver
    mincostflow.solve(); //minimiza la suma de las distancias con un grafo, modelado como flujo de costo minimo
    TaxiAssignmentSolution solution_mincostflow(instance.n);
    solution_mincostflow = mincostflow.getSolution(); //solucion obtenida (asignaciones de taxis-pasajeros)
    double time_mcf = mincostflow.getSolutionTime(); //tiempo de ejecucion del batching

    //resultados para small_1.csv con método min cost flow 
    std::cout << "objective value_mincostflow: " << mincostflow.getObjectiveValue() <<  std::endl; 
    std::cout << "tiempo de ejecución_mincostflow " << time_mcf << " microsegundos" << std::endl; //aclaración: la primera ejecución suele tardar significativamente más que las subsiguientes

    std::cout << std::endl;
    
    
    //--------------------------------------------------------------------------------------------------
    //ejercicio 4: experimentamos sobre todos los archivos
    std::string folderPath = "input/";
    std::vector<std::filesystem::directory_entry> entries;

    int i=0;
    for(const auto& entry : std::filesystem::directory_iterator(folderPath)){
        if (entry.is_regular_file() && entry.path().extension() == ".csv"){
            entries.push_back(entry); //creamos un vector 'entry' con todos los archivos a usar
        }
    }
    

    //ordenamos los archivos en orden small, medium, large, xl
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

            //calculamos el valor de la funcion objetivo, tiempo de ejecucion, y distancia maxima recorrida por un taxi para buscar a un pasajero bajo esta asignacion, con el método FCFS (greedy)
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

            //calculamos el valor de la funcion objetivo, tiempo de ejecucion, y distancia maxima recorrida por un taxi para buscar a un pasajero bajo esta asignacion, con el método Min Cost Flow (batching)
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
   


    //--------------------------------------------------------------------------------------------------
    //ejercicio 6: creamos un csv para analizar las quejas de los conductores de que muchas veces los viajes son muy cortos en comparacion a la distancia recorrida para llegar hasta el pasajero
    //consideramos que un viaje es lo suficientemente largo o "eficiente" si d_t >= d_ij, es decir si el ratio d_ij/d_t <= 1

    std::ofstream myfile2;
    myfile2.open ("ratios.csv");
    myfile2 << "archivo" << "," << "d_ij" << "," << "d_t" << "," << "ratio" << "\n";

    for(const auto& entry : entries){
        std::string filePath = entry.path().string();
        std::ifstream file(filePath);
        if(!file.is_open()){
            std::cout << "No se puede acceder al archivo: " << filePath << std::endl;
        }

        else{
            double d_ij, d_t, ratio;

            TaxiAssignmentInstance instance(filePath);


            BatchingSolver solver_mcf(instance); 
            solver_mcf.solve(); 
            TaxiAssignmentSolution solution_mcf(instance.n); 
            solution_mcf = solver_mcf.getSolution(); 
            for(int i=0; i<instance.n ; i++){
                int j = solution_mcf.getAssignedPax(i);
                d_ij = instance.dist[i][j];
                d_t = instance.pax_trip_dist[j];
                ratio = (d_ij/d_t);
                if(ratio == std::numeric_limits<double>::infinity()){
                    d_t = d_ij;
                    ratio = 1;
                }
                myfile2 << filePath << "," << d_ij << "," << d_t << "," << ratio << "\n";
            }
            
        //los calculos y análisis se encuentran en el informe
        }
    }



    //--------------------------------------------------------------------------------------------------
    //ejercicio 7: implementamos la estrategia propuesta en el ejercicio 6, basada en el ejercicio 3, adaptada para considerar la relación entre la distancia recorrida hasta al pasajero j, y luego el viaje a realizar
    BatchingSolver mcf(instance); 
    mcf.solve_alternativa(); 
    TaxiAssignmentSolution solution_mcf(instance.n);
    solution_mcf = mcf.getSolution();

    //resultados para small_1.csv con método adaptado, basado en el min cost flow
    std::cout << "funcion objetivo: " << mcf.getObjectiveValue() << std::endl;
    std::cout << "tiempo de ejecución: " << mcf.getSolutionTime() << std::endl; //aclaración: la primera ejecución suele tardar significativamente más que las subsiguientes

    //realizamos una experimentación para el nuevo modelo para compararlo con los métodos anteriores
    //computamos unicamente el valor de la funcion objetivo, luego calcularemos las mejoras relativas y absolutas con el archivo anterior gap.csv
    std::ofstream myfile3;
    myfile3.open ("gap_alternativa.csv");
    myfile3 << "archivo" << "," << "f_obj modelo" << "\n";
   
    for(const auto& entry : entries){
        std::string filePath = entry.path().string();
        std::ifstream file(filePath);
        if(!file.is_open()){
            std::cout << "No se puede acceder al archivo: " << filePath << std::endl;
        }

        else{
            TaxiAssignmentInstance instance(filePath);
            BatchingSolver solver_nuevo(instance); 
            solver_nuevo.solve_alternativa(); 
            TaxiAssignmentSolution solution_nuevo(instance.n); 
            double funcion_objetivo_nuevo = solver_nuevo.getObjectiveValue();

            myfile3 << filePath << "," << funcion_objetivo_nuevo << "\n";
        }
    }

    //calculamos los nuevos ratios, para ver si la ineficiencia mejoró
    std::ofstream myfile4;
    myfile4.open ("ratios_alternativa.csv");
    myfile4 << "archivo" << "," << "d_ij" << "," << "d_t" << "," << "ratio" << "\n";

    for(const auto& entry : entries){
        std::string filePath = entry.path().string();
        std::ifstream file(filePath);
        if(!file.is_open()){
            std::cout << "No se puede acceder al archivo: " << filePath << std::endl;
        }

        else{
            double d_ij, d_t, ratio;

            TaxiAssignmentInstance instance(filePath);
            BatchingSolver solver_mcf(instance); 
            solver_mcf.solve_alternativa(); 
            TaxiAssignmentSolution solution_mcf(instance.n); 
            solution_mcf = solver_mcf.getSolution(); 

            for(int i=0; i<instance.n ; i++){
                int j = solution_mcf.getAssignedPax(i);
                d_ij = instance.dist[i][j];
                d_t = instance.pax_trip_dist[j];
                ratio = (d_ij/d_t);
                if(ratio == std::numeric_limits<double>::infinity()){
                    d_t = d_ij;
                    ratio = 1;
                }
                myfile4 << filePath << "," << d_ij << "," << d_t << "," << ratio << "\n";
            }
        }
    }

    return 0;
}