#include "Huffman.h"

Huffman::Huffman(){}

Huffman::Huffman(std::unordered_map<char, double> arg) {
    total_nodes = 0;
    for(auto a : arg) {
        characters.push({nullptr, nullptr, a.first, a.second});
        total_nodes++;
    }
}

void Huffman::delete_tree(struct node* root) {
    if(root == nullptr)
        return;
    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}

Huffman::~Huffman() {
    auto root = characters.top();
    characters.pop();
    if(!characters.empty()) {
        std::cerr << "ERROR! BAD FUNCTION CALL!\n";
        return;
    }
    delete_tree(root.left);
    delete_tree(root.right);
}

void Huffman::compute() {
    while(characters.size() > 1) {
        struct node* first = new node (characters.top());
        characters.pop();
        struct node* second = new node (characters.top());
        characters.pop();
        struct node newelement {first, second, 0, first->weight + second->weight};
        characters.push(newelement);
        total_nodes++;
    }
}

void Huffman::show(struct node *element) {
    if(element == nullptr)
        return;
    show(element->left);
    printf("%c --> %f\n", element->value, element->weight);
    show(element->right);
}