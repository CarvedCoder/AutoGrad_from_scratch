#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include "class.h"
#include "forward.h"
#include "backward.h"

void testBasicOperations() {
    using namespace NodeOps;
    std::cout << "=== Testing Basic Operations ===" << std::endl;
    
    // Test addition
    auto a = Node::CreateNode(3.0);
    auto b = Node::CreateNode(2.0);
    auto c = a + b;  // Should be 5.0, grad_a = 1, grad_b = 1
    
    auto order = Node::topoSort(c);
    forward(order);
    backward(order);
    
    std::cout << "Addition: " << a->GetData() << " + " << b->GetData() 
              << " = " << c->GetData() << std::endl;
    std::cout << "Gradients: a=" << a->GetGrad() << ", b=" << b->GetGrad() << std::endl;
    
    // Test subtraction
    auto d = Node::CreateNode(5.0);
    auto e = Node::CreateNode(3.0);
    auto f = d - e;  // Should be 2.0, grad_d = 1, grad_e = -1
    
    order = Node::topoSort(f);
    forward(order);
    backward(order);
    
    std::cout << "Subtraction: " << d->GetData() << " - " << e->GetData() 
              << " = " << f->GetData() << std::endl;
    std::cout << "Gradients: d=" << d->GetGrad() << ", e=" << e->GetGrad() << std::endl;
    
    // Test multiplication
    auto g = Node::CreateNode(4.0);
    auto h = Node::CreateNode(3.0);
    auto i = g * h;  // Should be 12.0, grad_g = 3, grad_h = 4
    
    order = Node::topoSort(i);
    forward(order);
    backward(order);
    
    std::cout << "Multiplication: " << g->GetData() << " * " << h->GetData() 
              << " = " << i->GetData() << std::endl;
    std::cout << "Gradients: g=" << g->GetGrad() << ", h=" << h->GetGrad() << std::endl;
    
    // Test division
    auto j = Node::CreateNode(12.0);
    auto k = Node::CreateNode(4.0);
    auto l = j / k;  // Should be 3.0, grad_j = 1/4 = 0.25, grad_k = -12/16 = -0.75
    
    order = Node::topoSort(l);
    forward(order);
    backward(order);
    
    std::cout << "Division: " << j->GetData() << " / " << k->GetData() 
              << " = " << l->GetData() << std::endl;
    std::cout << "Gradients: j=" << j->GetGrad() << ", k=" << k->GetGrad() << std::endl;
    std::cout << std::endl;
}

void testUnaryOperations() {
    using namespace NodeOps;
    std::cout << "=== Testing Unary Operations ===" << std::endl;
    
    // Test negation
    auto a = Node::CreateNode(5.0);
    auto b = -a;  // Should be -5.0, grad_a = -1
    
    auto order = Node::topoSort(b);
    forward(order);
    backward(order);
    
    std::cout << "Negation: -" << a->GetData() << " = " << b->GetData() << std::endl;
    std::cout << "Gradient: a=" << a->GetGrad() << std::endl;
    
    // Test exponential
    auto c = Node::CreateNode(1.0);
    auto d = node_exp(c);  // Should be e ≈ 2.718, grad_c = e
    
    order = Node::topoSort(d);
    forward(order);
    backward(order);
    
    std::cout << "Exponential: exp(" << c->GetData() << ") = " << d->GetData() << std::endl;
    std::cout << "Gradient: c=" << c->GetGrad() << std::endl;
    
    // Test natural log
    auto e = Node::CreateNode(2.71828);  // approximately e
    auto f = node_log(e);  // Should be ≈ 1.0, grad_e = 1/e
    
    order = Node::topoSort(f);
    forward(order);
    backward(order);
    
    std::cout << "Natural log: log(" << e->GetData() << ") = " << f->GetData() << std::endl;
    std::cout << "Gradient: e=" << e->GetGrad() << std::endl;
    
    // Test square root
    auto g = Node::CreateNode(9.0);
    auto h = node_sqrt(g);  // Should be 3.0, grad_g = 1/(2*sqrt(9)) = 1/6
    
    order = Node::topoSort(h);
    forward(order);
    backward(order);
    
    std::cout << "Square root: sqrt(" << g->GetData() << ") = " << h->GetData() << std::endl;
    std::cout << "Gradient: g=" << g->GetGrad() << std::endl;
    std::cout << std::endl;
}

