#include <iostream>

#include "rb_tree.h"

int main()
{
    TepNik::rb_tree<int, int> tree;
    std::cout << "Tree:\n";
    for (int i = 1; i <= 10; ++i)
        tree.insert(i, 2);
    tree.print();
    tree.erase(7);
    tree.erase(8);
    tree.erase(9);
    std::cout << "Tree after erasing 7, 8, 9:\n";
    tree.print();
    return 0;
}