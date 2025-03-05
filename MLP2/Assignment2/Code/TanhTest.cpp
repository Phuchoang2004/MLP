#include "layer/Tanh.h"
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <iostream>
#include <cassert>

void test_tanh_forward() {
    Tanh tanh_layer;
    xt::xarray<double> input = {0.0, 1.0, -1.0, 0.5, -0.5};
    xt::xarray<double> expected_output = xt::tanh(input);
    
    xt::xarray<double> output = tanh_layer.forward(input);
    
    assert(xt::allclose(output, expected_output));
    std::cout << "Forward pass test passed!" << std::endl;
}

void test_tanh_backward() {
    Tanh tanh_layer;
    tanh_layer.set_train_mode(true);
    
    xt::xarray<double> input = {0.0, 1.0, -1.0, 0.5, -0.5};
    xt::xarray<double> forward_output = tanh_layer.forward(input);
    
    xt::xarray<double> DY = {1.0, 1.0, 1.0, 1.0, 1.0};
    xt::xarray<double> expected_DX = DY * (1 - forward_output * forward_output);
    
    xt::xarray<double> DX = tanh_layer.backward(DY);
    
    assert(xt::allclose(DX, expected_DX));
    std::cout << "Backward pass test passed!" << std::endl;
}

int main() {
    test_tanh_forward();
    test_tanh_backward();
    return 0;
}