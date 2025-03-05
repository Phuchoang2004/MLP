void test_softmax_properties() {
    Softmax softmax;
    xt::xarray<double> X = {{1.0, 2.0, 3.0}, {1.0, 2.0, 3.0}};
    xt::xarray<double> Y = softmax.forward(X);
    
    // Check that each value is in the range (0, 1)
    assert(xt::all(Y >= 0.0) && xt::all(Y <= 1.0));
    
    // Check that the sum of each row is 1
    xt::xarray<double> row_sums = xt::sum(Y, {1});
    assert(xt::allclose(row_sums, xt::xarray<double>({1.0, 1.0})));
    
    std::cout << "Softmax properties test passed!" << std::endl;
}

int main() {
    test_softmax_properties();
    return 0;
}