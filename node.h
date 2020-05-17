#ifndef _NODE_OTHER_H_
#define _NODE_OTHER_H_

#include <sstream>

#include "rb_tree.h"

namespace TepNik
{

// Definition of the Node structure
template <typename Key_type, typename Value_type>
struct rb_tree<Key_type, Value_type>::Node
{
    // Key and value
    Key_type *key;
    Value_type *value;

    // Attribute of the Node that shows it's color:
    // black or red or it is list
    enum color {black, red, list};
    color node_color;

    // Nodes of parent, left child and right child
    Node *parent, *left_child, *right_child;
    // Constructor
    Node() : node_color{color::list},
             parent{nullptr},
             left_child{nullptr},
             right_child{nullptr},
             key{nullptr},
             value{nullptr} {}
    Node(Key_type key, Value_type value);
    Node(const Node &sec);
    Node(Node &&sec) noexcept;
    // Destructor
    ~Node();
    // Function that swaps Nodes
    void swap(Node &sec) noexcept;
    // Functions that check and set color of the Node
    bool is_red() const;
    bool is_black() const;
    bool is_list() const;
    void set_red();
    void set_black();
    void set_list();

    // Is equal operator
    bool operator==(const Node &sec) const;
    bool is_equal_sub_trees(const Node &sec) const;

    Node& operator=(const Node &sec);
    Node& operator=(Node &&sec) noexcept;
    Node& copy_sub_tree(const Node &sec);

