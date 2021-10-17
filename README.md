# HuffmanCoding
An C++ implementation of the algorithm Huffman Coding thats create the tree and returns the compression ratio

# How to use

> Compile in bash with "g++ main.cpp"

> Execute

> Write the route of the text to compress with the flags "-w" or "-c"

> The program will return the execution time, number of leaf nodes in the Huffman Tree...

> And approximation of the text size in bits, the compressed text size in bits and the compression ratio

The -w flag is to compress by word and -c to compress by character

By default the program will compress by character

# Example (in bash)

g++ main.cpp -o Huffman_compress

./Huffman_compress -w