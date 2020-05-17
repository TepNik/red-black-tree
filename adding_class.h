#ifndef _ADDING_CLASS_H_
#define _ADDING_CLASS_H_

#include "rb_tree.h"

namespace TepNik
{

// Definition of the adding_class
template <typename Key_type, typename Value_type>
class rb_tree<Key_type, Value_type>::adding_class
{
private:
    // Function that does restructuring of the tree
    // after adding new Node in it.
    void restructuring_adding(Node *node, Node **root);

    // Check if it is case 1 for restructoring after adding element.
    // More information in definition of this function.
    bool is_case_1_adding(const Node *node,
                          const Node *parent,
                          const Node *grand_parent,
                          const Node *uncle) const;
    // Check if it is case 2 for restructoring after adding element.
    // More information in definition of this function.
    bool is_case_2_adding(const Node *node,
                          const Node *parent,
                          const Node *grand_parent,
                          const Node *uncle) const;
    // Check if it is case 3 for restructoring after adding element.
    // More information in definition of this function.
    bool is_case_3_adding(const Node *node,
                          const Node *parent,
                          const Node *grand_parent,
                          const Node *uncle) const;

    // Make restructuring of the tree for case 1 after adding element
    // More information in definition of this function
    void do_case_1_adding(Node *node,
                          Node *parent,
                          Node *grand_parent,
                          Node *uncle,
                          Node **root);
    // Make restructuring of the tree for case 2 after adding element
    // More information in definition of this function
    void do_case_2_adding(Node *node,
                          Node *parent,
                          Node *grand_parent,
                          Node *uncle,
                          Node **root);
    // Make restructuring of the tree for case 3 after adding element
    // More information in definition of this function
    void do_case_3_adding(Node *node,
                          Node *parent,
                          Node *grand_parent,
                          Node *uncle,
                          Node **root);

public:
    void insert(Key_type key,
                Value_type value,
                rb_tree<Key_type, Value_type> &tree);
};

// Function that does insertion of the Node in the tree.
template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::adding_class::insert(
    Key_type key,
    Value_type value,
    rb_tree<Key_type, Value_type> &tree)
{
    Node *node = find_place_for(key, tree.root);

    if (node->is_list())
    {
        delete node->left_child;
        node->left_child = new Node;
        node->left_child->parent = node;
        delete node->right_child;
        node->right_child = new Node;
        node->right_child->parent = node;

        node->set_red();
        node->key   = new Key_type{std::move(key)};
        node->value = new Value_type{std::move(value)};
        ++tree.number_of_elements;
        restructuring_adding(node, &tree.root);
    }
    else
        *node->value = std::move(value);
}

// Function that does restructuring of the tree
// after adding new Node in it
template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::adding_class::restructuring_adding(
    Node *node,
    Node **root)
{
    if (node == *root)
    {
        node->set_black();
        return;
    }
    Node *parent = node->parent;
    Node *grand_parent = nullptr;
    if (parent)
        grand_parent = parent->parent;
    if (grand_parent == nullptr)
        return;

    Node *uncle;
    direction parent_status = left_or_right_node(parent);
    if (parent_status == direction::left)
        uncle = grand_parent->right_child;
    else
        uncle = grand_parent->left_child;
    if (is_case_1_adding(node, parent, grand_parent, uncle))
        do_case_1_adding(node, parent, grand_parent, uncle, root);
    else if (is_case_2_adding(node, parent, grand_parent, uncle))
        do_case_2_adding(node, parent, grand_parent, uncle, root);
    else if (is_case_3_adding(node, parent, grand_parent, uncle))
        do_case_3_adding(node, parent, grand_parent, uncle, root);
}

// Check if it is case 1 for restructoring after adding element. It is when
// 1) Node is red
// 2) Parent is red
// 3) Grand parent is black
// 4) Uncle is red
template <typename Key_type, typename Value_type>
bool rb_tree<Key_type, Value_type>::adding_class::is_case_1_adding(
    const Node *node,
    const Node *parent,
    const Node *grand_parent,
    const Node *uncle) const
{
    if (!node->is_black() &&
        !parent->is_black() &&
        !uncle->is_black() &&
        grand_parent->is_black())
        return true;
    return false;
}

// Check if it is case 2 for restructoring after adding element. It is when
// 1) Node is red and it is right child
// 2) Parent is red and it is left child
// 3) Grand parent is black
// 4) Uncle is black
// or
// 1) Node is red and it is left child
// 2) Parent is red and it is right child
// 3) Grand parent is black
// 4) Uncle is black
template <typename Key_type, typename Value_type>
bool rb_tree<Key_type, Value_type>::adding_class::is_case_2_adding(
    const Node *node,
    const Node *parent,
    const Node *grand_parent,
    const Node *uncle) const
{
    if (!node->is_black() &&
        !parent->is_black() &&
        uncle->is_black() &&
        grand_parent->is_black())
    {
        direction status_node = left_or_right_node(node);
        direction status_parent = left_or_right_node(parent);
        if (status_node != status_parent)
        {
            return true;
        }
    }
    return false;
}

// Check if it is case 3 for restructoring after adding element. It is when
// 1) Node is red and it is left child
// 2) Parent is red and it is left child
// 3) Grand parent is black
// 4) Uncle is black
// or
// 1) Node is red and it is right child
// 2) Parent is red and it is right child
// 3) Grand parent is black
// 4) Uncle is black
template <typename Key_type, typename Value_type>
bool rb_tree<Key_type, Value_type>::adding_class::is_case_3_adding(
    const Node *node,
    const Node *parent,
    const Node *grand_parent,
    const Node *uncle) const
{
    if (!node->is_black() &&
        !parent->is_black() &&
        uncle->is_black() &&
        grand_parent->is_black())
    {
        direction status_node = left_or_right_node(node);
        direction status_parent = left_or_right_node(parent);
        if (status_node == status_parent)
            return true;
    }
    return false;
}

// Make restructuring of the tree for case 1 after adding element. It is when
// 1) Node is red
// 2) Parent is red
// 3) Grand parent is black
// 4) Uncle is red
// Need to do:
// 1) Color parent and uncle in black
// 2) Color grand parent in red
// 3) Call restructuring for grand parent
template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::adding_class::do_case_1_adding(
    Node *node,
    Node *parent,
    Node *grand_parent,
    Node *uncle,
    Node **root)
{
    parent->set_black();
    uncle->set_black();
    grand_parent->set_red();
    restructuring_adding(grand_parent, root);
}

// Make restructuring of the tree for case 2 after adding element. It is when
// 1) Node is red and it is right child
// 2) Parent is red and it is left child
// 3) Grand parent is black
// 4) Uncle is black
// or
// 1) Node is red and it is left child
// 2) Parent is red and it is right child
// 3) Grand parent is black
// 4) Uncle is black
// Need to do:
// This case is similar to 3 case.
// Here we transfer this 2 sub cases in 2 sub cases of case 3
// After that we do case 3
template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::adding_class::do_case_2_adding(
    Node *node,
    Node *parent,
    Node *grand_parent,
    Node *uncle,
    Node **root)
{
    direction status_node = left_or_right_node(node);
    direction status_parent = left_or_right_node(parent);
    if (status_node == direction::right &&
        status_parent == direction::left)
    {
        grand_parent->left_child = node;
        parent->right_child = node->left_child;
        node->left_child = parent;
    }
    else if (status_node == direction::left &&
             status_parent == direction::right)
    {
        grand_parent->right_child = node;
        parent->left_child = node->right_child;
        node->right_child = parent;
    }
    node->parent = grand_parent;
    parent->parent = node;
    do_case_3_adding(parent, node, grand_parent, uncle, root);
}

// Make restructuring of the tree for case 3 after adding element. It is when
// 1) Node is red and it is left child
// 2) Parent is red and it is left child
// 3) Grand parent is black
// 4) Uncle is black
// or
// 1) Node is red and it is right child
// 2) Parent is red and it is right child
// 3) Grand parent is black
// 4) Uncle is black
// Need to do:
// 1) Rotate to the right and to the left respectively
// 2) Color parent to balck
// 3) Color grand parent to red
template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::adding_class::do_case_3_adding(
    Node *node,
    Node *parent,
    Node *grand_parent,
    Node *uncle,
    Node **root)
{
    direction status_node = left_or_right_node(node);
    direction status_parent = left_or_right_node(parent);
    Node *gr_gr_parent = grand_parent->parent;
    if (gr_gr_parent != nullptr)
    {
        direction status_gr_p = left_or_right_node(grand_parent);
        if (status_gr_p == direction::left)
            gr_gr_parent->left_child = parent;
        else if (status_gr_p == direction::right)
            gr_gr_parent->right_child = parent;
    }

    if (status_node == direction::left &&
        status_parent == direction::left)
    {
        parent->right_child->parent = grand_parent;
        grand_parent->left_child = parent->right_child;
        parent->right_child = grand_parent;

        parent->set_black();
        grand_parent->set_red();
    }
    else if (status_node == direction::right &&
             status_parent == direction::right)
    {
        parent->left_child->parent = grand_parent;
        grand_parent->right_child = parent->left_child;
        parent->left_child = grand_parent;

        parent->set_black();
        grand_parent->set_red();
    }
    parent->parent = grand_parent->parent;
    grand_parent->parent = parent;
    if (*root == grand_parent)
        *root = parent;
}

}; // namespace TepNik

#endif