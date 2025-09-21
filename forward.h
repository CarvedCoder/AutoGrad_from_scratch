#ifndef FORWARD_H
#define FORWARD_H
#include <cmath>
#include <iostream>
#include <ostream>
#include <string>
#include "node.h"
static void forward(const std::vector<int>&order){
    for(auto n:order){ 
        auto node = Node::GetNode(n);
        if (!node){
            std::cerr << "Node corruption (nullptr returned)" << std::endl;
            return;
        }
        auto parents = node->GetParents();
        if (node->GetOp() == "input") {continue;}
        else if (node->GetOp() == "+"){
            double result=0.0;
            for(auto pid : parents){
                auto parent = Node::GetNode(pid);
                if (!parent) {
                    std::cerr << "Parent node " << pid << " not found" << std::endl;
                    return;
                }
                result += parent->GetData();
            }
            node->SetData(result);
        }
        else if (node->GetOp() == "-"){
            if (parents.size() < 2) {
                std::cerr << "Less than 2 parents for diff" << std::endl;
                return;
            }
            auto parent1 = Node::GetNode(parents[0]);
            auto parent2 = Node::GetNode(parents[1]);
            if (!parent1 || !parent2) {
                std::cerr << "Parent nodes not found for subtraction" << std::endl;
                return;
            }
            double result = parent1->GetData() - parent2->GetData();
            node->SetData(result);
        }
        else if (node->GetOp() == "*"){
            double result=1.0;
            for(auto pid : parents){
                auto parent = Node::GetNode(pid);
                if (!parent) {
                    std::cerr << "Parent node " << pid << " not found" << std::endl;
                    return;
                }
                result *= parent->GetData();
            }
            node->SetData(result);
        }
        else if (node->GetOp() == "/"){
            if (parents.size() < 2) {
                std::cerr << "Less than 2 parents for div" << std::endl;
                return;
            }
            auto parent1 = Node::GetNode(parents[0]);
            auto parent2 = Node::GetNode(parents[1]);
            if (!parent1 || !parent2) {
                std::cerr << "Parent nodes not found for division" << std::endl;
                return;
            }
            double result = parent1->GetData() / parent2->GetData();
            node->SetData(result);
        }
        else if (node->GetOp() == "negate"){
            if (parents.empty()) {
                std::cerr << "No parent for negate operation" << std::endl;
                return;
            }
            auto parent = Node::GetNode(parents[0]);
            if (!parent) {
                std::cerr << "Parent node not found for negate" << std::endl;
                return;
            }
            node->SetData(-parent->GetData());                 
        }
        else if (node->GetOp()=="exp") {
            if (parents.empty()) {
                std::cerr << "No parent for exp opreation" << std::endl;
            }
            auto parent = Node::GetNode(parents[0]);
            node->SetData(exp(parent->GetData()));
        }
        else if(node->GetOp() == "pow"){
            if (parents.empty()){
                std::cerr << "No parent for pow opreation" << std::endl;
            }
            auto parent1 = Node::GetNode(parents[0]);
            auto parent2 = Node::GetNode(parents[1]);
            node->SetData(pow(parent1->GetData(),parent2->GetData()));
        } 
        else if (node->GetOp() == "log"){
            if (parents.empty()) {
                std::cerr << "No parents for log opreation" << std::endl;
            }
            auto parent = Node::GetNode(parents[0]);
            if (parent->GetData() <= 0.0) {
                std::cerr << "can't pass <=0 into log function" << std::endl;
                return;
            }
            node->SetData(log(parent->GetData()));
        }
        else if (node->GetOp() == "sqrt"){
            if (parents.empty()) {
                std::cerr << "No parents for sqrt opreation" << std::endl;
            }
            auto parent = Node::GetNode(parents[0]);
            node->SetData(sqrt(parent->GetData()));
        }
        else if (node->GetOp().substr(0,4) == "pow_"){
            if (parents.empty()){
                 std::cerr << "No parents for pow_ "<< node->GetOp().substr(4) << " opreation" << std::endl;
            }
            std::string str_exp = node->GetOp().substr(4);
            auto exp = std::stod(str_exp);
            auto parent = Node::GetNode(parents[0]);
            node->SetData(pow(parent->GetData(),exp));
        }
    }
}
#endif // FORWARD_H
