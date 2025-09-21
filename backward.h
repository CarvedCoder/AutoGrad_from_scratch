#ifndef BACKWARD_H
#define BACKWARD_H
#include "node.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

static void backward(std::vector<int>&order){
    std::reverse(order.begin(),order.end());
    for(auto n : order){Node::GetNode(n)->ZeroGrad();}
    Node::GetNode(order[0])->setGrad(1.0);
    for(auto n : order){
        auto node = Node::GetNode(n);
        if (!node){
            std::cerr << "Node corruption (nullptr returned)" << std::endl;
            return;
        }
        auto parents = node->GetParents(); 
        if (node->GetOp() == "input"){continue;}
        else if (node->GetOp() == "+"){
            for (auto pid : parents){
                Node::GetNode(pid)->AddGrad(node->GetGrad());
            } 
        }
        else if (node->GetOp()=="*") {
            auto parent1 = Node::GetNode(parents[0]);
            auto parent2 = Node::GetNode(parents[1]);
            parent1->AddGrad(node->GetGrad()*parent2->GetData());
            parent2->AddGrad(node->GetGrad()*parent1->GetData());
        }
        else if (node->GetOp() == "-"){
            auto parent1 = Node::GetNode(parents[0]);
            auto parent2 = Node::GetNode(parents[1]);
            parent1->AddGrad(node->GetGrad());
            parent2->AddGrad(-node->GetGrad());
        }
        else if (node->GetOp() =="/") {
            auto parent1 = Node::GetNode(parents[0]);
            auto parent2 = Node::GetNode(parents[1]);
            parent1->AddGrad(node->GetGrad() * 1/parent2->GetData());
            parent2->AddGrad(node->GetGrad() * (-parent1->GetData() /(parent2->GetData()*parent2->GetData())));
        }
        else if (node->GetOp()=="negate") {
            auto parent = Node::GetNode(parents[0]);
            parent->AddGrad(-node->GetGrad());
        }
        else if (node->GetOp() == "pow") {
            auto parent1 = Node::GetNode(parents[0]);
            auto parent2 = Node::GetNode(parents[1]);
            parent1->AddGrad(node->GetGrad()*parent2->GetData()*pow(parent1->GetData(),parent2->GetData()-1));
            parent2->AddGrad(node->GetGrad()*pow(parent1->GetData(),parent2->GetData())*log(parent1->GetData()));
        }
        else if (node->GetOp() == "log"){
            auto parent = Node::GetNode(parents[0]);
            parent->AddGrad(node->GetGrad() * (1/parent->GetData()));
        }
        else if(node->GetOp() == "sqrt"){
            auto parent = Node::GetNode(parents[0]);
            if (parent->GetData() < 0.0) {
                std::cerr << "Can't pass <0.0 in pow(something,-1/2)" <<std::endl;
                return;
            }
            parent->AddGrad(node->GetGrad()/(2.0 *sqrt(parent->GetData())));
        }
        else if(node->GetOp() == "exp"){
            auto parent = Node::GetNode(parents[0]);
            parent->AddGrad(node->GetGrad()*exp(parent->GetData()));
        }
        else if (node->GetOp().substr(0,4) == "pow_") {
            std::string str_exp = node->GetOp().substr(4);
            auto exp = std::stod(str_exp);
            auto parent = Node::GetNode(parents[0]);
            parent->AddGrad(node->GetGrad()*exp*pow(parent->GetData(),exp-1));
        }

    }
}

#endif // BACKWARD_H

