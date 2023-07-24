#include "auxiliares.h"

//funciones que ordenan los archivos en orden small, medium, large, xl (el orden dentro de cada tamaño de instancia no es relevante)
std::map<char, int> orden = {{'s',1} , {'m',2}, {'l',3}, {'x', 4}};
bool compareFilenames(const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b){
    return a.path().filename().string() < b.path().filename().string(); 
    
}

bool compareFilenames2(const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b){
    return orden[a.path().filename().string()[0]] <  orden[b.path().filename().string()[0]];
}


            