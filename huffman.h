#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <utility>


class Huffman_Node{
    public:
        std::string data;
        int freq;
        Huffman_Node *right;
        Huffman_Node *left;
        // Constructor
        Huffman_Node(std::string data, int freq){
            this -> data = data;
            this -> freq = freq;
            this -> right = NULL;
            this -> left = NULL;
        }
};

// Funcion para leer el texto por caracter. Retorna el tamaño en bits del archivo
long long Read_by_char(std::string file_name, std::unordered_map<std::string, int> &map);

// Funcion para leer el texto por palabra. Retorna el tamaño en bits del archivo (aprox)
long long Read_by_string(std::string file_name, std::unordered_map<std::string, int> &map);

// Obtenemos el tamaño en bits de los codigos de cada caracter recorriendo las hojas en pre-order y lo guarda en tree_size
// Ademas de guardar la cantidad de nodos en n_nodes
void Compressed_text_size(Huffman_Node *n, std::string code, long long *tree_size, long long *n_nodes);

// Comparador de pares respecto al segundo elemento para la cola de prioridad
struct comp_Huff_Node{
    bool operator()(Huffman_Node *a, Huffman_Node *b){
        return a -> freq > b -> freq;
    }
};

// --- dificación de Huffman para caracteres --- 
double Huffman_compress_ratio(std::string file_name, bool mode){

    char c;                     // La usamos para almacenar los caracteres del texto y procesarlos
    long long text_size;        // La usamos para calcular el tamaño en bytes del texto
    long long n_nodos = 0;      // Aqui almacenamos la cantidad de nodos con palabras o caracteres
    Huffman_Node *root = NULL;  // Aqui almacenaremos la raiz al arbol de Huffman

    // Creamos un mapa para guardar frecuencias
    std::unordered_map<std::string, int> map;

    if(mode)
        text_size = Read_by_string(file_name, map);
    else
        text_size = Read_by_char(file_name, map);


    // Creamos un min heap a base de una priority queue para los elementos "caracter - frecuencia"
    // Tienen mayor prioridad los elementos con menor frecuencia para armar el arbol
    std::priority_queue<Huffman_Node *, std::vector<Huffman_Node *>, comp_Huff_Node> *min_heap = new std::priority_queue<Huffman_Node *, std::vector<Huffman_Node *>, comp_Huff_Node>();

    // Pusheamos los elementos del map como nodos al min heap
    for(auto e: map){
        // e.first = caracter y e.second = frecuencia
        /*if(e.first == "\n")
            std::cout << "\\n " << e.second << std::endl;
        else
            std::cout << e.first << " " << e.second << std::endl;*/
        min_heap -> push(new Huffman_Node(e.first, e.second));
    }


    // Creamos el arbol de Huffman para codificar los caracteres
    // Esta parte está basada de la fuente:
    // https://gist.github.com/pwxcoo/72d7d3c5c3698371c21e486722f9b34b
    while(min_heap -> size() != 1){
        // Tomamos los nodos al tope del heap
        Huffman_Node *left = min_heap -> top();
        min_heap -> pop();
        Huffman_Node *right = min_heap -> top();
        min_heap -> pop();

        // Creamos un nodo "vacío" que unirá los 2 nodos anteriores y que tendrá la suma de sus frecuencias
        int freq_sum = right -> freq + left -> freq;
        Huffman_Node *aux = new Huffman_Node("\0", freq_sum);
        aux -> right = right;
        aux -> left = left;
        // Lo colocamos en el heap
        min_heap -> push(aux);
    }
    // Colocamos el ultimo nodo del min heap como raíz
    root = min_heap -> top();
    min_heap -> pop();

    // Ahora que tenemos el arbol, calculamos el ratio de compresion y lo retornamos
    long long compressed_size = 0;
    Compressed_text_size(root, "", &compressed_size, &n_nodos);

    double compress_ratio = (double)compressed_size / text_size;
    std::cout << "Text size: " << text_size << " bits" << std::endl;
    std::cout << "Compressed text size: " << compressed_size << " bits" << std::endl;
    std::cout << "nodes: " << n_nodos << std::endl;
    std::cout << "Compress ratio of " << file_name << " : " << compress_ratio << std::endl;
    return compress_ratio;
}

// Para leer por caracteres
long long Read_by_char(std::string file_name, std::unordered_map<std::string, int> &map){

    // Abrir archivo para leer
    FILE *fp = fopen(file_name.c_str(), "r");   // Abrir archivo
    char c;                                     // Aqui guardaremos los caracteres que vamos leyendo
    long long text_size = 0;                          // Contaador de los caracteres totales en el texto

    // Si no encuentra el archivo, retorna NULL
    if(fp == NULL){
        std::cout << file_name << " no encontrado!\n";
        return -1;
    }
    c = fgetc(fp);

    // Nos aseguramos de que el archivo a codificar no este vacio
    // Si lo esta, retorna NULL
    if(c == EOF){
        std::cout << "El archivo " << file_name << " esta vacio!\n";
        return -1;
    }
    // Si no, inserta el caracter leido en el mapa
    else{
        text_size += 8;
        map[std::string(1, c)]++;
    }

    // Por cada caracter, aumentamos en 1 su frecuencia en el diccionario
    while((c = fgetc(fp)) !=  EOF){
        text_size += 8;
        map[std::string(1, c)]++;
    }

    return text_size;
}

// Para leer por palabras
long long Read_by_string(std::string file_name, std::unordered_map<std::string, int> &map){
    long long text_size = 0;  // Para guardar la cantidad de palabras
    std::string line, word;   // Para guardar las lineas y parsearlas a palabras
    std::istringstream iss;   // Para parsear las lineas y guardar las palabras en "word"

    std::ifstream inFile;     // Para abrir el archivo
    inFile.open(file_name);

    // Si no se encuentra el archivo, retornar error y -1
    if(!inFile.is_open()){
        std::cout << "No se encontro archivo " << file_name << std::endl;
        return -1;
    }

    // Si el archivo está vacio, reetornar error y -1
    if(inFile.eof()){
        std::cout << "El archivo " << file_name << " esta vacio!\n";
        return -1;
    }

    // Con el archivo abierto
    while(!inFile.eof()){
        // Lo leemos linea por linea
        std::getline(inFile, line);
        //std::cout << line << std::endl;
        // Y lo separamos por palabras
        if(inFile.good()){
            iss.clear();
            iss.str(line);
            // Cada palabra la pusheamos al mapa 
            while(iss.good()){
                iss >> word;
                //std::cout << word << std::endl;
                map[word]++;
                // Y aumentamos en word.size() * 8 la cantidad de palabras para el ratio
                text_size += word.size() * 8;
            }
        }
    }
    return text_size;
}

void Compressed_text_size(Huffman_Node *n, std::string code, long long *tree_size, long long *n_nodes){
    
    // Si es un nodo vacío (hijo de una hoja) terminar
    if(n == NULL)
        return;
    
    // Si es una hoja, sumamos el largo de la codificacion del caracter por su numero de ocurrencias
    // y aumentamos en 1 la cantidad de nodos con caracteres o palabras
    if(n -> left == NULL && n -> right == NULL){
        //std::cout << "element: " << n -> data << " freq: " << n -> freq << " code: " << code << std::endl;
        *tree_size += code.size() * n -> freq;
        *n_nodes += 1;
    }

    // Si es un nodo intermedio, recorremos recursivamente en ambas direcciones
    Compressed_text_size(n -> left, code + "0", tree_size, n_nodes);
    Compressed_text_size(n -> right, code + "1", tree_size, n_nodes);
}


#endif