    static void print_sub_tree(const std::string& prefix, Node *node,
                               bool is_left, bool is_there_right_brother,
                               int width, std::ostream &stream,
                               bool with_color);
};

// Functions that print Node
template<typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::Node::print_sub_tree(
    const std::string& prefix, Node *node,
    bool is_left, bool is_there_right_brother, int width,
    std::ostream &stream,
    bool with_color)
{
    if(node && !node->is_list())
    {
        std::stringstream line_1;
        line_1 << "key:" << *node->key; //<< ";parent:" << (node->parent && !node->parent->is_list() ? *node->parent->key : 0);
        std::stringstream line_2;
        line_2 << "value:" << *node->value;
        std::stringstream line_3;
        line_3 << "color:" << (node->is_black() ? "black" : "red");
        std::string line_1_str = line_1.str(),
                    line_2_str = line_2.str(),
                    line_3_str = line_3.str();

        // print the value of the node
        stream << prefix << (is_left ? '+' : 'L');
        for (int i = 0; i < width; ++i)
            stream << "-";
        stream << line_1_str << '\n';
        stream << prefix << (is_left && is_there_right_brother ? '|' : ' ');
        for (int i = 0; i < width; ++i)
            stream << " ";
        stream << line_2_str << '\n';
        if (with_color)
        {
            stream << prefix << (is_left && is_there_right_brother ? '|' : ' ');
            for (int i = 0; i < width; ++i)
                stream << " ";
            stream << line_3_str << '\n';
        }

        int new_width;
        if (with_color)
            new_width = std::max(line_1_str.size(),
                                 std::max(line_2_str.size(),
                                          line_3_str.size())
                                );
        else
            new_width = std::max(line_1_str.size(),
                                 line_2_str.size());

        std::string new_prefix = prefix +
                                 (is_left && is_there_right_brother ? "│" : " ") +
                                 std::string(width, ' ');

        if (node->right_child != nullptr &&
            !node->right_child->is_list())
            print_sub_tree(new_prefix, node->left_child, true, true, new_width, stream, with_color);
        else
            print_sub_tree(new_prefix, node->left_child, true, false, new_width, stream, with_color);
        print_sub_tree(new_prefix, node->right_child, false, false, new_width, stream, with_color);
    }
}

// Constructor
template <typename Key_type, typename Value_type>
rb_tree<Key_type, Value_type>::Node::Node(Key_type key, Value_type value) :
    key{new Key_type{std::move(key)}},
    value{new Value_type{std::move(value)}},
    left_child{new Node},
    right_child{new Node},
    node_color{color::red}
{
    left_child->parent = this;
    right_child->parent = this;
}

template <typename Key_type, typename Value_type>
bool rb_tree<Key_type, Value_type>::Node::operator==(const Node &sec) const
{
    if (is_list() && sec.is_list())
        return true;
    else if (!is_list() && !sec.is_list())
        return node_color == sec.node_color &&
               *key       == *sec.key       &&
               *value     == *sec.value;
    else
        return false;
}

template <typename Key_type, typename Value_type>
bool rb_tree<Key_type, Value_type>::Node::is_equal_sub_trees(const Node &sec) const
{
    if (*this == sec)
    {
        if (!this->is_list() && !sec.is_list())
            return left_child->is_equal_sub_trees(*sec.left_child) &&
                   right_child->is_equal_sub_trees(*sec.right_child);
        else if (this->is_list() && sec.is_list())
            return true;
        else
            return false;
    }
    else return false;
}

// Function that swaps Nodes
template<typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::Node::swap(Node &sec) noexcept
{
    using std::swap;

    swap(this->key, sec.key);
    swap(this->value, sec.value);
    swap(this->node_color, sec.node_color);
    swap(this->parent, sec.parent);
    swap(this->left_child, sec.left_child);
    swap(this->right_child, sec.right_child);
}

// Function that swaps Nodes
template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::swap(Node &first, Node &sec) noexcept
{
    first.swap(sec);
}

template <typename Key_type, typename Value_type>
rb_tree<Key_type, Value_type>::Node::~Node()
{
    delete left_child;
    delete right_child;
    delete key;
    delete value;
}

template <typename Key_type, typename Value_type>
rb_tree<Key_type, Value_type>::Node::Node(const Node &sec)
{
    node_color = sec.node_color;
    parent = nullptr;
    if (!is_list())
    {
        key = new Key_type{*sec.key};
        value = new Value_type{*sec.value};
        left_child = new Node();
        right_child = new Node();
    }
    else
    {
        key = nullptr;
        value = nullptr;
        left_child = nullptr;
        right_child = nullptr;
    }
}

template <typename Key_type, typename Value_type>
rb_tree<Key_type, Value_type>::Node::Node(Node &&sec) noexcept
{
    key         = sec.key;
    value       = sec.value;
    node_color  = sec.node_color;
    parent      = sec.parent;
    left_child  = sec.left_child;
    right_child = sec.right_child;

    sec.key         = nullptr;
    sec.value       = nullptr;
    sec.parent      = nullptr;
    sec.left_child  = nullptr;
    sec.right_child = nullptr;
}

template <typename Key_type, typename Value_type>
typename rb_tree<Key_type, Value_type>::Node&
    rb_tree<Key_type, Value_type>::Node::operator=(const Node &sec)
{
    if (this != &sec)
    {
        Node temp(sec);
        this->swap(temp);
    }
    return *this;
}

template <typename Key_type, typename Value_type>
typename rb_tree<Key_type, Value_type>::Node&
    rb_tree<Key_type, Value_type>::Node::operator=(Node &&sec) noexcept
{
    if (this != &sec)
    {
        Node temp(std::move(sec));
        this->swap(temp);
    }
    return *this;
}

template <typename Key_type, typename Value_type>
typename rb_tree<Key_type, Value_type>::Node&
    rb_tree<Key_type, Value_type>::Node::copy_sub_tree(const Node &sec)
{
    if (this != &sec)
    {
        Node temp(sec);
        if (sec.left_child != nullptr)
        {
            delete temp.left_child;
            temp.left_child = new Node;
            temp.left_child->copy_sub_tree(*sec.left_child);
        }
        if (sec.right_child != nullptr)
        {
            delete temp.right_child;
            temp.right_child = new Node;
            temp.right_child->copy_sub_tree(*sec.right_child);
        }
        temp.parent = sec.parent;
        this->swap(temp);
    }
    return *this;
}

template <typename Key_type, typename Value_type>
bool rb_tree<Key_type, Value_type>::Node::is_red() const
{
    return node_color == color::red;
}

template <typename Key_type, typename Value_type>
bool rb_tree<Key_type, Value_type>::Node::is_black() const
{
    return node_color != color::red;
}

template <typename Key_type, typename Value_type>
bool rb_tree<Key_type, Value_type>::Node::is_list() const
{
    return node_color == color::list;
}

template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::Node::set_red()
{
    node_color = color::red;
}

template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::Node::set_black()
{
    node_color = color::black;
}

template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::Node::set_list()
{
    delete left_child;
    delete right_child;
    left_child  = nullptr;
    right_child = nullptr;

    delete key;
    delete value;
    key   = nullptr;
    value = nullptr;

    node_color = color::list;
}

};

#endif