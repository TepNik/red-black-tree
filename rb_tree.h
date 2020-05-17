#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include <utility>
#include <string>
#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>

//#define DEBUG

namespace TepNik
{

template<typename Key_type, typename Value_type>
class rb_tree
{
private:
    // Structure for incapsulation of
    // key and value of the Node
    // and also for it's color
    struct Node;
    // Function that swap Nodes
    static void swap(Node &first, Node &sec) noexcept;

    // number_of_elements -- number of elements in array
    int number_of_elements;
    // Pointer to root
    Node *root;

    // Fttribute of Node that shows which child it is
    // left or right (relative to it's parent)
    // or root Node.
    enum direction {left, right, this_is_root};
    // Function that find out what direction has Node.
    static direction left_or_right_node(const Node *node);

    // Class that implement adding in the tree
    class adding_class;
    inline static adding_class adder;

    // Class that implement erasing in the tree
    class erasing_class;
    inline static erasing_class eraser;

    // Function that finds node with this key
    // or list where this key should be
    static Node *find_place_for(const Key_type &key, Node *node);

    // Function that helps function get_keys
    void _get_keys(const Node *node, std::vector<Key_type> &result) const;
    // Function that helps function get_values
    void _get_values(const Node *node, std::vector<Value_type> &result) const;
public:
    // Function that does insertion of the Node in the tree
    void insert(Key_type key,
                Value_type value);
    void erase(const Key_type &key);

    //Value_type& operator[](Key_type key);

    // Constructors
    rb_tree() : root{new Node},
                number_of_elements{0} {};
    // Copy and move constructors
    rb_tree(const rb_tree &sec);
    rb_tree(rb_tree &&sec) noexcept;
    // Destructor
    ~rb_tree();

    // Assignment operators (copy and move)
    rb_tree& operator=(const rb_tree &sec);
    rb_tree& operator=(rb_tree &&sec) noexcept;

    // Equal to operator
    bool operator==(const rb_tree &sec) const;

    // Function that finds element with this key and
    // return pointer to its value
    // if there is no element with this key
    // return nullptr
    const Value_type* find(const Key_type &key) const;
    Value_type* find(const Key_type &key);

    // Function that cleans container
    void clear();

    std::vector<Key_type> get_keys() const;
    std::vector<Value_type> get_values() const;

    // Functions that print tree
    void print(std::ostream &stream = std::cout, bool with_color = false) const;

    // Function that swaps trees
    void swap(rb_tree &sec) noexcept;

    int size() const {return number_of_elements;}
};

};

#include "node.h"
#include "adding_class.h"
#include "erasing_class.h"
#include "rb_tree_other.h"

#ifdef DEBUG
// Check if all functions can be generated for <int, int> case
template class TepNik::rb_tree<int, int>;
// Check if all functions can be generated for <std::string, std::string> case
template class TepNik::rb_tree<std::string, std::string>;
#endif

#endif