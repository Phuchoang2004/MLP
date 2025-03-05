#include <iostream>
#include "layer/Sigmoid.h"
#include "tensor/xtensor/xarray.hpp"

void test_sigmoid_forward() {
    Sigmoid sigmoid;
    xt::xarray<double> X = {0.0, 2.0, -2.0};
    xt::xarray<double> Y = sigmoid.forward(X);

    std::cout << "Forward pass result: " << Y << std::endl;
}

void test_sigmoid_backward() {
    Sigmoid sigmoid;
    xt::xarray<double> X = {0.0, 2.0, -2.0};
    xt::xarray<double> DY = {1.0, 1.0, 1.0};
    sigmoid.forward(X); // Cache the forward pass result
    xt::xarray<double> DX = sigmoid.backward(DY);

    std::cout << "Backward pass result: " << DX << std::endl;
}

int main() {
    test_sigmoid_forward();
    test_sigmoid_backward();
    return 0;
}