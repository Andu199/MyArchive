#include "Parser.h"

Parser::Parser(char *file) : file_name {file} { }

std::unordered_map<char, double> Parser::read_content() {
    std::ifstream f;
    f.open(file_name.c_str(), std::ios::binary);
    char ch;
    std::vector<long long> counter (256, 0);
    while(true) {
        f.read(&ch, 1);
        if(f.eof()) break;
        counter[(unsigned char) ch] ++;
		total_no++;
    }
    f.close();
	
	std::unordered_map<char, double> result;
	for(int i = 0; i < 256; ++i)
		if(counter[i])
			result.insert({(char) i, (double) counter[i] / total_no});

	return result;
}

void Parser::write_tree(struct node *root, std::ofstream& g) {
    if(root == nullptr)
        return;
    char leaf = 0;
    if(root->left == nullptr && root->right == nullptr)
        leaf = 1;
    g.write(&root->value, 1);
    g.write(&leaf, 1);
    write_tree(root->left, g);
    write_tree(root->right, g);
}

void Parser::encode(struct node *root, std::string code,
    std::unordered_map<char, std::string>& encoding) {
    if(root->left == nullptr && root->right == nullptr) {
        encoding.insert({(unsigned char) root->value, code});
        return;
    }
    encode(root->left, code + "0", encoding);
    encode(root->right, code + "1", encoding);
}

void Parser::create_file(Huffman &huffman_tree) {
    std::ifstream f;
    std::ofstream g;
    f.open(file_name.c_str(), std::ios::binary);
    g.open(("Zip_" + file_name).c_str(), std::ios::binary);

    int total_nodes = huffman_tree.getTotalNodes();
    g.write(reinterpret_cast<const char *>(&total_nodes), sizeof(int));
    g.write(reinterpret_cast<const char *>(&total_no), sizeof(long long));
    auto handler = huffman_tree.getRoot();
    write_tree(&handler, g);
    std::unordered_map<char, std::string> encoding;
    if(handler.left == nullptr && handler.right == nullptr)
        encoding.insert({(unsigned char) handler.value, "0"});
    else
        encode(&handler, "", encoding);

    char ch, payload = 0;
    int bits_count = 0;
    while(true) {
        f.read(&ch, 1);
        if(f.eof()) break;
        auto combination = encoding.find((unsigned char) ch)->second;
        for(int i = 0; i < combination.size(); ++i) {
            payload <<= 1;
            bits_count++;
            if(combination[i] == '1')
                payload += 0x01;
            if(bits_count == 8) {
                bits_count = 0;
                g.write(&payload, 1);
                payload = 0;
            }
        }
    }
    if(bits_count) {
        payload <<= (8 - bits_count);
        g.write(&payload, 1);
    }

    f.close();
    g.close();
}

struct node Parser::read_tree(std::ifstream& f, int total_nodes) {
    total_nodes--;
    char value, leaf;
    f.read(&value, 1);
    f.read(&leaf, 1);
    std::stack<struct node*> stack;
    struct node root {nullptr, nullptr, value, 0}; 
    stack.push(&root);
    while(total_nodes > 0) {
        f.read(&value, 1);
        f.read(&leaf, 1);
        --total_nodes;
        struct node *newnode = new node(nullptr, nullptr, value, 0);
        struct node *node;
        while(!stack.empty()) {
            node = stack.top();
            if(node->right == nullptr)
                break;
            stack.pop();
        }
        if(node->left == nullptr)
            node->left = newnode;
        else
            node->right = newnode;
        if(leaf == 0)
            stack.push(newnode);
    }
    return root;
}

void Parser::read_binary_content() {
    std::ifstream f;
    std::ofstream g;
    f.open(file_name, std::ios::binary);
    g.open("Unzip_" + file_name, std::ios::binary);
    int total_nodes;
    f.read(reinterpret_cast<char *>(&total_nodes), sizeof(int));
    f.read(reinterpret_cast<char *>(&total_no), sizeof(long long));
    struct node root = read_tree(f, total_nodes);
    if(total_nodes == 1) {
        struct node *leftnode = new node(root.left, root.right, root.value, root.weight);
        root.left = leftnode;
    }

    Huffman huffman_tree;
    huffman_tree.setRoot(root);
    huffman_tree.setTotalNodes(total_nodes);

    char payload = 0, mask = 0x80;
    int bits_count = 0;
    struct node *current_node = &root;
    while(total_no > 0) {
        if(bits_count == 0) {
            f.read(&payload, 1);
            bits_count = 8;
        }
        if((mask & payload) == 0)
            current_node = current_node->left;
        else
            current_node = current_node->right;
        if(current_node->left == nullptr && current_node->right == nullptr) {
            g.write(&current_node->value, 1);
            current_node = &root;
            --total_no;
        }
        payload <<= 1;
        --bits_count;
    }
}
