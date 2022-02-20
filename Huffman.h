#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include "MyTree.h"


class Huffman {
public:
    Huffman();
    Huffman(std::unordered_map<char, double>);
    ~Huffman();
    void compute();
    void show(struct node*);
    int getTotalNodes() {return total_nodes;}
    void setTotalNodes(int _total_nodes) {total_nodes = _total_nodes;}
    node getRoot() {return characters.top();}
    void setRoot(node _root) {characters.push(_root);}
private:
    int total_nodes;
    void delete_tree(struct node*);
    std::priority_queue<struct node, std::vector<struct node>, CompareTree> characters;
};