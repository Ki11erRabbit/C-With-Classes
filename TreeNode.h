#include "Node.h"
#include <stdlib.h>
#include "stdio.h"

class TreeNode /*implements Node*/ {
    size_t numChildren;
    TreeNode **children;

    TreeNode TreeNode(void *data, size_t elemSize, size_t numChild) {
        TreeNode node;
        node.numChildren = numChild;
        node.children = (TreeNode**)malloc(sizeof(TreeNode*) * numChild);
        for (size_t i = 0; i < numChild; i++)
            node.children[i] = NULL;
        return node;
    }
    TreeNode *newTreeNode(void *data, size_t elemSize, size_t numChild) {
        TreeNode *node;
        node->numChildren = numChild;
        node->children = (TreeNode**)malloc(sizeof(TreeNode*) * numChild);
        for (size_t i = 0; i < numChild; i++)
            node->children[i] = NULL;
        return node;
    }
    void _TreeNode(TreeNode *node) {
        free(node->children);
    }
    void _newTreeNode(TreeNode *node) {
        free(node->children);
        free(node);
    }

    TreeNode getLeft() {
        return *this->children[0];
    }
    TreeNode getRight() {
        return *this->children[this->numChildren-1];
    }
    TreeNode get(size_t index) {
        if (index > this->numChildren){
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        return *this->children[index];
    }

    void setLeft(TreeNode *node) {
        this->children[0] = node;
    }
    void setRight(TreeNode *node) {
        this->children[this->numChildren-1] = node;
    }
    void set(TreeNode *node, size_t index) {
        if (index > this->numChildren){
            fprintf(stderr, "%s", "Error: out of bounds");
            exit(1);
        }
        this->children[index] = node;
    }
};