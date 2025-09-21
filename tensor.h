#ifndef TENSOR_H
#define TENSOR_H
#include <memory>
#include <stdexcept>
#include <vector>

class Tensor:std::enable_shared_from_this<Tensor>{
private:
    std::vector<double> data; 
    std::vector<int> shape;
    std::vector<int> stride;
private:
    Tensor(const std::vector<double>&data,const std::vector<int>&shape,const std::vector<int>&stride): data(data),shape(shape),stride(stride){}
public:
    using Tensorptr = std::shared_ptr<Tensor>;
    static Tensorptr CreateTensor(const std::vector<double>& data,const std::vector<int>& shape){
        int expected_size = 1;
        for (auto dim:shape) {
            expected_size *= dim;
        }
        if (expected_size != data.size() || expected_size < 0){
            throw std::invalid_argument("shape and data size don't match or -ve int passed in shape");
        }
        auto stride = calculate_strides(shape); 
        auto tensor = Tensorptr(new Tensor(data,shape,stride));
        return tensor;
    }

    static std::vector<int> calculate_strides(const std::vector<int>&shape){
        if (shape.size() == 0) return {};
        std::vector<int> strides(shape.size());
        int stride_val = 1;
        for(int i = shape.size()-1;i >= 0;i--){
            strides[i] = stride_val;
            stride_val *= shape[i];
        }
    return strides;
    } 

    
    static Tensorptr CreateScalar(double data){
        return CreateTensor({data},{});
    }

    static Tensorptr CreateZeros(const std::vector<int>&shape){
        if (shape.size()==0) {return CreateScalar(0.0);}
        int total_size = 1;
        for(auto dim:shape) total_size *= dim;
        std::vector<double> zeros(total_size,0.0);
        return CreateTensor(zeros,shape);
    }

    static Tensorptr CreateOnes(const std::vector<int>&shape){
        if (shape.size()==0) {return CreateScalar(1.0);}
        int total_size = 1;
        for (auto dim : shape) total_size*=dim;
        std::vector<double> ones(total_size,1.0);
        return CreateTensor(ones,shape);
    } 

    std::vector<int>GetShape(){return shape;}
    int GetTotalSize(){return data.size();}
    double GetDataElem(int i){return data[i];}
    void SetDataElem(int i,double val){data[i]=val;}
    double &operator()(int i){return data[i];}
    double &operator()(int i,int j){return data[i*stride[0] + j*stride[1]];}
    double &operator()(int i,int j,int k){return data[i*stride[0]+j*stride[1]+k*stride[2]];}
};

namespace TensorOps{

    static Tensor::Tensorptr operator+(const std::shared_ptr<Tensor>&t1,const std::shared_ptr<Tensor>&t2){
        if(t1->GetShape() != t2->GetShape()){
            throw std::invalid_argument("size of tensors don't match for add operator");
        }
        auto result = Tensor::CreateZeros(t1->GetShape());
        if(t1->GetShape().size() == 0){
            result->SetDataElem(0, t1->GetDataElem(0)+t2->GetDataElem(0));
            return result;
        }
        
        for(int i = 0; i < t1->GetTotalSize();i++){
            result->SetDataElem(i,t1->GetDataElem(i)+ t2->GetDataElem(i));
        }
        return result;
    }
    static Tensor::Tensorptr operator-(const std::shared_ptr<Tensor>&t1,const std::shared_ptr<Tensor>&t2){
        if(t1->GetShape() != t2->GetShape()){
            throw std::invalid_argument("size of tensors don't match for add operator");
        }
        auto result = Tensor::CreateZeros(t1->GetShape());
        if(t1->GetShape().size() == 0){
            result->SetDataElem(0, t1->GetDataElem(0) - t2->GetDataElem(0));
            return result;
        }
        
        for(int i = 0; i < t1->GetTotalSize();i++){
            result->SetDataElem(i,t1->GetDataElem(i) - t2->GetDataElem(i));
        }
        return result;
    }
    static Tensor::Tensorptr operator*(const std::shared_ptr<Tensor>&t1,const std::shared_ptr<Tensor>&t2){
        if(t1->GetShape() != t2->GetShape()){
            throw std::invalid_argument("size of tensors don't match for add operator");
        }
        auto result = Tensor::CreateZeros(t1->GetShape());
        if(t1->GetShape().size() == 0){
            result->SetDataElem(0, t1->GetDataElem(0) * t2->GetDataElem(0));
            return result;
        }
        
        for(int i = 0; i < t1->GetTotalSize();i++){
            result->SetDataElem(i,t1->GetDataElem(i) * t2->GetDataElem(i));
        }
        return result;
    }

}
#endif // TENSOR_H
