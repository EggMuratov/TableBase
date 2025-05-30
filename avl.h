#include <iostream>
#include <algorithm>
#include <utility>
#include <stdexcept>

namespace TREE
{

    template <typename TKey, typename TVal>
    class Node
    {
    public:
        using DataType = std::pair<TKey, TVal>;

        Node *left = nullptr;
        Node *right = nullptr;
        Node *parent = nullptr;
        DataType data;

        Node(const DataType &d = {}) : data(d) {}
        virtual ~Node() = default;
    };

    template <typename TKey, typename TVal>
    class AVLNode : public Node<TKey, TVal>
    {
    public:
        int height = 0;

        AVLNode(const typename Node<TKey, TVal>::DataType &d = {}) : Node<TKey, TVal>(d) {}
    };

    template <typename TKey, typename TVal, typename NodeType = Node<TKey, TVal>>
    class Tree
    {
    public:
        using NodePtr = NodeType *;

        NodePtr root = nullptr;

        virtual ~Tree() { deleteTree(root); }

        bool isEmpty() const { return root == nullptr; }

        NodePtr search(const TKey &key) const
        {
            NodePtr current = root;
            while (current && current->data.first != key)
            {
                current = key < current->data.first ? current->left : current->right;
            }
            if (!current)
                throw std::runtime_error("Node not found");
            return current;
        }

        NodePtr insert(NodePtr node, const TKey &key)
        {
            if (!node)
            {
                NodePtr newNode = new NodeType({key, TVal()});
                if (!root)
                    root = newNode;
                return newNode;
            }

            if (key < node->data.first)
            {
                node->left = insert(node->left, key);
                node->left->parent = node;
            }
            else if (key > node->data.first)
            {
                node->right = insert(node->right, key);
                node->right->parent = node;
            }
            return node;
        }

        void remove(const TKey &key)
        {
            root = removeNode(root, key);
        }

        void walk(NodePtr node) const
        {
            if (!node)
                return;
            walk(node->left);
            std::cout << "Key: " << node->data.first;
            if (node->data.second != TVal())
                std::cout << ", Value: " << node->data.second;
            std::cout << "\n";
            walk(node->right);
        }

    protected:
        void deleteTree(NodePtr node)
        {
            if (!node)
                return;
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }

        NodePtr min(NodePtr node) const
        {
            while (node && node->left)
                node = node->left;
            return node;
        }

        NodePtr removeNode(NodePtr node, const TKey &key)
        {
            if (!node)
                return nullptr;

            if (key < node->data.first)
            {
                node->left = removeNode(node->left, key);
            }
            else if (key > node->data.first)
            {
                node->right = removeNode(node->right, key);
            }
            else
            {
                if (!node->left)
                {
                    NodePtr temp = node->right;
                    delete node;
                    return temp;
                }
                else if (!node->right)
                {
                    NodePtr temp = node->left;
                    delete node;
                    return temp;
                }
                NodePtr successor = min(node->right);
                node->data = successor->data;
                node->right = removeNode(node->right, successor->data.first);
            }
            return node;
        }
    };

    template <typename TKey, typename TVal>
    class AVLTree : public Tree<TKey, TVal, AVLNode<TKey, TVal>>
    {
        using Base = Tree<TKey, TVal, AVLNode<TKey, TVal>>;
        using NodePtr = typename Base::NodePtr;

    public:
        NodePtr insert(NodePtr node, const TKey &key) override
        {
            if (!node)
            {
                NodePtr newNode = new AVLNode<TKey, TVal>({key, TVal()});
                if (!this->root)
                    this->root = newNode;
                return newNode;
            }

            if (key < node->data.first)
            {
                node->left = insert(node->left, key);
                node->left->parent = node;
            }
            else if (key > node->data.first)
            {
                node->right = insert(node->right, key);
                node->right->parent = node;
            }
            else
            {
                return node;
            }

            updateHeight(node);
            return balance(node);
        }

        void remove(const TKey &key) override
        {
            this->root = removeNode(this->root, key);
        }

        void walk(NodePtr node) const override
        {
            if (!node)
                return;
            walk(node->left);
            std::cout << "Key: " << node->data.first << ", Height: " << node->height << "\n";
            walk(node->right);
        }

    private:
        int getHeight(NodePtr node) const
        {
            return node ? node->height : -1;
        }

        void updateHeight(NodePtr node)
        {
            int lh = getHeight(node->left);
            int rh = getHeight(node->right);
            node->height = 1 + std::max(lh, rh);
        }

        int getBalance(NodePtr node) const
        {
            return getHeight(node->left) - getHeight(node->right);
        }

        NodePtr rotateLeft(NodePtr x)
        {
            NodePtr y = x->right;
            x->right = y->left;
            if (y->left)
                y->left->parent = x;
            y->left = x;
            y->parent = x->parent;
            x->parent = y;
            updateHeight(x);
            updateHeight(y);
            return y;
        }

        NodePtr rotateRight(NodePtr y)
        {
            NodePtr x = y->left;
            y->left = x->right;
            if (x->right)
                x->right->parent = y;
            x->right = y;
            x->parent = y->parent;
            y->parent = x;
            updateHeight(y);
            updateHeight(x);
            return x;
        }

        NodePtr balance(NodePtr node)
        {
            int bal = getBalance(node);
            if (bal > 1)
            {
                if (getBalance(node->left) < 0)
                    node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            else if (bal < -1)
            {
                if (getBalance(node->right) > 0)
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            return node;
        }

        NodePtr removeNode(NodePtr node, const TKey &key)
        {
            if (!node)
                return nullptr;

            if (key < node->data.first)
            {
                node->left = removeNode(node->left, key);
            }
            else if (key > node->data.first)
            {
                node->right = removeNode(node->right, key);
            }
            else
            {
                if (!node->left || !node->right)
                {
                    NodePtr temp = node->left ? node->left : node->right;
                    delete node;
                    return temp;
                }
                NodePtr successor = this->min(node->right);
                node->data = successor->data;
                node->right = removeNode(node->right, successor->data.first);
            }

            updateHeight(node);
            return balance(node);
        }
    };

} 
