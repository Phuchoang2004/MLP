#include <iostream>
#include "layer/FCLayer.h"
#include "tensor/xtensor_lib.h"

void test_FCLayer() {
    // Create an FCLayer with 3 inputs and 2 outputs
    FCLayer fc(3, 2, true);

    // Create a batch of 4 input samples, each with 3 features
    xt::xarray<double> X = {{1.0, 2.0, 3.0},
                            {4.0, 5.0, 6.0},
                            {7.0, 8.0, 9.0},
                            {10.0, 11.0, 12.0}};

    // Perform forward pass
    xt::xarray<double> Y = fc.forward(X);
    std::cout << "Forward pass output:\n" << Y << std::endl;

    // Create a gradient of the loss with respect to the output
    xt::xarray<double> DY = {{0.1, 0.2},
                             {0.3, 0.4},
                             {0.5, 0.6},
                             {0.7, 0.8}};

    // Perform backward pass
    xt::xarray<double> DX = fc.backward(DY);
    std::cout << "Backward pass output (gradient w.r.t input):\n" << DX << std::endl;
}

int main() {
    test_FCLayer();
    return 0;
}