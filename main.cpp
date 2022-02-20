#include <iostream>
#include <string.h>
#include "Parser.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 3)
        throw invalid_argument {"Invalid Input!\n"};
    
    if(strcmp(argv[2], "a") == 0) {
        Parser parser {argv[1]};
        Huffman huffman_tree {parser.read_content()};
        huffman_tree.compute();
        parser.create_file(huffman_tree);
        
    } else if(strcmp(argv[2], "d") == 0) {
        Parser parser {argv[1]};
        parser.read_binary_content();
        
    } else
        throw invalid_argument {"Invalid Command!\n"};
}