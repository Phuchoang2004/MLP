#include <iostream>
#include "ReLU.h"

void test_relu() {
    ReLU relu("test_relu");

    // Test forward pass
    xt::xarray<double> X = {{-1.0, 2.0}, {3.0, -4.0}};
    xt::xarray<double> Y = relu.forward(X);
    std::cout << "Forward pass result:\n" << Y << std::endl;

    // Expected output: {{0.0, 2.0}, {3.0, 0.0}}

    // Test backward pass
    xt::xarray<double> DY = {{0.1, 0.2}, {0.3, 0.4}};
    xt::xarray<double> DX = relu.backward(DY);
    std::cout << "Backward pass result:\n" << DX << std::endl;

    // Expected output: {{0.0, 0.2}, {0.3, 0.0}}
}

int main() {
    test_relu();
    return 0;
}