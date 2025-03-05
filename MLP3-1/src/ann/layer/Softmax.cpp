/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Softmax.cpp
 * Author: ltsach
 * 
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"
#include "ann/functions.h"
#include "sformat/fmt_lib.h"
#include <filesystem> //require C++17
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name): m_nAxis(axis) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {
}

Softmax::~Softmax() {
}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
    //YOUR CODE IS HERE
    xt::xarray<double> softmax_output = softmax(X, m_nAxis);
    m_aCached_Y = softmax_output;
    return m_aCached_Y;
}
xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
    //YOUR CODE IS HERE
    auto softmax_output = m_aCached_Y;
    xt::xarray<double> gradient = xt::zeros<double>(DY.shape());
    
    for (size_t batch = 0; batch < DY.shape()[0]; batch++) {
        auto s = xt::view(softmax_output, batch, xt::all());
        auto dy = xt::view(DY, batch, xt::all());
        auto dx = xt::view(gradient, batch, xt::all());
        
        double sum_sdy = xt::sum(s * dy)();
        dx = s * (dy - sum_sdy);
    }
    
    return gradient;
}

string Softmax::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}: {:4d}",
                    "Softmax", this->getname(), m_nAxis);
    return desc;
}
