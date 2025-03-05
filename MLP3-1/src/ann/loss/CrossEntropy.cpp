/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Tanh.cpp
 * Author: ltsach
 * 
 * Created on September 1, 2024, 7:03 PM
 */

#include "layer/Tanh.h"
#include "sformat/fmt_lib.h"
#include "ann/functions.h"

Tanh::Tanh(string name) {
    if(trim(name).size() != 0) m_sName = name;
    else m_sName = "Tanh_" + to_string(++m_unLayer_idx);
}

Tanh::Tanh(const Tanh& orig) {
    m_sName = "Tanh_" + to_string(++m_unLayer_idx);
}

Tanh::~Tanh() {
}

xt::xarray<double> Tanh::forward(xt::xarray<double> X) {
    //YOUR CODE IS HERE
    // Calculate the Tanh activation
    xt::xarray<double> Y = xt::tanh(X);
    
    // Cache the output for use in the backward pass if in training mode
    if (m_trainable) {
        m_aCached_Y = Y;
    }
    
    return Y;
}
xt::xarray<double> Tanh::backward(xt::xarray<double> DY) {
    //YOUR CODE IS HERE
    // Retrieve the cached output from the forward pass
    xt::xarray<double> Y = m_aCached_Y;
    
    // Calculate the gradient of the loss with respect to the input
    xt::xarray<double> DX = DY * (1 - Y * Y);
    
    return DX;
}

string Tanh::get_desc(){
    string desc = fmt::format("{:<10s}, {:<15s}:",
                    "Tanh", this->getname());
    return desc;
}
