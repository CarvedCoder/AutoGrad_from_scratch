#include <iostream>
#include <vector>
#include "class.h"

int main(void){
    using namespace NodeOps;
    std::vector<int> Nodes;
    auto a1 = Node::CreateNode(3.14);
    auto a2 = Node::CreateNode(1.12);
    auto b1 = a1+a2;
    auto b2 = Node::CreateNode(2.33);
    auto c1 = b1-b2;
    auto c2 = Node::CreateNode(1.12);
    auto c3 = Node::CreateNode(1.13);
    auto d1 = c3 * c2;
    auto d2 = Node::CreateNode(1.18);
    auto e = d1/d2;
    auto neg_e = -e; 
    auto order = Node::topoSort(neg_e);
    
    Node::forward(order);
    std::cout << "Finished forward pass" << std::endl;
    std::cout << "Node data : " << neg_e->GetData() << std::endl;
}