void testPowerOperations() {
    using namespace NodeOps;
    std::cout << "=== Testing Power Operations ===" << std::endl;
    
    // Test node^node power
    auto a = Node::CreateNode(2.0);
    auto b = Node::CreateNode(3.0);
    auto c = node_pow(a, b);  // Should be 8.0, grad_a = 3*2^2 = 12, grad_b = 8*ln(2)
    
    auto order = Node::topoSort(c);
    forward(order);
    backward(order);
    
    std::cout << "Power (node^node): " << a->GetData() << "^" << b->GetData() 
              << " = " << c->GetData() << std::endl;
    std::cout << "Gradients: a=" << a->GetGrad() << ", b=" << b->GetGrad() << std::endl;
    
    // Test node^constant power
    auto d = Node::CreateNode(3.0);
    auto e = node_pow(d, 2.0);  // Should be 9.0, grad_d = 2*3^1 = 6
    
    order = Node::topoSort(e);
    forward(order);
    backward(order);
    
    std::cout << "Power (node^const): " << d->GetData() << "^2.0 = " << e->GetData() << std::endl;
    std::cout << "Gradient: d=" << d->GetGrad() << std::endl;
    
    // Test fractional power
    auto f = Node::CreateNode(8.0);
    auto g = node_pow(f, 1.0/3.0);  // Should be 2.0 (cube root), grad_f = (1/3)*8^(-2/3)
    
    order = Node::topoSort(g);
    forward(order);
    backward(order);
    
    std::cout << "Fractional power: " << f->GetData() << "^(1/3) = " << g->GetData() << std::endl;
    std::cout << "Gradient: f=" << f->GetGrad() << std::endl;
    std::cout << std::endl;
}

void testComplexExpression() {
    using namespace NodeOps;
    std::cout << "=== Testing Complex Expression ===" << std::endl;
    
    // Test: f(x,y) = (x*y + exp(x)) / sqrt(y)
    auto x = Node::CreateNode(2.0);
    auto y = Node::CreateNode(4.0);
    
    auto xy = x * y;           // 2*4 = 8
    auto exp_x = node_exp(x);  // exp(2) ≈ 7.389
    auto numerator = xy + exp_x; // 8 + 7.389 = 15.389
    auto sqrt_y = node_sqrt(y);  // sqrt(4) = 2
    auto result = numerator / sqrt_y; // 15.389 / 2 ≈ 7.695
    
    auto order = Node::topoSort(result);
    forward(order);
    backward(order);
    
    std::cout << "Complex expression: (x*y + exp(x)) / sqrt(y)" << std::endl;
    std::cout << "x = " << x->GetData() << ", y = " << y->GetData() << std::endl;
    std::cout << "Result = " << result->GetData() << std::endl;
    std::cout << "Gradients: dx = " << x->GetGrad() << ", dy = " << y->GetGrad() << std::endl;
    
    // Manual verification of gradients:
    // ∂f/∂x = (y + exp(x)) / sqrt(y) = (4 + exp(2)) / 2
    // ∂f/∂y = (x - (x*y + exp(x))/(2*y)) / sqrt(y)
    double manual_dx = (4.0 + exp(2.0)) / 2.0;
    double manual_dy = (2.0 - (8.0 + exp(2.0))/(2.0*4.0)) / 2.0;
    std::cout << "Expected gradients: dx = " << manual_dx << ", dy = " << manual_dy << std::endl;
    std::cout << std::endl;
}

void testChainRule() {
    using namespace NodeOps;
    std::cout << "=== Testing Chain Rule ===" << std::endl;
    
    // Test: f(x) = log(exp(x^2))  Should simplify to x^2, so gradient should be 2x
    auto x = Node::CreateNode(3.0);
    auto x_squared = node_pow(x, 2.0);  // x^2 = 9
    auto exp_x2 = node_exp(x_squared);  // exp(9)
    auto result = node_log(exp_x2);     // log(exp(9)) = 9
    
    auto order = Node::topoSort(result);
    forward(order);
    backward(order);
    
    std::cout << "Chain rule test: log(exp(x^2)) where x = " << x->GetData() << std::endl;
    std::cout << "Result = " << result->GetData() << std::endl;
    std::cout << "Gradient: dx = " << x->GetGrad() << std::endl;
    std::cout << "Expected gradient: 2x = " << 2.0 * x->GetData() << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << std::fixed << std::setprecision(6);
    
    testBasicOperations();
    testUnaryOperations();
    testPowerOperations();
    testComplexExpression();
    testChainRule();
    
    std::cout << "=== Original Test Case ===" << std::endl;
    using namespace NodeOps;
    
    auto a1 = Node::CreateNode(3.14);
    auto a2 = Node::CreateNode(1.12);
    auto b1 = a1 + a2;
    auto b2 = Node::CreateNode(2.33);
    auto c1 = b1 - b2;
    auto c3 = Node::CreateNode(1.13);
    auto d1 = c3 * c1;
    auto d2 = Node::CreateNode(1.18);
    auto e = d1 / d2;
    auto neg_e = -e;
    
    auto order = Node::topoSort(neg_e);
    
    forward(order);
    std::cout << "Forward pass completed" << std::endl;
    std::cout << "Final result: " << neg_e->GetData() << std::endl;
    
    backward(order);
    std::cout << "Backward pass completed" << std::endl;
    std::cout << "Gradient of a1: " << a1->GetGrad() << std::endl;
    
    return 0;
}
