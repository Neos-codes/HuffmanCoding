#include <iostream>
#include <string.h>
#include <time.h>
#include "huffman.h"
#include <limits>

int main(int argc, char *argv[]){

    bool mode = false;   // mode = false comprimir por caracteres, mode = true comprimir por letras
    clock_t start, end;
    
    // Escogemos el modo de compresion. Si no tiene flag, comprime por caracteres por defecto
    for(int i = 1; i < argc; i++){
        if(!strcmp(argv[i], "-w")){
            std::cout << "Compresion por palabras\n";
            mode = true;
            break;
        }
        else if(!strcmp(argv[i], "-c")){
            std::cout << "Compresion por caracteres\n";
            break;
        }
    }
    std::string file_name;
    std::cout << "Ingrese la ruta del archivo a codificar: ";
    std::cin >> file_name;
    
    start = clock();

    Huffman_compress_ratio(file_name, mode);

    end = clock();
    std::cout << "Tiempo de ejecucion: " << (double)(end - start) / CLOCKS_PER_SEC;

    return 0;
}