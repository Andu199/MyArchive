struct node {
    struct node *left;
    struct node *right;
    char value;
    double weight;

    node(struct node* _left, struct node* _right,
        char _value, double _weight) : 
        left{_left}, right{_right}, value{_value}, weight{_weight} { }
};

struct CompareTree {
    bool operator()(struct node& n1, struct node& n2)
    {
      return n1.weight > n2.weight;
    }
};