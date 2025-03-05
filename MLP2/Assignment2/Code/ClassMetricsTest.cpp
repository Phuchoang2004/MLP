#include <iostream>
#include "metrics/ClassMetrics.h"
#include "xtensor/xarray.hpp"

void test_class_metrics() {
    // Create a ClassMetrics object
    ClassMetrics metrics(3); // Assume 3 classes for this example

    // Create dummy data for testing
    xt::xarray<unsigned long> y_true = {0, 1, 2, 1, 0};
    xt::xarray<unsigned long> y_pred = {0, 2, 1, 1, 0};

    // Accumulate metrics
    metrics.accumulate(y_true, y_pred);

    // Get and print metrics
    auto result = metrics.get_metrics();
    std::cout << "Metrics: " << result << std::endl;

    // Reset metrics
    metrics.reset_metrics();
    auto reset_result = metrics.get_metrics();
    std::cout << "Metrics after reset: " << reset_result << std::endl;
}

int main() {
    test_class_metrics();
    return 0;
}