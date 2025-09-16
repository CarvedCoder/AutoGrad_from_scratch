#include <iostream>
#include <vector>
#include "class.h"

int main(void){
    std::vector<int> Nodes;
    auto a1 = Node::CreateNode(3.14);
    auto a2 = Node::CreateNode(1.12);
    auto b1 = Node::add(a1, a2);
    auto b2 = Node::CreateNode(2.33);
    auto c1 = Node::diff(b1, b2);
    auto c2 = Node::CreateNode(1.12);
    auto d1 = Node::mul(c1,c2);
    auto d2 = Node::CreateNode(1.18);
    auto e = Node::div(d1,d2);

    auto sorted = Node::topoSort(e);

    for (auto node : sorted){
        std::cout << node << " ";
    }
}



