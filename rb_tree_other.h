#ifndef _RB_TREE_OTHER_H_
#define _RB_TREE_OTHER_H_

#include <cmath>

#include "rb_tree.h"

namespace TepNik
{

// Function that finds node with this key
// or list where this key should be
template<typename Key_type, typename Value_type>
typename rb_tree<Key_type, Value_type>::Node*
    rb_tree<Key_type, Value_type>::find_place_for(const Key_type &key, Node *node)
{
    if (node->is_list() || *node->key == key)
        return node;
    else if (*node->key > key)
        return find_place_for(key, node->left_child);
    else
        return find_place_for(key, node->right_child);
}

// Functions that print tree
template<typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::print(std::ostream &stream, bool with_color) const
{
    Node::print_sub_tree("", root, false, false, 2, stream, with_color);
}

// Copy assignment operator
template<typename Key_type, typename Value_type>
rb_tree<Key_type, Value_type>&
    rb_tree<Key_type, Value_type>::operator=(const rb_tree &sec)
{
    if (this != &sec)
    {
        rb_tree temp(sec);
        this->swap(temp);
    }
    return *this;
}

// Move assignment operator
template<typename Key_type, typename Value_type>
rb_tree<Key_type, Value_type>&
    rb_tree<Key_type, Value_type>::operator=(rb_tree &&sec) noexcept
{
    if (this != &sec)
    {
        rb_tree temp(std::move(sec));
        this->swap(temp);
    }
    return *this;
}

// Equal to operator
template<typename Key_type, typename Value_type>
bool rb_tree<Key_type, Value_type>::operator==(const rb_tree &sec) const
{
    if (number_of_elements != sec.number_of_elements)
        return false;
    return root->is_equal_sub_trees(*sec.root);
}

// Copy constructor
template<typename Key_type, typename Value_type>
rb_tree<Key_type, Value_type>::rb_tree(const rb_tree &sec)
{
    number_of_elements = sec.number_of_elements;
    root = new Node;
    root->copy_sub_tree(*sec.root);
}

// Move constructor
template<typename Key_type, typename Value_type>
rb_tree<Key_type, Value_type>::rb_tree(rb_tree &&sec) noexcept
{
    number_of_elements = sec.number_of_elements;
    root               = sec.root;
    sec.root           = nullptr;
}

// Destructor
template<typename Key_type, typename Value_type>
rb_tree<Key_type, Value_type>::~rb_tree()
{
    delete root;
}

// Function that find out what direction has Node.
template<typename Key_type, typename Value_type>
typename rb_tree<Key_type, Value_type>::direction
    rb_tree<Key_type, Value_type>::left_or_right_node(const Node *node)
{
    if (node->parent == nullptr)
        return direction::this_is_root;
    else if (node->parent->left_child == node)
        return direction::left;
    else
        return direction::right;
}

// Function that swaps trees
template<typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::swap(rb_tree &sec) noexcept
{
    using std::swap;

    swap(this->number_of_elements, sec.number_of_elements);
    swap(this->root, sec.root);
}

// Function that swaps trees
template <typename Key_type, class Value_type>
void swap(rb_tree<Key_type, Value_type> &first,
          rb_tree<Key_type, Value_type> &sec) noexcept
{
    first.swap(sec);
}

// Function that does insertion of the Node in the tree
template <typename Key_type, class Value_type>
void rb_tree<Key_type, Value_type>::insert(Key_type key,
                                           Value_type value)
{
    adder.insert(std::move(key),
                 std::move(value),
                 *this);
}

template <typename Key_type, class Value_type>
void rb_tree<Key_type, Value_type>::erase(const Key_type &key)
{
    eraser.erase(key, *this);
}

template <typename Key_type, class Value_type>
const Value_type* rb_tree<Key_type, Value_type>::find(const Key_type &key) const
{
    const Node* node = find_place_for(key, root);
    assert(node != nullptr);
    if (node->is_list())
        return nullptr;
    else
        return node->value;
}

template <typename Key_type, class Value_type>
Value_type* rb_tree<Key_type, Value_type>::find(const Key_type &key)
{
    return const_cast<Value_type*>(std::as_const(*this).find(key));
}

template <typename Key_type, class Value_type>
void rb_tree<Key_type, Value_type>::clear()
{
    delete root;
    root = new Node;
    number_of_elements = 0;
}

template <typename Key_type, class Value_type>
std::vector<Key_type> rb_tree<Key_type, Value_type>::get_keys() const
{
    std::vector<Key_type> result;
    result.reserve(number_of_elements);
    _get_keys(root, result);
    return result;
}

template <typename Key_type, class Value_type>
void rb_tree<Key_type, Value_type>::_get_keys(const Node *node, std::vector<Key_type> &result) const
{
    if (node == nullptr || node->is_list())
        return;
    _get_keys(node->left_child, result);
    result.push_back(*node->key);
    _get_keys(node->right_child, result);
}

template <typename Key_type, class Value_type>
std::vector<Value_type> rb_tree<Key_type, Value_type>::get_values() const
{
    std::vector<Key_type> result;
    result.reserve(number_of_elements);
    _get_values(root, result);
    return result;
}

template <typename Key_type, class Value_type>
void rb_tree<Key_type, Value_type>::_get_values(const Node *node, std::vector<Value_type> &result) const
{
    if (node == nullptr || node->is_list())
        return;
    _get_values(node->left_child, result);
    result.push_back(*node->value);
    _get_values(node->right_child, result);
}

};

#endif