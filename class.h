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
private:
    Node(double data , const std::string &op) : data(data),op(op), id(next_id++){}
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

static void forward(const std::vector<int>&order){
    for(auto n:order){ 
        auto node = GetNode(n);
        if (!node){
            std::cerr << "Node corruption (nullptr returned)" << std::endl;
            return;
        }
        auto parents = node->GetParents();
        if (node->GetOp() == "input") {continue;}
        else if (node->GetOp() == "+"){
            double result=0.0;
            for(auto pid : parents){
                auto parent = GetNode(pid);
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
            auto parent1 = GetNode(parents[0]);
            auto parent2 = GetNode(parents[1]);
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
                auto parent = GetNode(pid);
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
            auto parent1 = GetNode(parents[0]);
            auto parent2 = GetNode(parents[1]);
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
            auto parent = GetNode(parents[0]);
            if (!parent) {
                std::cerr << "Parent node not found for negate" << std::endl;
                return;
            }
            node->SetData(-parent->GetData());                 
        }
    }
}

    void addParent(const int pid){prev.push_back(pid);}
    double GetData(){return data;}
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
    void SetData(const double new_data){data=new_data;}
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
}



