#ifndef CLASS_H
#define CLASS_H 
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Node :public std::enable_shared_from_this<Node>{
private:
    double data;
    std::string op;
    int id;
    inline static int next_id = 0;
    inline static std::unordered_map<int, std::weak_ptr<Node>> registry;
    std::vector<int> prev;
    double grad=0.0;
private:
    Node(double data , const std::string &op) : data(data),op(op), grad(0.0),id(next_id++){}
public:
    using Nodeptr = std::shared_ptr<Node>;
    static Nodeptr CreateNode(double data,const std::string &op= "input"){
        auto node = Nodeptr (new Node(data,op));
        registry[node->id] = node; 
        return node;
    } 

    static void topoDfs(const Nodeptr& node,std::unordered_set<int>&seen,std::vector<int>&order){
        if(!node || seen.count(node->id)){return;}
        seen.insert(node->id);
        for(int pid:node->prev){
            if(auto p = GetNode(pid)) {topoDfs(p,seen,order);}
        }
        order.push_back(node->id);
    }

    static std::vector<int> topoSort(const Nodeptr& root){
        std::unordered_set<int> seen;
        std::vector<int> order;
        topoDfs(root, seen, order);
        return order;
    }
    void addParent(const int pid){prev.push_back(pid);}
    double GetData(){return data;}
    double GetGrad(){return grad;}
    std::string GetOp(){return op;}
    int GetId(){return id;}
    std::vector<int> GetParents()const {return prev;} 
    static Nodeptr GetNode(int id){
        if(registry.find(id) != registry.end()){
            if(auto ptr = registry[id].lock()){
                return ptr;
            }
        }
        return nullptr;
    }
    void setGrad(const double new_grad){grad = new_grad;}
    void SetData(const double new_data){data=new_data;}
    void AddGrad(const double new_grad){grad += new_grad;}
    void ZeroGrad(){grad=0.0;}
};

namespace NodeOps {
    static Node::Nodeptr operator +(const Node::Nodeptr& x1,const Node::Nodeptr& x2){
        auto result = Node::CreateNode(0.0,"+");
        result->addParent(x1->GetId());
        result->addParent(x2->GetId());
        return result;
    }
    static Node::Nodeptr operator -(const Node::Nodeptr& x1,const Node::Nodeptr& x2){
        auto result = Node::CreateNode(0.0,"-");
        result->addParent(x1->GetId());
        result->addParent(x2->GetId());
        return result;
    }
    static Node::Nodeptr operator *(const Node::Nodeptr& x1,const Node::Nodeptr& x2){
        auto result = Node::CreateNode(0.0,"*");
        result->addParent(x1->GetId());
        result->addParent(x2->GetId());
        return result;
    } 
    static Node::Nodeptr operator /(const Node::Nodeptr& x1,const Node::Nodeptr& x2){
        auto result = Node::CreateNode(0.0,"/");
        result->addParent(x1->GetId());
        result->addParent(x2->GetId());
        return result;
    }
    static Node::Nodeptr operator -(const Node::Nodeptr& x){
        auto result = Node::CreateNode(0.0,"negate");
        result->addParent(x->GetId());
        return result;
    }
    static Node::Nodeptr node_pow(const Node::Nodeptr&x1,const Node::Nodeptr&x2){
        auto result = Node::CreateNode(0.0,"pow");
        result->addParent(x1->GetId());
        result->addParent(x2->GetId());
        return result;
    }
    static Node::Nodeptr node_pow(const Node::Nodeptr& x,double y){
        auto result = Node::CreateNode(0.0,"pow_"+std::to_string(y));
        result->addParent(x->GetId());
        return result;
    }
    static Node::Nodeptr node_exp(const Node::Nodeptr&x){
        auto result = Node::CreateNode(0.0,"exp");
        result->addParent(x->GetId());
        return result;
    }
    static Node::Nodeptr node_log(const Node::Nodeptr&x){
        auto result = Node::CreateNode(0.0,"log");
        result->addParent(x->GetId());
        return result;
    }
    static Node::Nodeptr node_sqrt(const Node::Nodeptr&x){
        auto result = Node::CreateNode(0.0,"sqrt");
        result->addParent(x->GetId());
        return result;
    }
}
#endif // CLASS_H

