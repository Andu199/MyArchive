#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stack>
#include "Huffman.h"

class Parser {
public:
    Parser(char[]);
    std::unordered_map<char, double> read_content();
    void create_file(Huffman&);
    void read_binary_content();
private:
    void encode(struct node*, std::string, std::unordered_map<char, std::string>&);
    void write_tree(struct node*, std::ofstream&);
    struct node read_tree(std::ifstream&, int);
    long long total_no = 0;
    std::string file_name;
};