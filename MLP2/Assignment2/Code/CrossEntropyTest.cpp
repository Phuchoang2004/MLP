#include <iostream>
#include "loss/CrossEntropy.h"
#include "xtensor/xarray.hpp"

int main() {
    // Create a CrossEntropy loss object
    CrossEntropy loss_layer(REDUCE_MEAN);

    // Define sample predictions and targets
    xt::xarray<double> predictions = {{0.1, 0.9}, {0.8, 0.2}};
    xt::xarray<double> targets = {{0, 1}, {1, 0}};

    // Compute the forward pass
    double loss = loss_layer.forward(predictions, targets);
    std::cout << "Loss: " << loss << std::endl;

    // Compute the backward pass
    xt::xarray<double> grad = loss_layer.backward();
    std::cout << "Gradient: " << grad << std::endl;

    return 0;
}