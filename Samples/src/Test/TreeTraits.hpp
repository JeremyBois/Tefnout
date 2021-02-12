#ifndef __TREETRAITS__HPP
#define __TREETRAITS__HPP

#include <iostream>
#include <type_traits>

namespace Tree
{

/*
Based on article at https://dont.tech/2017/09/28/c-traits/
(Note) Some typos in the article for example for left and right node

Differences ?
This version is generic on the Node value and the Node type

Why ?
Could be useful for the engine
*/

// Trait declaration
template <typename T> struct NodeTraits
{
    static const bool isStoringDepth = false;
};

// Node using defaut trait
template <typename T> class SimpleNode
{
  public:
    SimpleNode() : left(nullptr), right(nullptr), value(0)
    {
    }
    SimpleNode *left;
    SimpleNode *right;
    T value;
};

// Node with specialized trait
template <typename T> class NodeWithDepth
{
  public:
    NodeWithDepth() : left(nullptr), right(nullptr), value(0), depth(1)
    {
    }
    NodeWithDepth *left;
    NodeWithDepth *right;
    T value;
    std::size_t depth;
};

// Trait specialization for NodeWithDepth
template <typename T> struct NodeTraits<NodeWithDepth<T>>
{
    static const bool isStoringDepth = true;
};

// A generic BinaryTree that accept different node type (Node) and value type (T)
template <template <typename> typename Node, typename T> class BST
{
  public:
    BST() : head(nullptr)
    {
    }

    // Interface hiden Node creation from user
    void Add(const T _value)
    {
        // Function selected at compilation time, no overhead
        AddImpl<NodeTraits<Node<T>>::isStoringDepth>(&head, _value);
    }

  private:
    Node<T> *head;

    // Concrete implementation of Add based on `isStoringDepth`
    template <bool isStoringDepth> void AddImpl(Node<T> **_head, const T _value);
};

// Using a wrapper struct to be able to specialize AddImpl
// More at https://stackoverflow.com/a/10178791
template <template <typename> typename Node, typename T, bool isStoringDepth> struct BSTHelper
{
    // Static is required because function selected at compile time
    static void AddImpl(Node<T> **_head, T _value)
    {
        std::cout << "Using depth = " << isStoringDepth << " - "
                  << "Value added = " << _value << std::endl;
        if (*_head == nullptr)
        {
            auto *node = new Node<T>();
            node->left = nullptr;
            node->right = nullptr;
            node->value = _value;
            *_head = node;
        }
        else
        {
            if ((*_head)->value < _value)
            {
                BSTHelper<Node, T, isStoringDepth>::AddImpl(&((*_head)->right), _value);
            }
            else
            {
                BSTHelper<Node, T, isStoringDepth>::AddImpl(&((*_head)->left), _value);
            }
        }
    }
};

template <template <typename> typename Node, typename T> struct BSTHelper<Node, T, true>
{
    // Static is required because function selected at compile time
    static void AddImpl(Node<T> **_head, T _value)
    {
        std::cout << "Using depth = " << true << " - "
                  << "Value added = " << _value << std::endl;
        if (*_head == nullptr)
        {
            auto *node = new Node<T>();
            node->value = _value;
            *_head = node;
        }
        else
        {
            if ((*_head)->value < _value)
            {
                BSTHelper<Node, T, true>::AddImpl(&((*_head)->right), _value);
                (*_head)->depth = (*_head)->right->depth + 1;
            }
            else
            {
                BSTHelper<Node, T, true>::AddImpl(&((*_head)->left), _value);
                (*_head)->depth = (*_head)->left->depth + 1;
            }
        }
    }
};

// Concrete implementation using specialization or defaut based
// on `isStoringDepth`
template <template <typename> typename Node, typename T> // Needed for BST
template <bool isStoringDepth>                           // Needed for Add
void BST<Node, T>::AddImpl(Node<T> **_head, const T _value)
{
    BSTHelper<Node, T, isStoringDepth>::AddImpl(_head, _value);
}

} // namespace Tree
#endif
