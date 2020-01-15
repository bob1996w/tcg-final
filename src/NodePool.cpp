#include "NodePool.hpp"
#include "TreeNode.hpp"

std::stack<TreeNode*> NodePool::pool;

TreeNode* NodePool::alloc() {
    if (isEmpty()) {
        return new TreeNode();
    }
    else {
        TreeNode* tmp = pool.top();
        pool.pop();
        return tmp;
    }
}

bool NodePool::isEmpty() {
    return pool.empty();
}

void NodePool::free(TreeNode* node) {
    if (node == nullptr) { return; }
    push(node);
    if (node->numChild != 0) {
        for (int i = 0; i < node->numChild; ++i) {
            free(node->child[i]);
        }
        node->numChild = 0;
    }
}

void NodePool::push(TreeNode* node) {
    pool.push(node);
}