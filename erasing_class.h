#ifndef _ERASING_CLASS_H_
#define _ERASING_CLASS_H_

#include "rb_tree.h"

namespace TepNik
{

// Definition of the erasing_class
template <typename Key_type, typename Value_type>
class rb_tree<Key_type, Value_type>::erasing_class
{
private:
    void stage_2(Node *replacement,
                 Node *x,
                 typename Node::color deleted_color,
                 typename Node::color replacement_color,
                 rb_tree<Key_type, Value_type> &tree);
    void stage_3(Node *node, rb_tree<Key_type, Value_type> &tree);

    void rotate_left(Node *node, rb_tree<Key_type, Value_type> &tree);
    void rotate_right(Node *node, rb_tree<Key_type, Value_type> &tree);

    Node* get_replacement(Node *node);
    Node* get_sibling(Node *node);

public:
    void erase(const Key_type &key,
               rb_tree<Key_type, Value_type> &tree);
};

template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::erasing_class::erase(
    const Key_type &key,
    rb_tree<Key_type, Value_type> &tree)
{
    Node *node = find_place_for(key, tree.root);

    if (node->is_list())
        return;

    typename Node::color deleted_color, replacement_color;
    deleted_color = node->node_color;

    Node *x;
    if (node->left_child->is_list() &&
        node->right_child->is_list())
    {
        replacement_color = Node::color::list;
        node->set_list();
        x = node;
        stage_2(node, x, deleted_color, replacement_color, tree);
    }
    else if (node->left_child->is_list() &&
             !node->right_child->is_list())
    {
        replacement_color = node->right_child->node_color;
        x = node->right_child;
        Node *temp = node->right_child;
        /* std::cout << "node = " << *node->key << '\n';
        std::cout << "temp = " << (temp->is_list() ? 0 : *temp->key) << '\n'; */

        direction node_direction = left_or_right_node(node);
        if (node_direction == direction::left)
            node->parent->left_child = temp;
        else if (node_direction == direction::right)
            node->parent->right_child = temp;
        else
            tree.root = temp;
        temp->parent = node->parent;

        node->right_child = nullptr;
        delete node;
        stage_2(temp, temp, deleted_color, replacement_color, tree);
    }
    else if (node->right_child->is_list() &&
             !node->left_child->is_list())
    {
        replacement_color = node->left_child->node_color;
        Node *temp = node->left_child;

        direction node_direction = left_or_right_node(node);
        if (node_direction == direction::left)
            node->parent->left_child = temp;
        else if (node_direction == direction::right)
            node->parent->right_child = temp;
        else
            tree.root = temp;
        temp->parent = node->parent;

        node->left_child = nullptr;
        delete node;
        stage_2(temp, temp, deleted_color, replacement_color, tree);
    }
    else
    {
        Node *replacement = get_replacement(node);
        assert(replacement != nullptr);
        assert(replacement->left_child->is_list());
        x = replacement->right_child;
        replacement_color = replacement->node_color;

        direction replacement_direction = left_or_right_node(replacement);
        if (replacement_direction == direction::left)
            replacement->parent->left_child = x;
        else if (replacement_direction == direction::right)
            replacement->parent->right_child = x;
        else
            tree.root = x;
        x->parent = replacement->parent;

        direction node_direction = left_or_right_node(node);
        if (node_direction == direction::left)
            node->parent->left_child = replacement;
        else if (node_direction == direction::right)
            node->parent->right_child = replacement;
        else
            tree.root = replacement;
        replacement->parent = node->parent;
        delete replacement->left_child;
        replacement->left_child = node->left_child;
        replacement->left_child->parent = replacement;
        replacement->right_child = node->right_child;
        replacement->right_child->parent = replacement;

        node->left_child = nullptr;
        node->right_child = nullptr;
        delete node;
        stage_2(replacement, x, deleted_color, replacement_color, tree);
    }

    --tree.number_of_elements;
}

template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::erasing_class::stage_2(
    Node *replacement,
    Node *x,
    typename Node::color deleted_color,
    typename Node::color replacement_color,
    rb_tree<Key_type, Value_type> &tree)
{
    if (deleted_color      == Node::color::red &&
        (replacement_color == Node::color::red ||
         replacement_color == Node::color::list))
    {
        return;
    }
    else if (deleted_color     == Node::color::red &&
             replacement_color == Node::color::black)
    {
        replacement->set_red();
        stage_3(x, tree);
    }
    else if (deleted_color     == Node::color::black &&
             replacement_color == Node::color::red)
    {
        replacement->set_black();
        return;
    }
    else if (deleted_color     == Node::color::black &&
             (replacement_color == Node::color::black ||
              replacement_color == Node::color::list))
    {
        stage_3(x, tree);
    }
}

template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::erasing_class::stage_3(
    Node *node,
    rb_tree<Key_type, Value_type> &tree)
{
    if (node->is_red())
    {
        node->set_black();
        return;
    }
    Node *sibling = get_sibling(node);
    if (node->is_black() && sibling->is_red())
    {
        sibling->set_black();
        node->parent->set_red();

        direction node_direction = left_or_right_node(node);
        if (node_direction == direction::left)
            rotate_left(node->parent, tree);
        if (node_direction == direction::right)
            rotate_right(node->parent, tree);
        assert(node_direction != direction::this_is_root);

        stage_3(node, tree);
        return;
    }
    else if (node->is_black() && sibling->is_black() &&
             sibling->left_child && sibling->left_child->is_black() &&
             sibling->right_child && sibling->right_child->is_black())
    {
        sibling->set_red();
        node = node->parent;
        assert(node != nullptr);
        if (node->is_red())
        {
            node->set_black();
            return;
        }
        else
        {
            stage_3(node, tree);
            return;
        }
    }
    direction node_direction = left_or_right_node(node);
    if (node->is_black() &&
        sibling->is_black() &&
        node_direction == direction::left &&
        sibling->left_child->is_red() &&
        sibling->right_child->is_black())
    {
        sibling->left_child->set_black();
        sibling->set_red();
        rotate_right(sibling, tree);
        sibling = node->parent->right_child;
    }
    else if (node->is_black() &&
             sibling->is_black() &&
             node_direction == direction::right &&
             sibling->right_child->is_red() &&
             sibling->left_child->is_black())
    {
        sibling->right_child->set_black();
        sibling->set_red();
        rotate_left(sibling, tree);
        sibling = node->parent->left_child;
    }

    if (node->is_black() && sibling->is_black() &&
        node_direction == direction::left &&
        sibling->right_child->is_red())
    {
        if (node->parent->is_black())
            sibling->set_black();
        else
            sibling->set_red();
        node->parent->set_black();
        sibling->right_child->set_black();
        rotate_left(node->parent, tree);
    }
    else if (node->is_black() && sibling->is_black() &&
             node_direction == direction::right &&
             sibling->left_child->is_red())
    {
        if (node->parent->is_black())
            sibling->set_black();
        else
            sibling->set_red();
        node->parent->set_black();
        sibling->left_child->set_black();
        rotate_right(node->parent, tree);
    }
}

template <typename Key_type, typename Value_type>
typename rb_tree<Key_type, Value_type>::Node*
    rb_tree<Key_type, Value_type>::erasing_class::get_replacement(Node *node)
{
    Node *result = node->right_child;
    while(result && !result->is_list() && !result->left_child->is_list())
        result = result->left_child;
    return result;
}

template <typename Key_type, typename Value_type>
typename rb_tree<Key_type, Value_type>::Node*
    rb_tree<Key_type, Value_type>::erasing_class::get_sibling(Node *node)
{
    direction node_direction = left_or_right_node(node);
    if (node_direction == direction::left)
        return node->parent->right_child;
    else if (node_direction == direction::right)
        return node->parent->left_child;
    assert(0);
}

template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::erasing_class::rotate_left(
    Node *node,
    rb_tree<Key_type, Value_type> &tree)
{
    assert(node);
    Node *left_child  = node->left_child,
         *right_child = node->right_child;

    direction node_direction = left_or_right_node(node);
    if (node_direction == direction::left)
        node->parent->left_child = right_child;
    else if (node_direction == direction::right)
        node->parent->right_child = right_child;
    else
        tree.root = right_child;
    right_child->parent = node->parent;

    node->right_child = right_child->left_child;
    right_child->left_child = node;
    node->parent = right_child;
}

template <typename Key_type, typename Value_type>
void rb_tree<Key_type, Value_type>::erasing_class::rotate_right(
    Node *node,
    rb_tree<Key_type, Value_type> &tree)
{
    Node *left_child  = node->left_child,
         *right_child = node->right_child;

    direction node_direction = left_or_right_node(node);
    if (node_direction == direction::left)
        node->parent->left_child = left_child;
    else if (node_direction == direction::right)
        node->parent->right_child = left_child;
    else
        tree.root = left_child;
    left_child->parent = node->parent;

    node->left_child = left_child->right_child;
    left_child->right_child = node;
    node->parent = left_child;
}

}; // namespace TepNik

#endif