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

    static Nodeptr add(const Nodeptr&x1, const Nodeptr&x2){
        auto result = CreateNode(0.0,"+");
        result->prev.push_back(x1->GetId());
        result->prev.push_back(x2->GetId());
        return result;
    }

    static Nodeptr diff(const Nodeptr&x1, const Nodeptr&x2){
        auto result = CreateNode(0.0,"-");
        result->prev.push_back(x1->GetId());
        result->prev.push_back(x2->GetId());
        return result;
    }

    static Nodeptr mul(const Nodeptr&x1, const Nodeptr&x2){
        auto result = CreateNode(0.0,"*");
        result->prev.push_back(x1->GetId());
        result->prev.push_back(x2->GetId());
        return result;
    }

    static Nodeptr div(const Nodeptr&x1, const Nodeptr&x2){
        auto result = CreateNode(0.0,"/");
        result->prev.push_back(x1->GetId());
        result->prev.push_back(x2->GetId());
        return result;
    }

    static void topoDfs(const Nodeptr& node,std::unordered_set<int>&seen,std::vector<int>&order){
        if(!node || seen.count(node->id)){return;}
        seen.insert(node->id);
        for(int pid:node->prev){
            if(auto p2 = GetNode(pid)) {topoDfs(p2,seen,order);}
        }
        order.push_back(node->id);
    }

    static std::vector<int> topoSort(const Nodeptr& root){
        std::unordered_set<int> seen;
        std::vector<int> order;
        topoDfs(root, seen, order);
        return order;
    }


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
};



