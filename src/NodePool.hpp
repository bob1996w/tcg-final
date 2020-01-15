#pragma once
#include <stack>

class TreeNode;

class NodePool {
public:
    static std::stack<TreeNode*> pool;
    static TreeNode* alloc();
    static bool isEmpty();
    static void free(TreeNode* node);
private:
    static void push(TreeNode* node);
};